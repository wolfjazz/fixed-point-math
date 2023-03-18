/** \file
 * Declaration of the sq class template.
 */

#ifndef _FPM_SQ_HPP_
#define _FPM_SQ_HPP_

#include "fpm.hpp"
#include <limits>


namespace fpm {

template<
    typename BASE_T,  ///< type of the scaled integer stored in memory
    scaling_t F,      ///< number of fraction bits (precision 2^-F)
    double V_MIN,     ///< minimum real value represented by this type
    double V_MAX      ///< maximum real value represented by this type
>
class sq final
{
    static_assert(std::is_integral<BASE_T>::value, "base type must be integral");

public:
    static constexpr BASE_T T_MIN = std::numeric_limits<BASE_T>::min();  ///< minimum possible value of base type
    static constexpr BASE_T T_MAX = std::numeric_limits<BASE_T>::max();  ///< maximum possible value of base type
    static constexpr BASE_T MIN = v2s<BASE_T, F>(V_MIN);  ///< minimum value of integer value range
    static constexpr BASE_T MAX = v2s<BASE_T, F>(V_MAX);  ///< maximum value of integer value range

    /// Explicit compile-time-only "constructor" from floating-point value.
    /// Performs compile-time overflow checks.
    template< double VALUE >
    static consteval sq from_real() {
        constexpr BASE_T scaledValue = v2s<BASE_T, F>(VALUE);  // does not compile if scaled value does not fit BASE_T

        // do not compile if initial value does not fit user-defined value range
        static_assert(scaledValue >= MIN && scaledValue <= MAX, "value is out of user range");

        return sq(scaledValue);
    }

    /// Destructor.
    constexpr ~sq()
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
    sq() = delete;

    /// Explicit, possibly compile-time-only constructor from integer value.
    explicit constexpr sq(BASE_T value) noexcept : value(value)
    {}

    /// scaled integer value that represents a fixed-point value; stored in memory
    BASE_T const value;
};

}


#endif
// EOF
