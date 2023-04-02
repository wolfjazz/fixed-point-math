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
    double REAL_V_MIN_ARG,  ///< minimum real value represented by this type
    double REAL_V_MAX_ARG,  ///< maximum real value represented by this type
    overflow OVF_ACTION = overflow::FORBIDDEN  ///< overflow action performed when overflow check is positive
>
class q final
{
    static_assert(std::is_integral_v<BASE_T>, "base type must be integral");

public:
    static constexpr double REAL_V_MIN = REAL_V_MIN_ARG;  ///< minimum real value
    static constexpr double REAL_V_MAX = REAL_V_MAX_ARG;  ///< maximum real value
    static constexpr BASE_T V_MIN = v2s<BASE_T, F>(REAL_V_MIN_ARG);  ///< minimum value of integer value range
    static constexpr BASE_T V_MAX = v2s<BASE_T, F>(REAL_V_MAX_ARG);  ///< maximum value of integer value range
    static constexpr double RESOLUTION = v2s<double, -F>(1);  ///< real resolution of this type

    /// Corresponding sq type.
    template< double SQ_REAL_V_MIN = REAL_V_MIN, double SQ_REAL_V_MAX = REAL_V_MAX >
    using sq = fpm::sq< BASE_T, F, SQ_REAL_V_MIN, SQ_REAL_V_MAX >;

    /// Named "constructor" from a floating-point value. This will use v2s to scale the
    /// given floating-point value at compile-time and then call the construct method with the scaled
    /// integer value.
    /// \note When the value is within bounds, no overflow check is included. If it is out of range,
    ///       an overflow check is performed according to the overflow settings.
    template< double REAL_VALUE, overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    static consteval q from_real() {
        // does not compile if scaled value does not fit BASE_T
        constexpr BASE_T scaledValue = v2s<BASE_T, F>(REAL_VALUE);

        constexpr overflow overflowAction = (scaledValue >= V_MIN && scaledValue <= V_MAX)
            ? overflow::NO_CHECK  // value is within bounds; no overflow check needed
            : OVF_ACTION_OVERRIDE;
        return q::construct<overflowAction>(scaledValue);
    }

    /// Named "constructor" from a scaled integer value. This can be used to construct a
    /// well-behaved q value at compile-time without a redundant overflow check.
    /// \note When the value is within bounds, no overflow check is included. If it is out of range,
    ///       an overflow check is performed according to the overflow settings.
    template< BASE_T VALUE, overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    static consteval q from_scaled() {
        constexpr overflow overflowAction = (VALUE >= V_MIN && VALUE <= V_MAX)
            ? overflow::NO_CHECK  // value is within bounds; no overflow check needed
            : OVF_ACTION_OVERRIDE;
        return q::construct<overflowAction>(VALUE);
    }

    /// Named "constructor" from a runtime variable (lvalue) or a constant (rvalue).
    /// \note Overflow check is always included unless explicitly disabled.
    template< overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    static constexpr q construct(BASE_T value) noexcept {

        // if overflow is FORBIDDEN, remind user that overflow setting needs to be changed for this method
        static_assert(overflow::FORBIDDEN != OVF_ACTION_OVERRIDE,
            "runtime overflow check is not allowed; allow, or override default overflow action");

        if constexpr (overflow::ASSERT == OVF_ACTION_OVERRIDE) {
            if ( !(value >= V_MIN && value <= V_MAX)) {
                assert(false);  // value is out of range
            }
        }
        else if constexpr (overflow::SATURATE == OVF_ACTION_OVERRIDE) {
            if ( !(value >= V_MIN)) {
                value = V_MIN;
            }
            if ( !(value <= V_MAX)) {
                value = V_MAX;
            }
        }
        else { /* overflow::ALLOWED, overflow::NO_CHECK: no checks performed */ }

        return q(value);
    }

