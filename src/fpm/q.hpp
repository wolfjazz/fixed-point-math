/** \file
 * Declaration of the q class template.
 */

#ifndef _FPM_Q_HPP_
#define _FPM_Q_HPP_

#include "sq.hpp"
#include <cassert>
#include <algorithm>
#include <limits>
#include <type_traits>


namespace fpm {

template<
    typename BASE_T,  ///< type of the scaled integer stored in memory
    scaling_t F,      ///< number of fraction bits (precision 2^-F)
    double REAL_V_MIN_,  ///< minimum real value represented by this type
    double REAL_V_MAX_,  ///< maximum real value represented by this type
    overflow OVF_ACTION = overflow::FORBIDDEN >  ///< overflow action performed when overflow check is positive
requires (
       ValidBaseType<BASE_T>
    && RealLimitsInRangeOfBaseType<BASE_T, F, REAL_V_MIN_, REAL_V_MAX_>
)
class q final {
public:
    using base_t = BASE_T;
    static constexpr double REAL_V_MIN = REAL_V_MIN_;  ///< minimum real value
    static constexpr double REAL_V_MAX = REAL_V_MAX_;  ///< maximum real value
    static constexpr BASE_T V_MIN = v2s<BASE_T, F>(REAL_V_MIN_);  ///< minimum value of integer value range
    static constexpr BASE_T V_MAX = v2s<BASE_T, F>(REAL_V_MAX_);  ///< maximum value of integer value range
    static constexpr double RESOLUTION = v2s<double, -F>(1);  ///< real resolution of this type

    /// Corresponding sq type.
    template< double SQ_REAL_V_MIN = REAL_V_MIN, double SQ_REAL_V_MAX = REAL_V_MAX >
    using sq = fpm::sq< BASE_T, F, SQ_REAL_V_MIN, SQ_REAL_V_MAX >;

