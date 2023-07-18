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
    typename BaseT,  ///< type of the scaled integer stored in memory
    scaling_t f_,      ///< number of fraction bits (precision 2^-f)
    double realVMin_ = v2s<double, -f_>(std::numeric_limits<BaseT>::min()),  ///< minimum real value represented by this type
    double realVMax_ = v2s<double, -f_>(std::numeric_limits<BaseT>::max()),  ///< maximum real value represented by this type
    Overflow ovfBx_ = Overflow::forbidden  ///< overflow behavior; overflow is forbidden by default
>
requires (
    ValidBaseType<BaseT>
    && ValidScaling<BaseT, f_>
    && RealLimitsInRangeOfBaseType<BaseT, f_, realVMin_, realVMax_>
)
class q final {
public:
    using base_t = BaseT;  /// integral base type
    static constexpr scaling_t f = f_;  ///< number of fraction bits
    static constexpr double realVMin = realVMin_;  ///< minimum real value
    static constexpr double realVMax = realVMax_;  ///< maximum real value
    static constexpr base_t vMin = v2s<base_t, f>(realVMin_);  ///< minimum value of integer value range
    static constexpr base_t vMax = v2s<base_t, f>(realVMax_);  ///< maximum value of integer value range
    static constexpr double resolution = v2s<double, -f>(1);  ///< real resolution of this type
    static constexpr Overflow ovfBx = ovfBx_;  ///< overflow behavior

    // friend all q types so that private members of similar types can be accessed for construction
    // Note: As of May 2023, partial specializations cannot be friended, so we friend q in general.
    template< typename BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
    requires (
        ValidBaseType<BaseTQ>
        && ValidScaling<BaseTQ, fQ>
        && RealLimitsInRangeOfBaseType<BaseTQ, fQ, realVMinQ, realVMaxQ>
    )
    friend class q;

    /// Corresponding sq type.
    template< double realVMinSq = realVMin, double realVMaxSq = realVMax >
    using sq = fpm::sq< base_t, f, realVMinSq, realVMaxSq >;

    /// Create a new q type with the same base type and scaling but a different real value range.
    template< double newRealVMin, double newRealVMax, Overflow ovfBxOverride = ovfBx >
    struct relimit { using type = q< base_t, f, newRealVMin, newRealVMax, ovfBxOverride >; };

    /// Type alias for relimit::type.
    template< double newRealVMin, double newRealVMax, Overflow ovfBxOverride = ovfBx >
    using relimit_t = relimit<newRealVMin, newRealVMax, ovfBxOverride>::type;

    /// Named "constructor" from a runtime variable (lvalue) or a constant (rvalue).
    /// \note Overflow check is always included unless explicitly disabled.
    template< Overflow ovfBxOverride = ovfBx >
    requires (
        RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride>
    )
    static constexpr
    q construct(base_t value) noexcept {
        // perform overflow check
        _i::checkOverflow<ovfBxOverride, base_t>(value, vMin, vMax);

        return q(value);
    }

    /// Helper struct template for q::fromReal<.>
    template< double realValue, Overflow ovfBxOverride = ovfBx >
    requires (
        RealValueScaledFitsBaseType<base_t, f, realValue>
        && CompileTimeOnlyOverflowCheckPossible<ovfBxOverride>
    )
    class WrapReal {
        static constexpr base_t scaledValue = v2s<base_t, f>(realValue);
        static constexpr Overflow overflowBehavior = (scaledValue >= vMin && scaledValue <= vMax)
            ? Overflow::noCheck  // value is within bounds; no overflow check needed
            : ovfBxOverride;
        public:
        static constexpr q wrapped = q::construct<overflowBehavior>( scaledValue );
    };

    /// Helper struct template for q::fromScaled<.>
    template< base_t value, Overflow ovfBxOverride = ovfBx >
    requires (
        CompileTimeOnlyOverflowCheckPossible<ovfBxOverride>
    )
    class WrapScaled {
        static constexpr Overflow overflowBehavior = (value >= vMin && value <= vMax)
            ? Overflow::noCheck  // value is within bounds; no overflow check needed
            : ovfBxOverride;
        public:
        static constexpr q wrapped = q::construct<overflowBehavior>( value );
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
    template< double realValue, Overflow ovfBxOverride = ovfBx >
    static constexpr
    q fromReal = WrapReal<realValue, ovfBxOverride>::wrapped;

