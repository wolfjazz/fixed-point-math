/** \file
 * Implementation of the q class template.
 */

#ifndef _FPM___Q_HPP_
#define _FPM___Q_HPP_

#include "sq.hpp"


/** \addtogroup grp_fpm
 * \{ */
namespace fpm {

/// Concept of a q-like type.
/// \warning Does not guarantee that T is actually of type q. Only checks for the basic properties.
template< class T >
concept QType = requires (T t) {
    typename T::base_t;
    std::is_integral_v<typename T::base_t>;
    std::is_same_v<scaling_t, decltype(T::f)>;
    std::is_same_v<double, decltype(T::realVMin)>;
    std::is_same_v<double, decltype(T::realVMax)>;
    std::is_same_v<double, decltype(T::resolution)>;
    std::is_same_v<Overflow, decltype(T::ovfBx)>;
    { t.reveal() } -> std::same_as<typename T::base_t>;
};

/// Normal q type. This type can be used to construct a new q variable of any specialization.
/// Depending on the configured overflow behavior, a q type provides overflow protection with the
/// least possible amount of runtime checks against overflow.
/// It implements conversion operators and -functions to/from similar types and to/from static sq types.
/// \note The q type itself does not have arithmetic operators, as runtime overflow checks would be
/// required to guarantee overflow safety. Arithmetics are implemented by the static sq type only.
/// \note A q variable stores runtime values which may change, whereas an sq variable stores a temporary,
/// constant value used in a single mathematical operation that results in a new sq type.
template<
    typename BaseT,    ///< type of the scaled integer stored in memory
    scaling_t f_,      ///< number of fraction bits (precision 2^-f)
    double realVMin_,  ///< minimum real value represented by this type
    double realVMax_,  ///< maximum real value represented by this type
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

    /// Corresponding (related) sq type.
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
        details::checkOverflow<ovfBxOverride, base_t>(value, vMin, vMax);

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
        QType QFrom,
        // include overflow check if value range of this type is smaller than range of from-type
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different q's cannot be compared so easily
        bool ovfCheckNeeded = (QFrom::realVMin < realVMin || realVMax < QFrom::realVMax
                               || is_ovf_stricter_v<ovfBxOverride, QFrom::ovfBx>
                               || is_ovf_stricter_v<ovfBx, ovfBxOverride>) >
    requires (
        std::is_same_v<base_t, typename QFrom::base_t>
        && ScalingIsPossible<base_t, QFrom::f, base_t, f>
        && RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
    )
    static constexpr
    q fromQ(QFrom const &from) noexcept {
        using interm_b_t = interm_t<base_t>;
        interm_b_t fromValueScaled = s2s<interm_b_t, QFrom::f, f>( from.reveal() );

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            details::checkOverflow<ovfBxOverride, interm_b_t>(fromValueScaled, vMin, vMax);
        }

