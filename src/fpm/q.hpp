/** \file
 * Implementation of the q class template.
 */

#ifndef _FPM___Q_HPP_
#define _FPM___Q_HPP_

#include "sq.hpp"


/** \addtogroup grp_fpm
 * \{ */
namespace fpm {

template<
    typename BASE_T_,  ///< type of the scaled integer stored in memory
    scaling_t F_,      ///< number of fraction bits (precision 2^-F)
    double REAL_V_MIN_ = v2s<double, -F_>(std::numeric_limits<BASE_T_>::min()),  ///< minimum real value represented by this type
    double REAL_V_MAX_ = v2s<double, -F_>(std::numeric_limits<BASE_T_>::max()),  ///< maximum real value represented by this type
    overflow OVF_BX_ = overflow::FORBIDDEN  ///< overflow behavior; overflow is forbidden by default
>
requires (
    ValidBaseType<BASE_T_>
    && ValidScaling<BASE_T_, F_>
    && RealLimitsInRangeOfBaseType<BASE_T_, F_, REAL_V_MIN_, REAL_V_MAX_>
)
class q final {
public:
    using base_t = BASE_T_;  /// integral base type
    static constexpr scaling_t F = F_;  ///< number of fraction bits
    static constexpr double REAL_V_MIN = REAL_V_MIN_;  ///< minimum real value
    static constexpr double REAL_V_MAX = REAL_V_MAX_;  ///< maximum real value
    static constexpr base_t V_MIN = v2s<base_t, F>(REAL_V_MIN_);  ///< minimum value of integer value range
    static constexpr base_t V_MAX = v2s<base_t, F>(REAL_V_MAX_);  ///< maximum value of integer value range
    static constexpr double RESOLUTION = v2s<double, -F>(1);  ///< real resolution of this type
    static constexpr overflow OVF_BX = OVF_BX_;  ///< overflow behavior

    // friend all q types so that private members of similar types can be accessed for construction
    // Note: As of May 2023, partial specializations cannot be friended, so we friend q in general.
    template< typename _BASE_T_Q, scaling_t _F_Q, double _REAL_V_MIN_Q, double _REAL_V_MAX_Q, overflow _OVF_Q >
    requires (
        ValidBaseType<_BASE_T_Q>
        && ValidScaling<_BASE_T_Q, _F_Q>
        && RealLimitsInRangeOfBaseType<_BASE_T_Q, _F_Q, _REAL_V_MIN_Q, _REAL_V_MAX_Q>
    )
    friend class q;

    /// Corresponding sq type.
    template< double SQ_REAL_V_MIN = REAL_V_MIN, double SQ_REAL_V_MAX = REAL_V_MAX >
    using sq = fpm::sq< base_t, F, SQ_REAL_V_MIN, SQ_REAL_V_MAX >;

    /// Create a new q type with the same base type and scaling but a different real value range.
    template< double NEW_REAL_V_MIN, double NEW_REAL_V_MAX, overflow OVF_BX_OVERRIDE = OVF_BX >
    struct relimit { using type = q< base_t, F, NEW_REAL_V_MIN, NEW_REAL_V_MAX, OVF_BX_OVERRIDE >; };

    /// Type alias for relimit::type.
    template< double NEW_REAL_V_MIN, double NEW_REAL_V_MAX, overflow OVF_BX_OVERRIDE = OVF_BX >
    using relimit_t = relimit<NEW_REAL_V_MIN, NEW_REAL_V_MAX, OVF_BX_OVERRIDE>::type;

    /// Named "constructor" from a runtime variable (lvalue) or a constant (rvalue).
    /// \note Overflow check is always included unless explicitly disabled.
    template< overflow OVF_BX_OVERRIDE = OVF_BX >
    requires (
        RuntimeOverflowCheckAllowedWhenNeeded<OVF_BX_OVERRIDE>
    )
    static constexpr
    q construct(base_t value) noexcept {
        // perform overflow check
        _i::check_overflow<OVF_BX_OVERRIDE, base_t>(value, V_MIN, V_MAX);

        return q(value);
    }