    /// Alias for q::fromReal<., Overflow::forbidden>
    template< double realValue >
    static constexpr
    q fromRealNOvf = fromReal<realValue, Overflow::forbidden>;

    /// Alias for q::fromReal<., Overflow::saturate>
    template< double realValue >
    static constexpr
    q fromRealSat = fromReal<realValue, Overflow::saturate>;

    /// Alias for q::fromReal<., Overflow::allowed> and q::fromReal<., Overflow::noCheck>
    template< double realValue >
    static constexpr
    q fromRealOvf = fromReal<realValue, Overflow::allowed>;

    /// Named compile-time-only "constructor" from a scaled integer value. This can be used to
    /// construct a well-behaved q value at compile-time without a redundant overflow check.
    /// \note When the value is within bounds, no overflow check is needed. If it is out of range,
    ///       an overflow check is performed at compile-time according to the overflow settings.
    template< base_t value, Overflow ovfBxOverride = ovfBx >
    static constexpr
    q fromScaled = WrapScaled<value, ovfBxOverride>::wrapped;

    /// Alias for q::fromScaled<., Overflow::forbidden>
    template< base_t value >
    static constexpr
    q fromScaledNOvf = fromScaled<value, Overflow::forbidden>;

    /// Alias for q::fromScaled<., Overflow::saturate>
    template< base_t value >
    static constexpr
    q fromScaledSat = fromScaled<value, Overflow::saturate>;

    /// Alias for q::fromScaled<., Overflow::noCheck> and q::fromScaled<., Overflow::allowed>
    template< base_t value >
    static constexpr
    q fromScaledOvf = fromScaled<value, Overflow::noCheck>;

    /// Named "Copy-Constructor" from another q type with the same base type.
    /// \note When a q value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the q value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< Overflow ovfBxOverride = ovfBx,
        double realVMinFrom, double realVMaxFrom, scaling_t fFrom, Overflow ovfFrom,
        // include overflow check if value range of this type is smaller than range of from-type
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different q's cannot be compared so easily
        bool ovfCheckNeeded = (realVMinFrom < realVMin || realVMax < realVMaxFrom
                                  || is_ovf_stricter_v<ovfBxOverride, ovfFrom>
                                  || is_ovf_stricter_v<ovfBx, ovfBxOverride>) >
    requires (
        ScalingIsPossible<base_t, fFrom, base_t, f>
        && RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
    )
    static constexpr
    q fromQ(q<base_t, fFrom, realVMinFrom, realVMaxFrom, ovfFrom> const &from) noexcept {
        using fromQ = q<base_t, fFrom, realVMinFrom, realVMaxFrom, ovfFrom>;
        using interm_b_t = interm_t<base_t>;
        interm_b_t fromValueScaled = s2s<interm_b_t, fromQ::f, f>( from.reveal() );

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            _i::checkOverflow<ovfBxOverride, interm_b_t>(fromValueScaled, vMin, vMax);
        }

        return q(static_cast<base_t>(fromValueScaled));
    }

