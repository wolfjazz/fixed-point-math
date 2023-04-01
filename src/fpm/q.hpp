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

    /// Corresponding sq type.
    template< double SQ_V_MIN = V_MIN, double SQ_V_MAX = V_MAX >
    using sq = fpm::sq< BASE_T, F, SQ_V_MIN, SQ_V_MAX >;

    /// Explicit named "constructor" from a floating-point value. This will use v2s to scale the
    /// given floating-point value at compile-time and then call the construct method with the scaled
    /// integer value.
    /// \note When the value is within bounds, no overflow check is included. If it is out or range,
    ///       an overflow check is performed according to the overflow settings.
    template< double REAL_VALUE, overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    static consteval q from_real() {
        // does not compile if scaled value does not fit BASE_T
        constexpr BASE_T scaledValue = v2s<BASE_T, F>(REAL_VALUE);

        constexpr overflow overflowAction = (scaledValue >= MIN && scaledValue <= MAX)
            ? overflow::NO_CHECK  // value is within bounds; no overflow check needed
            : OVF_ACTION_OVERRIDE;
        return q::construct<overflowAction>(scaledValue);
    }

    /// Explicit named "constructor" from a scaled integer value. This can be used to construct a
    /// well-behaved q value at compile-time without a redundant overflow check.
    /// \note When the value is within bounds, no overflow check is included. If it is out or range,
    ///       an overflow check is performed according to the overflow settings.
    template< BASE_T VALUE, overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    static consteval q from_scaled() {
        constexpr overflow overflowAction = (VALUE >= MIN && VALUE <= MAX)
            ? overflow::NO_CHECK  // value is within bounds; no overflow check needed
            : OVF_ACTION_OVERRIDE;
        return q::construct<overflowAction>(VALUE);
    }

    /// Explicit named "constructor" from a runtime variable (lvalue) or a constant (rvalue).
    /// \note Overflow check is always included unless explicitly disabled.
    template< overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    static constexpr q construct(BASE_T value) noexcept {

        // if overflow is FORBIDDEN, remind user that overflow setting needs to be changed for this method
        static_assert(overflow::FORBIDDEN != OVF_ACTION_OVERRIDE,
            "runtime overflow check is not allowed; allow, or override default overflow action");

        if constexpr (overflow::ASSERT == OVF_ACTION_OVERRIDE) {
            if ( !(value >= MIN && value <= MAX)) {
                assert(false);  // value is out of range
            }
        }
        else if constexpr (overflow::SATURATE == OVF_ACTION_OVERRIDE) {
            if ( !(value >= MIN)) {
                value = MIN;
            }
            if ( !(value <= MAX)) {
                value = MAX;
            }
        }
        else { /* overflow::ALLOWED, overflow::NO_CHECK: no checks performed */ }

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

            // if overflow is FORBIDDEN, remind the user that overflow needs to be changed for this method
            static_assert(!overflowCheckNeeded || overflow::FORBIDDEN != OVF_ACTION_OVERRIDE,
                "runtime overflow check is not allowed; allow, or override default overflow action");

            if constexpr (overflow::ASSERT == OVF_ACTION_OVERRIDE) {
                if ( !(sqValue >= SQ_MIN && sqValue <= SQ_MAX)) {
                    assert(false);  // sqValue is out of range
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
            else { /* overflow::ALLOWED, overflow::NO_CHECK: no checks performed */ }
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