        return q(static_cast<base_t>(fromValueScaled));
    }

    /// Named "constructor" from a related sq variable.
    /// \note Overflow check is included if the range of the sq type is larger than the range of this q type.
    template< Overflow ovfBxOverride = ovfBx,
        SqType SqFrom,
        // include overflow check if the value range of q is smaller
        bool ovfCheckNeeded = (SqFrom::vMin < vMin || vMax < SqFrom::vMax) >
    requires (
        std::is_same_v<base_t, typename SqFrom::base_t>
        && f == SqFrom::f
        && RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
    )
    static constexpr
    q fromSq(SqFrom const &fromSq) noexcept {
        base_t qValue = fromSq.value;

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            details::checkOverflow<ovfBxOverride, base_t>(qValue, vMin, vMax);
        }

        return q(qValue);
    }

    /// Copy-Constructor from another q type with the same base type.
    /// \note Similar to q::fromQ(), however a bit stricter (q types have to be different) and there
    /// is no possibility to override the overflow behavior. If this is desired, use the named
    /// constructor q::fromQ() instead.
    template< QType QFrom,
        // include overflow check if value range of this type is smaller than range of from-type
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different q's cannot be compared so easily
        bool ovfCheckNeeded = (QFrom::realVMin < realVMin || realVMax < QFrom::realVMax
                               || is_ovf_stricter_v<ovfBx, QFrom::ovfBx>) >
    requires (
        !std::is_same_v< q, QFrom >  // when the same, default copy constructor should be used
        && std::is_same_v<base_t, typename QFrom::base_t>
        && ScalingIsPossible<base_t, QFrom::f, base_t, f>
        && RuntimeOverflowCheckAllowedWhenNeeded<ovfBx, ovfCheckNeeded>
    )
    constexpr
    q(QFrom const &from) noexcept : value( q::fromQ<ovfBx>(from).value ) {}

    /// Copy-Constructor from the same type.
    constexpr q(q const &) noexcept = default;

    /// Copy-Assignment from the same type.
    constexpr q& operator=(q const &) noexcept = default;

    /// Move-Constructor from the same type.
    constexpr q(q&&) noexcept = default;

    /// Move-Assignment from the same type.
    constexpr q& operator=(q&&) noexcept = default;

    /// Destructor.
    constexpr ~q() {}

    /// Copy-Assignment from a different q type with the same base type.
    /// \note The rhs type must have a range that is fully inside the range of the target type.
    /// If this is not the case, use the static fromQ constructor function.
    template< QType QFrom >
    requires (
        !std::is_same_v< q, QFrom >
        && ScalingIsPossible<base_t, QFrom::f, base_t, f>
        && realVMin <= QFrom::realVMin && QFrom::realVMax <= realVMax
    )
    constexpr
    q& operator=(QFrom const &from) noexcept {
        value = q::fromQ(from).value;
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
        bool ovfCheckNeeded = (realVMin < realVMinC || realVMaxC < realVMax || is_ovf_stricter_v<ovfC, ovfBx>) >
    requires (
        !std::is_same_v<base_t, BaseTC>
        && ScalingIsPossible<base_t, f, BaseTC, fC>
        && RuntimeOverflowCheckAllowedWhenNeeded<ovfC, ovfCheckNeeded>
    )
    explicit constexpr
    operator q<BaseTC, fC, realVMinC, realVMaxC, ovfC>() const noexcept {
        using QC = q<BaseTC, fC, realVMinC, realVMaxC, ovfC>;
        using interm_b_t = interm_t<base_t>;
        using interm_c_t = interm_t<BaseTC>;

        // first cast, then scale value
        auto cValue = s2s<interm_c_t, f, QC::f>(value);

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            details::checkOverflow<QC::ovfBx, interm_c_t, interm_b_t>(cValue, QC::vMin, QC::vMax);
        }

        // create target value; disable overflow check to avoid that value is checked again
        return QC( static_cast<QC::base_t>(cValue) );
    }

    /// Conversion to the related sq type.
    template< double realVMinSq = realVMin, double realVMaxSq = realVMax, Overflow ovfBxOverride = ovfBx,
        class SqTo = sq<realVMinSq, realVMaxSq>,
        // include overflow check if the value range of sq is smaller
        bool ovfCheckNeeded = (vMin < SqTo::vMin || SqTo::vMax < vMax) >
    requires (
        RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
    )
    constexpr
    SqTo toSq() const noexcept {
        base_t sqValue = value;

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            details::checkOverflow<ovfBxOverride, base_t>(sqValue, SqTo::vMin, SqTo::vMax);
        }

        return SqTo(sqValue);
    }

    /// Reveals the integer value stored in memory.
    constexpr base_t reveal() const noexcept {
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
    constexpr TargetT toReal() const noexcept {
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
template< QType QC, Overflow ovfBxOverride = QC::ovfBx,
    QType QFrom,
    // include overflow check if value range of cast-type is smaller than range of this type
    // or if different overflow properties could result in overflow if not checked
    // note: real limits are compared because scaled integers with different base types and q's
    //       cannot be compared so easily
    bool ovfCheckNeeded = (QFrom::realVMin < QC::realVMin || QC::realVMax < QFrom::realVMax
                           || is_ovf_stricter_v<ovfBxOverride, QFrom::ovfBx>
                           || is_ovf_stricter_v<QC::ovfBx, ovfBxOverride>) >
requires (
    !std::is_same_v<typename QFrom::base_t, typename QC::base_t>
    && (CanBaseTypeOverflow<typename QC::base_t, ovfBxOverride>
        || ScalingIsPossible<typename QFrom::base_t, QFrom::f, typename QC::base_t, QC::f>)
    && RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
)
constexpr
QC static_q_cast(QFrom from) noexcept {
    using interm_f_t = interm_t<typename QFrom::base_t>;
    using interm_c_t = interm_t<typename QC::base_t>;

    // static_cast can be reused if overflow is not overwritten
    if constexpr (ovfBxOverride == QC::ovfBx) {
        return static_cast<QC>(from);
    }
    // if overflow override is specified, compile overflow checks
    else {
        // scale source value and cast it to the intermediate type with the sign of the target type
        auto cValue = s2s<interm_c_t, QFrom::f, QC::f>(from.reveal());

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            details::checkOverflow<ovfBxOverride, interm_c_t, interm_f_t>(cValue, QC::vMin, QC::vMax);
        }

        // create target value; disable overflow check to avoid that value is checked again
        return QC::template construct<Overflow::noCheck>( static_cast<QC::base_t>(cValue) );
    }
}

/// Explicit, safe cast of a q type to another q type with a different base type.
/// This will always perform overflow checks, even if not necessarily needed.
/// \note Overflow::allowed is not possible - use static_cast instead.
template< QType QC, Overflow ovfBxOverride = QC::ovfBx,
    QType QFrom >
requires (
    !std::is_same_v<typename QFrom::base_t, typename QC::base_t>
    && ScalingIsPossible<typename QFrom::base_t, QFrom::f, typename QC::base_t, QC::f>
    && ovfBxOverride != Overflow::noCheck
    && RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, true>
)
constexpr
QC safe_q_cast(QFrom from) noexcept {
    using interm_f_t = interm_t<typename QFrom::base_t>;
    using interm_c_t = interm_t<typename QC::base_t>;

    // scale source value and cast it to the intermediate type with the sign of the target type
    auto cValue = s2s<interm_c_t, QFrom::f, QC::f>(from.reveal());

    // always perform overflow checks
    details::checkOverflow<ovfBxOverride, interm_c_t, interm_f_t>(cValue, QC::vMin, QC::vMax);

    // finally, create target value; disable overflow check to avoid that value is checked again
    return QC::template construct<Overflow::noCheck>( static_cast<QC::base_t>(cValue) );
}

/// Explicit, force cast of a q type to a different q type.
/// This will simply reinterpret the value of the source q type as a value of the target q type
/// without performing any scaling or overflow checks at all (overflow attributes are all ignored).
template< QType QC,
    QType QFrom >
constexpr
QC force_q_cast(QFrom from) noexcept {
    return QC::template construct<Overflow::noCheck>( static_cast<QC::base_t>(from.reveal()) );
}

/// Converts a literal integer into the corresponding best-fit q type.
/// Best-fit means that the literal integer represents both limits and the value.
template< typename BaseT, scaling_t f, char ...charArray >
consteval auto qFromLiteral() {
    constexpr std::size_t length = sizeof...(charArray);
    constexpr char chars[length]{ charArray... };
    static_assert(std::all_of(chars, chars + length, [](char c) { return isdigit(c); }), "The argument to _q must be a positive integer");
    constexpr double value = static_cast<double>( details::charArrayTo<BaseT, length>(chars) );
    return q<BaseT, f, value, value, Ovf::forbidden>::template fromReal<value>;
}

}
/**\}*/

#endif
// EOF