    /// Named "constructor" from a related sq variable.
    /// \note Overflow check is included if the range of the sq type is larger than the range of this q type.
    template< overflow OVF_ACTION_OVERRIDE = OVF_ACTION, double SQ_REAL_V_MIN, double SQ_REAL_V_MAX >
    static constexpr q from_sq(fpm::sq<BASE_T, F, SQ_REAL_V_MIN, SQ_REAL_V_MAX> const &fromSq) noexcept {
        static constexpr BASE_T SQ_V_MIN = v2s<BASE_T, F>(SQ_REAL_V_MIN);
        static constexpr BASE_T SQ_V_MAX = v2s<BASE_T, F>(SQ_REAL_V_MAX);

        BASE_T qValue = fromSq.reveal();

        // include overflow check if value range of q is smaller
        static constexpr bool overflowCheckNeeded = SQ_V_MIN < V_MIN || V_MAX < SQ_V_MAX;
        if constexpr (overflowCheckNeeded) {

            // if overflow is FORBIDDEN, remind the user that overflow needs to be changed for this method
            static_assert(!overflowCheckNeeded || overflow::FORBIDDEN != OVF_ACTION_OVERRIDE,
                "runtime overflow check is not allowed; allow, or override default overflow action");

            if constexpr (overflow::ASSERT == OVF_ACTION_OVERRIDE) {
                if ( !(qValue >= V_MIN && qValue <= V_MAX)) {
                    assert(false);  // sqValue is out of range
                }
            }
            else if constexpr (overflow::SATURATE == OVF_ACTION_OVERRIDE) {
                if ( !(qValue >= V_MIN)) {
                    qValue = V_MIN;
                }
                if ( !(qValue <= V_MAX)) {
                    qValue = V_MAX;
                }
            }
            else { /* overflow::ALLOWED, overflow::NO_CHECK: no checks performed */ }
        }

        return q(qValue);
    }

    /// Destructor.
    constexpr ~q()
    {}

    /// Conversion to related sq type.
    template< double SQ_REAL_V_MIN = REAL_V_MIN, double SQ_REAL_V_MAX = REAL_V_MAX, overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    sq<SQ_REAL_V_MIN, SQ_REAL_V_MAX> constexpr to_sq() const noexcept {
        static constexpr BASE_T SQ_V_MIN = v2s<BASE_T, F>(SQ_REAL_V_MIN);
        static constexpr BASE_T SQ_V_MAX = v2s<BASE_T, F>(SQ_REAL_V_MAX);

        BASE_T sqValue = value;

        // include overflow check if the value range of sq is smaller
        static constexpr bool overflowCheckNeeded = V_MIN < SQ_V_MIN || SQ_V_MAX < V_MAX;
        if constexpr (overflowCheckNeeded) {

            // if overflow is FORBIDDEN, remind the user that overflow needs to be changed for this method
            static_assert(!overflowCheckNeeded || overflow::FORBIDDEN != OVF_ACTION_OVERRIDE,
                "runtime overflow check is not allowed; allow, or override default overflow action");

            if constexpr (overflow::ASSERT == OVF_ACTION_OVERRIDE) {
                if ( !(sqValue >= SQ_V_MIN && sqValue <= SQ_V_MAX)) {
                    assert(false);  // sqValue is out of range
                }
            }
            else if constexpr (overflow::SATURATE == OVF_ACTION_OVERRIDE) {
                if ( !(sqValue >= SQ_V_MIN)) {
                    sqValue = SQ_V_MIN;
                }
                if ( !(sqValue <= SQ_V_MAX)) {
                    sqValue = SQ_V_MAX;
                }
            }
            else { /* overflow::ALLOWED, overflow::NO_CHECK: no checks performed */ }
        }

        return sq<SQ_REAL_V_MIN, SQ_REAL_V_MAX>(sqValue);
    }

    /// Reveals the integer value stored in the memory.
    BASE_T reveal() const noexcept {
        return value;
    }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning This conversion is expensive if the target type is a floating-point type.
    ///          If the target type is an integral type, there can be a significant loss of precision.
    ///          Use carefully!
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

    /// scaled integer value that represents a fixed-point value; stored in memory
    BASE_T value;
};

}


#endif
// EOF