    /// Named "constructor" from a runtime variable (lvalue) or a constant (rvalue).
    /// \note Overflow check is always included unless explicitly disabled.
    template< overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    requires ( RuntimeCheckAllowedWhenNeeded<OVF_ACTION_OVERRIDE> )
    static constexpr q construct(BASE_T value) noexcept {

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

    /// Named compile-time-only "constructor" from a floating-point value. This will use v2s to scale
    /// the given floating-point value at compile-time and then call the construct method with the
    /// scaled integer value at runtime.
    /// \note When the value is within bounds, no overflow check is included. If it is out of range,
    ///       an overflow check is performed according to the overflow settings.
    /// \note: When a real value is wrapped into a q value, there is an inherent rounding error due
    /// to the limited resolution. This error is called 'representation error' and it refers to the
    /// deviation from the initial real value when the q value is unscaled to a real value again.
    /// Usually the scaling error is in the order of the resolution of the q type.
    template< double REAL_VALUE, overflow OVF_ACTION_OVERRIDE = OVF_ACTION >
    requires ( RealValueScaledFitsBaseType<BASE_T, F, REAL_VALUE> )
    static consteval q from_real() {
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

    /// Named "Copy-Constructor" from another q type with the same base type.
    /// \note When a q value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the q value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< overflow OVF_ACTION_OVERRIDE = OVF_ACTION,
        double _REAL_V_MIN_FROM, double _REAL_V_MAX_FROM, scaling_t _F_FROM, overflow _OVF_FROM,
        // include overflow check if value range of this type is smaller than range of from-type,
        // or if scaling of this type is larger;
        // note: real limits are compared because scaled integers with different q's cannot be compared so easily
        bool _OVF_CHECK_NEEDED = (_REAL_V_MIN_FROM < REAL_V_MIN || REAL_V_MAX < _REAL_V_MAX_FROM
                                  || is_ovf_stricter(OVF_ACTION_OVERRIDE, _OVF_FROM) || F > _F_FROM) >
    requires ( RuntimeCheckAllowedWhenNeeded<OVF_ACTION_OVERRIDE, _OVF_CHECK_NEEDED> )
    static constexpr q from_q(q<BASE_T, _F_FROM, _REAL_V_MIN_FROM, _REAL_V_MAX_FROM, _OVF_FROM> const &from) noexcept {
        using interm_b_t = interm_t<BASE_T>;
        interm_b_t fromValueScaled = s2s<interm_b_t, _F_FROM, F>(from.reveal());

        if constexpr (_OVF_CHECK_NEEDED) {
            if constexpr (overflow::ASSERT == OVF_ACTION_OVERRIDE) {
                if ( !(fromValueScaled >= V_MIN && fromValueScaled <= V_MAX)) {
                    assert(false);  // from-value is out of range
                }
            }
            else if constexpr (overflow::SATURATE == OVF_ACTION_OVERRIDE) {
                if ( !(fromValueScaled >= V_MIN)) {
                    fromValueScaled = static_cast<interm_b_t>(V_MIN);
                }
                if ( !(fromValueScaled <= V_MAX)) {
                    fromValueScaled = static_cast<interm_b_t>(V_MAX);
                }
            }
            else { /* overflow::ALLOWED, overflow::NO_CHECK: no checks performed */ }
        }

        return q(static_cast<BASE_T>(fromValueScaled));
    }

    /// Named "constructor" from a related sq variable.
    /// \note Overflow check is included if the range of the sq type is larger than the range of this q type.
    template< overflow OVF_ACTION_OVERRIDE = OVF_ACTION,
        double _SQ_REAL_V_MIN, double _SQ_REAL_V_MAX,
        BASE_T _SQ_V_MIN = v2s<BASE_T, F>(_SQ_REAL_V_MIN), BASE_T _SQ_V_MAX = v2s<BASE_T, F>(_SQ_REAL_V_MAX),
        // include overflow check if the value range of q is smaller
        bool _OVF_CHECK_NEEDED = (_SQ_V_MIN < V_MIN || V_MAX < _SQ_V_MAX) >
    requires ( RuntimeCheckAllowedWhenNeeded<OVF_ACTION_OVERRIDE, _OVF_CHECK_NEEDED> )
    static constexpr q from_sq(fpm::sq<BASE_T, F, _SQ_REAL_V_MIN, _SQ_REAL_V_MAX> const &fromSq) noexcept {
        BASE_T qValue = fromSq.value;

        if constexpr (_OVF_CHECK_NEEDED) {
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

    /// Copy-Constructor from the same type.
    q(q const &) noexcept = default;

    /// Copy-Assignment from the same type.
    q& operator=(q const &) noexcept = default;

    /// Move-Constructor from the same type.
    q(q&&) noexcept = default;

    /// Move-Assignment from the same type.
    q& operator=(q&&) noexcept = default;

    /// Destructor.
    constexpr ~q()
    {}

    /// Copy-Assignment from a different q type with the same base type.
    template< double _REAL_V_MIN_RHS, double _REAL_V_MAX_RHS, scaling_t _F_RHS, overflow _OVF_RHS >
    requires (
        _F_RHS != F
    )
    q& operator=(q<BASE_T, _F_RHS, _REAL_V_MIN_RHS, _REAL_V_MAX_RHS, _OVF_RHS> const &rhs) noexcept {
        value = q::from_q(rhs).value;
        return *this;
    }

    /// Explicit cast to a different q type with a different base type.
    template< typename _BASE_T_C, scaling_t _F_C, double _REAL_V_MIN_C, double _REAL_V_MAX_C, overflow _OVF_C,
        // include overflow check if value range of cast-type is smaller than range of this type,
        // or if scaling of cast-type is larger;
        // note: real limits are compared because scaled integers with different base types and q's
        //       cannot be compared so easily
        bool _OVF_CHECK_NEEDED = (REAL_V_MIN < _REAL_V_MIN_C || _REAL_V_MAX_C < REAL_V_MAX
                                  || is_ovf_stricter(_OVF_C, OVF_ACTION) || _F_C > F) >
    requires (
        !std::is_same_v<BASE_T, _BASE_T_C>
        && RuntimeCheckAllowedWhenNeeded<_OVF_C, _OVF_CHECK_NEEDED>
    )
    operator q<_BASE_T_C, _F_C, _REAL_V_MIN_C, _REAL_V_MAX_C, _OVF_C> () const {
        using target_q = q<_BASE_T_C, _F_C, _REAL_V_MIN_C, _REAL_V_MAX_C, _OVF_C>;
        using interm_c_t = interm_t<_BASE_T_C>;

        // first cast, then scale value
        auto cValue = s2s<interm_c_t, F, _F_C>(static_cast<_BASE_T_C>(value));

        if constexpr (_OVF_CHECK_NEEDED) {
            if constexpr (overflow::ASSERT == _OVF_C) {
                if ( !(cValue >= target_q::V_MIN && cValue <= target_q::V_MAX)) {
                    assert(false);  // from-value is out of range
                }
            }
            else if constexpr (overflow::SATURATE == _OVF_C) {
                if ( !(cValue >= target_q::V_MIN)) {
                    cValue = static_cast<interm_c_t>(target_q::V_MIN);
                }
                if ( !(cValue <= target_q::V_MAX)) {
                    cValue = static_cast<interm_c_t>(target_q::V_MAX);
                }
            }
            else { /* overflow::ALLOWED, overflow::NO_CHECK: no checks performed */ }
        }

        // create target value; disable overflow check to avoid that value is checked again
        return target_q::template construct<overflow::NO_CHECK>(static_cast<_BASE_T_C>(cValue));
    }

    /// Conversion to the related sq type.
    template< double SQ_REAL_V_MIN = REAL_V_MIN, double SQ_REAL_V_MAX = REAL_V_MAX, overflow OVF_ACTION_OVERRIDE = OVF_ACTION,
        BASE_T _SQ_V_MIN = v2s<BASE_T, F>(SQ_REAL_V_MIN), BASE_T _SQ_V_MAX = v2s<BASE_T, F>(SQ_REAL_V_MAX),
        // include overflow check if the value range of sq is smaller
        bool _OVF_CHECK_NEEDED = (V_MIN < _SQ_V_MIN || _SQ_V_MAX < V_MAX) >
    requires ( RuntimeCheckAllowedWhenNeeded<OVF_ACTION_OVERRIDE, _OVF_CHECK_NEEDED> )
    sq<SQ_REAL_V_MIN, SQ_REAL_V_MAX> constexpr to_sq() const noexcept {
        BASE_T sqValue = value;

        if constexpr (_OVF_CHECK_NEEDED) {
            if constexpr (overflow::ASSERT == OVF_ACTION_OVERRIDE) {
                if ( !(sqValue >= _SQ_V_MIN && sqValue <= _SQ_V_MAX)) {
                    assert(false);  // sqValue is out of range
                }
            }
            else if constexpr (overflow::SATURATE == OVF_ACTION_OVERRIDE) {
                if ( !(sqValue >= _SQ_V_MIN)) {
                    sqValue = _SQ_V_MIN;
                }
                if ( !(sqValue <= _SQ_V_MAX)) {
                    sqValue = _SQ_V_MAX;
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
    TARGET_T to_real() const noexcept {
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