    /// Helper struct template for q::from_real<.>
    template< double REAL_VALUE, overflow OVF_BX_OVERRIDE = OVF_BX >
    requires (
        RealValueScaledFitsBaseType<base_t, F, REAL_VALUE>
        && CompileTimeOnlyOverflowCheckPossible<OVF_BX_OVERRIDE>
    )
    struct wrap_real {
        static constexpr base_t _scaledValue = v2s<base_t, F>(REAL_VALUE);
        static constexpr overflow _overflowBehavior = (_scaledValue >= V_MIN && _scaledValue <= V_MAX)
            ? overflow::NO_CHECK  // value is within bounds; no overflow check needed
            : OVF_BX_OVERRIDE;
        static constexpr q wrapped = q::construct<_overflowBehavior>( _scaledValue );
    };

    /// Helper struct template for q::from_scaled<.>
    template< base_t VALUE, overflow OVF_BX_OVERRIDE = OVF_BX >
    requires (
        CompileTimeOnlyOverflowCheckPossible<OVF_BX_OVERRIDE>
    )
    struct wrap_scaled {
        static constexpr overflow _overflowBehavior = (VALUE >= V_MIN && VALUE <= V_MAX)
            ? overflow::NO_CHECK  // value is within bounds; no overflow check needed
            : OVF_BX_OVERRIDE;
        static constexpr q wrapped = q::construct<_overflowBehavior>( VALUE );
    };

    /// Named compile-time-only "constructor" from a floating-point value. This will use v2s to scale
    /// the given floating-point value at compile-time before the q value is constructed with the
    /// scaled integer value.
    /// \note When the value is within bounds, no overflow check is needed. If it is out of range,
    ///       an overflow check is performed at compile-time according to the overflow settings.
    /// \note: When a real value is wrapped into a q value, there is an inherent rounding error due
    /// to the limited resolution. This error is called 'representation error' and it refers to the
    /// deviation from the initial real value when the q value is unscaled to a real value again.
    /// Usually the scaling error is in the order of the resolution of the q type.
    template< double REAL_VALUE, overflow OVF_BX_OVERRIDE = OVF_BX >
    static constexpr
    q from_real = wrap_real<REAL_VALUE, OVF_BX_OVERRIDE>::wrapped;

    /// Alias for q::from_real<., ovf::FORBIDDEN>
    template< double REAL_VALUE >
    static constexpr
    q from_real_novf = from_real<REAL_VALUE, ovf::FORBIDDEN>;

    /// Alias for q::from_real<., ovf::SATURATE>
    template< double REAL_VALUE >
    static constexpr
    q from_real_sat = from_real<REAL_VALUE, ovf::SATURATE>;

    /// Alias for q::from_real<., ovf::ALLOWED> and q::from_real<., ovf::NO_CHECK>
    template< double REAL_VALUE >
    static constexpr
    q from_real_ovf = from_real<REAL_VALUE, ovf::ALLOWED>;

    /// Named compile-time-only "constructor" from a scaled integer value. This can be used to
    /// construct a well-behaved q value at compile-time without a redundant overflow check.
    /// \note When the value is within bounds, no overflow check is needed. If it is out of range,
    ///       an overflow check is performed at compile-time according to the overflow settings.
    template< base_t VALUE, overflow OVF_BX_OVERRIDE = OVF_BX >
    static constexpr
    q from_scaled = wrap_scaled<VALUE, OVF_BX_OVERRIDE>::wrapped;

    /// Alias for q::from_scaled<., ovf::FORBIDDEN>
    template< base_t VALUE >
    static constexpr
    q from_scaled_novf = from_scaled<VALUE, ovf::FORBIDDEN>;

    /// Alias for q::from_scaled<., ovf::SATURATE>
    template< base_t VALUE >
    static constexpr
    q from_scaled_sat = from_scaled<VALUE, ovf::SATURATE>;

    /// Alias for q::from_scaled<., ovf::NO_CHECK> and q::from_scaled<., ovf::ALLOWED>
    template< base_t VALUE >
    static constexpr
    q from_scaled_ovf = from_scaled<VALUE, ovf::NO_CHECK>;