    /// Named "constructor" from a related sq variable.
    /// \note Overflow check is included if the range of the sq type is larger than the range of this q type.
    template< Overflow ovfBxOverride = ovfBx,
        double realVMinSq, double realVMaxSq,
        base_t vMinSq = v2s<base_t, f>(realVMinSq), base_t vMaxSq = v2s<base_t, f>(realVMaxSq),
        // include overflow check if the value range of q is smaller
        bool ovfCheckNeeded = (vMinSq < vMin || vMax < vMaxSq) >
    requires (
        RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
    )
    static constexpr
    q fromSq(fpm::sq<base_t, f, realVMinSq, realVMaxSq> const &fromSq) noexcept {
        base_t qValue = fromSq.value;

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            _i::checkOverflow<ovfBxOverride, base_t>(qValue, vMin, vMax);
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
    template< double realVMinRhs, double realVMaxRhs, scaling_t fRhs, Overflow ovfRhs >
    requires (
        fRhs != f
        && ScalingIsPossible<base_t, fRhs, base_t, f>
    )
    q& operator=(q<base_t, fRhs, realVMinRhs, realVMaxRhs, ovfRhs> const &rhs) noexcept {
        value = q::fromQ(rhs).value;
        return *this;
    }

    /// Explicit (static-) cast to a different q type with a different base type.
    /// \note If a cast does not work it's most probably due to unfulfilled requirements. Double check
    ///       whether a runtime overflow check is needed and make sure that it is allowed!
    template< typename BaseTC, scaling_t fC, double realVMinC, double realVMaxC, Overflow ovfC,
        // include overflow check if value range of cast-type is smaller than range of this type
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different base types and q's
        //       cannot be compared so easily
        bool ovfCheckNeeded = (realVMin < realVMinC || realVMaxC < realVMax
                                  || is_ovf_stricter_v<ovfC, ovfBx>) >
    requires (
        !std::is_same_v<base_t, BaseTC>
        && ScalingIsPossible<base_t, f, BaseTC, fC>
        && RuntimeOverflowCheckAllowedWhenNeeded<ovfC, ovfCheckNeeded>
    )
    explicit
    operator q<BaseTC, fC, realVMinC, realVMaxC, ovfC>() const noexcept {
        using target_q = q<BaseTC, fC, realVMinC, realVMaxC, ovfC>;
        using interm_b_t = interm_t<base_t>;
        using interm_c_t = interm_t<BaseTC>;

        // first cast, then scale value
        auto cValue = s2s<interm_c_t, f, target_q::f>(value);

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            _i::checkOverflow<target_q::ovfBx, interm_c_t, interm_b_t>(cValue, target_q::vMin, target_q::vMax);
        }

        // create target value; disable overflow check to avoid that value is checked again
        return target_q( static_cast<target_q::base_t>(cValue) );
    }

