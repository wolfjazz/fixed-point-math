/** \file
 * Declaration of the q class template.
 */

#ifndef _FPM_Q_HPP_
#define _FPM_Q_HPP_

#include "sq.hpp"
#include <cassert>
#include <algorithm>


namespace fpm {

template<
    typename BASE_T,  ///< type of the scaled integer stored in memory
    scaling_t F,      ///< number of fraction bits (precision 2^-F)
    double V_MIN,     ///< minimum real value represented by this type
    double V_MAX,     ///< maximum real value represented by this type
    overflow OVF_ACTION = overflow::ASSERT  ///< overflow action performed when overflow check is positive
>
class q final
{
    static_assert(std::is_integral_v<BASE_T>, "base type must be integral");

public:
    static constexpr BASE_T MIN = v2s<BASE_T, F>(V_MIN);  ///< minimum value of integer value range
    static constexpr BASE_T MAX = v2s<BASE_T, F>(V_MAX);  ///< maximum value of integer value range

    /// Explicit compile-time-only named "constructor" from a floating-point value.
    /// \note Performs compile-time overflow checks and does not compile if value is out of (user-) range.
    template< double VALUE >
    static consteval q from_real() {
        constexpr BASE_T scaledValue = v2s<BASE_T, F>(VALUE);  // does not compile if scaled value does not fit BASE_T

        // do not compile if initial value does not fit user-defined value range
        static_assert(scaledValue >= MIN && scaledValue <= MAX, "value is out of user range");

        return q(scaledValue);
    }

    /// Explicit runtime named "constructor" from a runtime variable (lvalue) or a constant (rvalue).
    static q construct(BASE_T value) {
        static_assert(overflow::FORBIDDEN != OVF_ACTION, "runtime overflow check is not allowed");
        if constexpr (overflow::ASSERT == OVF_ACTION) {
            if ( !(value >= MIN && value <= MAX)) {
                assert(false);  // value is out of user range
            }
        }
        else if constexpr (overflow::SATURATE == OVF_ACTION) {
            if ( !(value >= MIN)) {
                value = MIN;
            }
            if ( !(value <= MAX)) {
                value = MAX;
            }
        }
        else { /* overflow::ALLOWED */ }

        return q(value);
    }

    /// Destructor.
    constexpr ~q()
    {}

    /// Reveals the integer value stored in the memory.
    BASE_T reveal() const {
        return value;
    }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning Unscaled, real value will be of specified target integer type (no floats at runtime).
    ///          There can be a significant loss of precision.
    template< typename TARGET_T >
    TARGET_T unwrap() const noexcept {
        return s2s<TARGET_T, F, 0>(value);
    }

private:
    // delete default (runtime) constructor
    q() = delete;

    /// Explicit, possibly compile-time constructor from integer value.
    explicit constexpr q(BASE_T value) noexcept : value(value)
    {}

    //explicit q(BASE_T value) noexcept : value(value)

    /// scaled integer value that represents a fixed-point value; stored in memory
    BASE_T value;
};

}


#endif
// EOF