    /// Named "Copy-Constructor" from another q type with the same base type.
    /// \note When a q value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the q value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< overflow OVF_BX_OVERRIDE = OVF_BX,
        double _REAL_V_MIN_FROM, double _REAL_V_MAX_FROM, scaling_t _F_FROM, overflow _OVF_FROM,
        // include overflow check if value range of this type is smaller than range of from-type
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different q's cannot be compared so easily
        bool _OVF_CHECK_NEEDED = (_REAL_V_MIN_FROM < REAL_V_MIN || REAL_V_MAX < _REAL_V_MAX_FROM
                                  || is_ovf_stricter_v<OVF_BX_OVERRIDE, _OVF_FROM>
                                  || is_ovf_stricter_v<OVF_BX, OVF_BX_OVERRIDE>) >
    requires (
        ScalingIsPossible<base_t, _F_FROM, base_t, F>
        && RuntimeOverflowCheckAllowedWhenNeeded<OVF_BX_OVERRIDE, _OVF_CHECK_NEEDED>
    )
    static constexpr
    q from_q(q<base_t, _F_FROM, _REAL_V_MIN_FROM, _REAL_V_MAX_FROM, _OVF_FROM> const &from) noexcept {
        using from_q = q<base_t, _F_FROM, _REAL_V_MIN_FROM, _REAL_V_MAX_FROM, _OVF_FROM>;
        using interm_b_t = interm_t<base_t>;
        interm_b_t fromValueScaled = s2s<interm_b_t, from_q::F, F>( from.reveal() );

        // perform overflow check if needed
        if constexpr (_OVF_CHECK_NEEDED) {
            _i::check_overflow<OVF_BX_OVERRIDE, interm_b_t>(fromValueScaled, V_MIN, V_MAX);
        }

        return q(static_cast<base_t>(fromValueScaled));
    }

