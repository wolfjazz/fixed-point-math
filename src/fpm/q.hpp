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
    overflow OVF_ACTION = overflow::FORBIDDEN  ///< overflow action performed when overflow check is positive
>
class q final
{
    static_assert(std::is_integral_v<BASE_T>, "base type must be integral");

public:
    static constexpr BASE_T MIN = v2s<BASE_T, F>(V_MIN);  ///< minimum value of integer value range
    static constexpr BASE_T MAX = v2s<BASE_T, F>(V_MAX);  ///< maximum value of integer value range
    static constexpr double RESOLUTION = v2s<double, -F>(1.);  ///< real resolution of this type

    /// Related sq type.
    template< double SQ_V_MIN = V_MIN, double SQ_V_MAX = V_MAX >
    using sq = fpm::sq< BASE_T, F, SQ_V_MIN, SQ_V_MAX >;

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
    static q construct(BASE_T value) noexcept {
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

    /// Conversion to related sq type.
    template< double SQ_V_MIN = V_MIN, double SQ_V_MAX = V_MAX, overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    sq<SQ_V_MIN, SQ_V_MAX> constexpr to_sq() const noexcept {
        static constexpr BASE_T SQ_MIN = v2s<BASE_T, F>(SQ_V_MIN);
        static constexpr BASE_T SQ_MAX = v2s<BASE_T, F>(SQ_V_MAX);
        BASE_T sqValue = value;

        // no overflow check is needed if the value range is the same or larger;
        // do not include a value range check in this case
        static constexpr bool overflowCheckNeeded = (SQ_V_MIN - V_MIN) > RESOLUTION || (V_MAX - SQ_V_MAX) > RESOLUTION;
        if constexpr (overflowCheckNeeded) {

            // if overflow is FORBIDDEN, remind the user that it needs to be changed for this function
            static_assert(!overflowCheckNeeded || overflow::FORBIDDEN != OVF_ACTION_OVERRIDE,
                "runtime overflow check is not allowed; allow, or override default overflow action");

            if constexpr (overflow::ASSERT == OVF_ACTION_OVERRIDE) {
                if ( !(sqValue >= SQ_MIN && sqValue <= SQ_MAX)) {
                    assert(false);  // sqValue is out of user range
                }
            }
            else if constexpr (overflow::SATURATE == OVF_ACTION_OVERRIDE) {
                if ( !(sqValue >= SQ_MIN)) {
                    sqValue = SQ_MIN;
                }
                if ( !(sqValue <= SQ_MAX)) {
                    sqValue = SQ_MAX;
                }
            }
            else { /* overflow::ALLOWED */ }
        }

        return sq<SQ_V_MIN, SQ_V_MAX>(sqValue);
    }

    /// Reveals the integer value stored in the memory.
    BASE_T reveal() const noexcept {
        return value;
    }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning Unscaled, real value will be of specified target integer type (no floats at runtime).
    ///          There can be a significant loss of precision.
    template< typename TARGET_T = double >
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