    /// Conversion to the related sq type.
    template< double realVMinSq = realVMin, double realVMaxSq = realVMax, Overflow ovfBxOverride = ovfBx,
        base_t vMinSq = v2s<base_t, f>(realVMinSq), base_t vMaxSq = v2s<base_t, f>(realVMaxSq),
        // include overflow check if the value range of sq is smaller
        bool ovfCheckNeeded = (vMin < vMinSq || vMaxSq < vMax) >
    requires (
        RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
    )
    constexpr
    sq<realVMinSq, realVMaxSq>
    toSq() const noexcept {
        using target_sq = sq<realVMinSq, realVMaxSq>;
        base_t sqValue = value;

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            _i::checkOverflow<ovfBxOverride, base_t>(sqValue, target_sq::vMin, target_sq::vMax);
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
    template< typename TargetT = int >
#   else
    template< typename TargetT = double >
#   endif
    TargetT toReal() const noexcept {
        return v2s<TargetT, -f>(value);
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
template< class QC, Overflow ovfBxOverride = QC::ovfBx,
    typename BaseT, scaling_t f, double realVMin, double realVMax, Overflow ovfBx,
    typename BaseTC = QC::base_t, scaling_t fC = QC::f, double realVMinC = QC::realVMin,
    double realVMaxC = QC::realVMax, Overflow ovfBxC = QC::ovfBx,
    // include overflow check if value range of cast-type is smaller than range of this type
    // or if different overflow properties could result in overflow if not checked
    // note: real limits are compared because scaled integers with different base types and q's
    //       cannot be compared so easily
    bool ovfCheckNeeded = (realVMin < realVMinC || realVMaxC < realVMax
                              || is_ovf_stricter_v<ovfBxOverride, ovfBx>
                              || is_ovf_stricter_v<ovfBx, ovfBxOverride>) >
requires (
    !std::is_same_v<BaseT, BaseTC>
    && (CanBaseTypeOverflow<BaseTC, ovfBxOverride> || ScalingIsPossible<BaseT, f, BaseTC, fC>)
    && RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
)
constexpr
QC static_q_cast(q<BaseT, f, realVMin, realVMax, ovfBx> from) noexcept {
    using fromQ = q<BaseT, f, realVMin, realVMax, ovfBx>;
    using target_q = QC;
    using interm_f_t = interm_t<BaseT>;
    using interm_c_t = interm_t<BaseTC>;

    // static_cast can be reused if overflow is not overwritten
    if constexpr (ovfBxOverride == ovfBxC) {
        return static_cast<target_q>(from);
    }
    // if overflow override is specified, compile overflow checks
    else {
        // scale source value and cast it to the intermediate type with the sign of the target type
        auto cValue = s2s<interm_c_t, fromQ::f, target_q::f>(from.reveal());

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            _i::checkOverflow<ovfBxOverride, interm_c_t, interm_f_t>(cValue, target_q::vMin, target_q::vMax);
        }

        // create target value; disable overflow check to avoid that value is checked again
        return target_q::template construct<Overflow::noCheck>( static_cast<target_q::base_t>(cValue) );
    }
}

/// Explicit, safe cast of a q type to another q type with a different base type.
/// This will always perform overflow checks, even if not necessarily needed.
/// \note Overflow::allowed is not possible - use static_cast instead.
template< class QC, Overflow ovfBxOverride = QC::ovfBx,
    typename BaseT, scaling_t f, double realVMin, double realVMax, Overflow ovfBx,
    typename BaseTC = QC::base_t, scaling_t fC = QC::f >
requires (
    !std::is_same_v<BaseT, BaseTC>
    && ScalingIsPossible<BaseT, f, BaseTC, fC>
    && ovfBxOverride != Overflow::noCheck
    && RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, true>
)
constexpr
QC safe_q_cast(q<BaseT, f, realVMin, realVMax, ovfBx> from) noexcept {
    using fromQ = q<BaseT, f, realVMin, realVMax, ovfBx>;
    using target_q = QC;
    using interm_f_t = interm_t<BaseT>;
    using interm_c_t = interm_t<BaseTC>;

    // scale source value and cast it to the intermediate type with the sign of the target type
    auto cValue = s2s<interm_c_t, fromQ::f, target_q::f>(from.reveal());

    // always perform overflow checks
    _i::checkOverflow<ovfBxOverride, interm_c_t, interm_f_t>(cValue, target_q::vMin, target_q::vMax);

    // finally, create target value; disable overflow check to avoid that value is checked again
    return target_q::template construct<Overflow::noCheck>( static_cast<target_q::base_t>(cValue) );
}

/// Explicit, force cast of a q type to a different q type.
/// This will simply reinterpret the value of the source q type as a value of the target q type
/// without performing any scaling or overflow checks at all (overflow attributes are all ignored).
template< class QC,
    typename BaseT, scaling_t f, double realVMin, double realVMax, Overflow ovfBx,
    typename BaseTC = QC::base_t, scaling_t fC = QC::f >
constexpr
QC force_q_cast(q<BaseT, f, realVMin, realVMax, ovfBx> from) noexcept {
    using target_q = QC;
    return target_q::template construct<Overflow::noCheck>( static_cast<target_q::base_t>(from.reveal()) );
}

/// Converts a literal integer into the corresponding best-fit q type.
/// Best-fit means that the literal integer represents both limits and the value.
template< typename BaseT, scaling_t f, char ...charArray >
consteval auto qFromLiteral() {
    constexpr std::size_t length = sizeof...(charArray);
    constexpr char chars[length]{ charArray... };
    static_assert(std::all_of(chars, chars + length, [](char c) { return isdigit(c); }), "The argument to _q must be a positive integer");
    constexpr double value = static_cast<double>( _i::charArrayTo<BaseT, length>(chars) );
    return q<BaseT, f, value, value, Ovf::forbidden>::template fromReal<value>;
}

}
/**\}*/

#endif
// EOF