    /// Named "constructor" from a related sq variable.
    /// \note Overflow check is included if the range of the sq type is larger than the range of this q type.
    template< overflow OVF_BX_OVERRIDE = OVF_BX,
        double _SQ_REAL_V_MIN, double _SQ_REAL_V_MAX,
        base_t _SQ_V_MIN = v2s<base_t, F>(_SQ_REAL_V_MIN), base_t _SQ_V_MAX = v2s<base_t, F>(_SQ_REAL_V_MAX),
        // include overflow check if the value range of q is smaller
        bool _OVF_CHECK_NEEDED = (_SQ_V_MIN < V_MIN || V_MAX < _SQ_V_MAX) >
    requires (
        RuntimeOverflowCheckAllowedWhenNeeded<OVF_BX_OVERRIDE, _OVF_CHECK_NEEDED>
    )
    static constexpr
    q from_sq(fpm::sq<base_t, F, _SQ_REAL_V_MIN, _SQ_REAL_V_MAX> const &fromSq) noexcept {
        base_t qValue = fromSq.value;

        // perform overflow check if needed
        if constexpr (_OVF_CHECK_NEEDED) {
            _i::check_overflow<OVF_BX_OVERRIDE, base_t>(qValue, V_MIN, V_MAX);
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
    constexpr ~q() {}

    /// Copy-Assignment from a different q type with the same base type.
    template< double _REAL_V_MIN_RHS, double _REAL_V_MAX_RHS, scaling_t _F_RHS, overflow _OVF_RHS >
    requires (
        _F_RHS != F
        && ScalingIsPossible<base_t, _F_RHS, base_t, F>
    )
    q& operator=(q<base_t, _F_RHS, _REAL_V_MIN_RHS, _REAL_V_MAX_RHS, _OVF_RHS> const &rhs) noexcept {
        value = q::from_q(rhs).value;
        return *this;
    }

    /// Explicit (static-) cast to a different q type with a different base type.
    /// \note If a cast does not work it's most probably due to unfulfilled requirements. Double check
    ///       whether a runtime overflow check is needed and make sure that it is allowed!
    template< typename _BASE_T_C, scaling_t _F_C, double _REAL_V_MIN_C, double _REAL_V_MAX_C, overflow _OVF_C,
        // include overflow check if value range of cast-type is smaller than range of this type
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different base types and q's
        //       cannot be compared so easily
        bool _OVF_CHECK_NEEDED = (REAL_V_MIN < _REAL_V_MIN_C || _REAL_V_MAX_C < REAL_V_MAX
                                  || is_ovf_stricter_v<_OVF_C, OVF_BX>) >
    requires (
        !std::is_same_v<base_t, _BASE_T_C>
        && ScalingIsPossible<base_t, F, _BASE_T_C, _F_C>
        && RuntimeOverflowCheckAllowedWhenNeeded<_OVF_C, _OVF_CHECK_NEEDED>
    )
    explicit
    operator q<_BASE_T_C, _F_C, _REAL_V_MIN_C, _REAL_V_MAX_C, _OVF_C>() const noexcept {
        using target_q = q<_BASE_T_C, _F_C, _REAL_V_MIN_C, _REAL_V_MAX_C, _OVF_C>;
        using interm_b_t = interm_t<base_t>;
        using interm_c_t = interm_t<_BASE_T_C>;

        // first cast, then scale value
        auto cValue = s2s<interm_c_t, F, target_q::F>(value);

        // perform overflow check if needed
        if constexpr (_OVF_CHECK_NEEDED) {
            _i::check_overflow<target_q::OVF_BX, interm_c_t, interm_b_t>(cValue, target_q::V_MIN, target_q::V_MAX);
        }

        // create target value; disable overflow check to avoid that value is checked again
        return target_q( static_cast<target_q::base_t>(cValue) );
    }

    /// Conversion to the related sq type.
    template< double SQ_REAL_V_MIN = REAL_V_MIN, double SQ_REAL_V_MAX = REAL_V_MAX, overflow OVF_BX_OVERRIDE = OVF_BX,
        base_t _SQ_V_MIN = v2s<base_t, F>(SQ_REAL_V_MIN), base_t _SQ_V_MAX = v2s<base_t, F>(SQ_REAL_V_MAX),
        // include overflow check if the value range of sq is smaller
        bool _OVF_CHECK_NEEDED = (V_MIN < _SQ_V_MIN || _SQ_V_MAX < V_MAX) >
    requires (
        RuntimeOverflowCheckAllowedWhenNeeded<OVF_BX_OVERRIDE, _OVF_CHECK_NEEDED>
    )
    constexpr
    sq<SQ_REAL_V_MIN, SQ_REAL_V_MAX>
    to_sq() const noexcept {
        using target_sq = sq<SQ_REAL_V_MIN, SQ_REAL_V_MAX>;
        base_t sqValue = value;

        // perform overflow check if needed
        if constexpr (_OVF_CHECK_NEEDED) {
            _i::check_overflow<OVF_BX_OVERRIDE, base_t>(sqValue, target_sq::V_MIN, target_sq::V_MAX);
        }

        return target_sq(sqValue);
    }

    /// Reveals the integer value stored in memory.
    base_t reveal() const noexcept {
        return value;
    }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning This conversion is expensive if the target type is a floating-point type.
    ///          If the target type is an integral type, there can be a significant loss of precision.
    ///          Use carefully!
#   if defined FPM_USE_SH
    template< typename TARGET_T = int >
#   else
    template< typename TARGET_T = double >
#   endif
    TARGET_T to_real() const noexcept {
        return v2s<TARGET_T, -F>(value);
    }

private:
    // delete default (runtime) constructor
    q() = delete;

    /// Explicit, possibly compile-time constructor from integer value.
    explicit constexpr q(base_t value) noexcept : value(value) {}

    /// scaled integer value that represents a fixed-point value; stored in memory
    base_t value;
};

/// Explicit (static-) cast to another q type with a different base type.
/// Allows to override the default overflow behavior.
template< class Q_C, overflow OVF_BX_OVERRIDE = Q_C::OVF_BX,
    typename _BASE_T, scaling_t _F, double _REAL_V_MIN, double _REAL_V_MAX, overflow _OVF_BX,
    typename _BASE_T_C = Q_C::base_t, scaling_t _F_C = Q_C::F, double _REAL_V_MIN_C = Q_C::REAL_V_MIN,
    double _REAL_V_MAX_C = Q_C::REAL_V_MAX, overflow _OVF_BX_C = Q_C::OVF_BX,
    // include overflow check if value range of cast-type is smaller than range of this type
    // or if different overflow properties could result in overflow if not checked
    // note: real limits are compared because scaled integers with different base types and q's
    //       cannot be compared so easily
    bool _OVF_CHECK_NEEDED = (_REAL_V_MIN < _REAL_V_MIN_C || _REAL_V_MAX_C < _REAL_V_MAX
                              || is_ovf_stricter_v<OVF_BX_OVERRIDE, _OVF_BX>
                              || is_ovf_stricter_v<_OVF_BX, OVF_BX_OVERRIDE>) >
requires (
    !std::is_same_v<_BASE_T, _BASE_T_C>
    && (CanBaseTypeOverflow<_BASE_T_C, OVF_BX_OVERRIDE> || ScalingIsPossible<_BASE_T, _F, _BASE_T_C, _F_C>)
    && RuntimeOverflowCheckAllowedWhenNeeded<OVF_BX_OVERRIDE, _OVF_CHECK_NEEDED>
)
constexpr
Q_C static_q_cast(q<_BASE_T, _F, _REAL_V_MIN, _REAL_V_MAX, _OVF_BX> from) noexcept {
    using from_q = q<_BASE_T, _F, _REAL_V_MIN, _REAL_V_MAX, _OVF_BX>;
    using target_q = Q_C;
    using interm_f_t = interm_t<_BASE_T>;
    using interm_c_t = interm_t<_BASE_T_C>;

    // static_cast can be reused if overflow is not overwritten
    if constexpr (OVF_BX_OVERRIDE == _OVF_BX_C) {
        return static_cast<target_q>(from);
    }
    // if overflow override is specified, compile overflow checks
    else {
        // scale source value and cast it to the intermediate type with the sign of the target type
        auto cValue = s2s<interm_c_t, from_q::F, target_q::F>(from.reveal());

        // perform overflow check if needed
        if constexpr (_OVF_CHECK_NEEDED) {
            _i::check_overflow<OVF_BX_OVERRIDE, interm_c_t, interm_f_t>(cValue, target_q::V_MIN, target_q::V_MAX);
        }

        // create target value; disable overflow check to avoid that value is checked again
        return target_q::template construct<overflow::NO_CHECK>( static_cast<target_q::base_t>(cValue) );
    }
}

/// Explicit, safe cast of a q type to another q type with a different base type.
/// This will always perform overflow checks, even if not necessarily needed.
/// \note overflow::ALLOWED is not possible - use static_cast instead.
template< class Q_C, overflow OVF_BX_OVERRIDE = Q_C::OVF_BX,
    typename _BASE_T, scaling_t _F, double _REAL_V_MIN, double _REAL_V_MAX, overflow _OVF_BX,
    typename _BASE_T_C = Q_C::base_t, scaling_t _F_C = Q_C::F >
requires (
    !std::is_same_v<_BASE_T, _BASE_T_C>
    && ScalingIsPossible<_BASE_T, _F, _BASE_T_C, _F_C>
    && OVF_BX_OVERRIDE != overflow::NO_CHECK
    && RuntimeOverflowCheckAllowedWhenNeeded<OVF_BX_OVERRIDE, true>
)
constexpr
Q_C safe_q_cast(q<_BASE_T, _F, _REAL_V_MIN, _REAL_V_MAX, _OVF_BX> from) noexcept {
    using from_q = q<_BASE_T, _F, _REAL_V_MIN, _REAL_V_MAX, _OVF_BX>;
    using target_q = Q_C;
    using interm_f_t = interm_t<_BASE_T>;
    using interm_c_t = interm_t<_BASE_T_C>;

    // scale source value and cast it to the intermediate type with the sign of the target type
    auto cValue = s2s<interm_c_t, from_q::F, target_q::F>(from.reveal());

    // always perform overflow checks
    _i::check_overflow<OVF_BX_OVERRIDE, interm_c_t, interm_f_t>(cValue, target_q::V_MIN, target_q::V_MAX);

    // finally, create target value; disable overflow check to avoid that value is checked again
    return target_q::template construct<overflow::NO_CHECK>( static_cast<target_q::base_t>(cValue) );
}

/// Explicit, force cast of a q type to a different q type.
/// This will simply reinterpret the value of the source q type as a value of the target q type
/// without performing any scaling or overflow checks at all (overflow attributes are all ignored).
template< class Q_C,
    typename _BASE_T, scaling_t _F, double _REAL_V_MIN, double _REAL_V_MAX, overflow _OVF_BX,
    typename _BASE_T_C = Q_C::base_t, scaling_t _F_C = Q_C::F >
constexpr
Q_C force_q_cast(q<_BASE_T, _F, _REAL_V_MIN, _REAL_V_MAX, _OVF_BX> from) noexcept {
    using target_q = Q_C;
    return target_q::template construct<overflow::NO_CHECK>( static_cast<target_q::base_t>(from.reveal()) );
}

}
/**\}*/

#endif
// EOF
