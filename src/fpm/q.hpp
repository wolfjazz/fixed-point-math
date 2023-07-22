/** \file
 * Implementation of the q class template.
 */

#ifndef FPM_FPM_Q_HPP_
#define FPM_FPM_Q_HPP_

#include "sq.hpp"


/** \addtogroup grp_fpm
 * \{ */
namespace fpm {

/// Concept of a q-like type.
/// \warning Does not guarantee that T is actually of type q. Only checks for the basic properties.
template< class T >
concept QType = requires (T t) {
    { std::bool_constant<T::isQType>() } -> std::same_as<std::true_type>;
    typename T::base_t;
    std::is_integral_v<typename T::base_t>;
    std::is_same_v<scaling_t, decltype(T::f)>;
    std::is_same_v<double, decltype(T::realVMin)>;
    std::is_same_v<double, decltype(T::realVMax)>;
    std::is_same_v<typename T::base_t, decltype(T::vMin)>;
    std::is_same_v<typename T::base_t, decltype(T::vMax)>;
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
/// \warning Two q types which differ only by the sign of the 0 in a limit are not equal by design,
/// although the underlying integral value will be the same when 0. This fact is not corrected for,
/// because the outcome might be different (wrong) when the compiler is doing its calculations.
/// It's recommended to use -0 when a type is declared that has only negative numbers in its range.
template<
    std::integral BaseT,  ///< type of the scaled integer stored in memory
    scaling_t f_,         ///< number of fraction bits (precision 2^-f)
    double realVMin_ = details::lowestRealVMin<BaseT, f_>,   ///< minimum real value represented by this type
    double realVMax_ = details::highestRealVMax<BaseT, f_>,  ///< maximum real value represented by this type
    Overflow ovfBx_ = Overflow::forbidden  ///< overflow behavior; overflow is forbidden by default
>
requires (
    details::ValidBaseType<BaseT>
    && details::ValidScaling<BaseT, f_>
    && details::RealLimitsInRangeOfBaseType<BaseT, f_, realVMin_, realVMax_>
)
class q final {
public:
    static constexpr bool isQType = true;  ///< identifier for the QType concept
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
    template< std::integral BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
    requires (
        details::ValidBaseType<BaseTQ>
        && details::ValidScaling<BaseTQ, fQ>
        && details::RealLimitsInRangeOfBaseType<BaseTQ, fQ, realVMinQ, realVMaxQ>
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
        details::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride>
    )
    static constexpr
    q construct(base_t value) noexcept {
        // perform overflow check
        details::checkOverflow<ovfBxOverride, base_t>(value, vMin, vMax);

        return q(value);
    }

    /// Named compile-time-only "constructor" from a floating-point value. This will use v2s to scale
    /// the given floating-point value at compile-time before the q value is constructed with the
    /// scaled integer value.
    /// \note When the value is within bounds, no overflow check is needed. If it is out of range,
    ///       an overflow check is performed at compile-time according to the overflow settings.
    /// \note: When a real value is wrapped into a q value, there is an inherent rounding error due
    /// to the limited resolution. This error is called 'representation error' and it refers to the
    /// deviation from the initial real value when the q value is unscaled to a real value again.
    /// Usually the scaling error is in the order of the resolution of the q type.
    template< double realValue, Overflow ovfBxOverride = ovfBx,
        base_t scaledValue = v2s<base_t, f>(realValue),
        Overflow ovfBxEffective = (vMin <= scaledValue && scaledValue <= vMax) ? Overflow::noCheck : ovfBxOverride >
    requires (
        details::RealValueScaledFitsBaseType<base_t, f, realValue>
        && details::CompileTimeOnlyOverflowCheckPossible<ovfBxOverride>
        && details::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxEffective>
    )
    static constexpr
    q fromReal = q::construct<ovfBxEffective>( scaledValue );

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
    template< base_t value, Overflow ovfBxOverride = ovfBx,
        Overflow ovfBxEffective = (vMin <= value && value <= vMax) ? Overflow::noCheck : ovfBxOverride >
    requires (
       details::CompileTimeOnlyOverflowCheckPossible<ovfBxOverride>
        && details::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxEffective>
    )
    static constexpr
    q fromScaled = q::construct<ovfBxEffective>( value );

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
        && details::ScalingIsPossible<base_t, QFrom::f, base_t, f>
        && details::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
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
        && details::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
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
    /// Similar to q::fromQ(), however a bit stricter (q types have to be different) and there
    /// is no possibility to override the overflow behavior. If this is desired, use the named
    /// constructor q::fromQ() instead.
    /// \note When a q value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the q value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< QType QFrom,
        // include overflow check if value range of this type is smaller than range of from-type
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different q's cannot be compared so easily
        bool ovfCheckNeeded = (QFrom::realVMin < realVMin || realVMax < QFrom::realVMax
                               || is_ovf_stricter_v<ovfBx, QFrom::ovfBx>) >
    requires (
        !std::is_same_v< q, QFrom >  // when the same, default copy constructor should be used
        && std::is_same_v<base_t, typename QFrom::base_t>
        && details::ScalingIsPossible<base_t, QFrom::f, base_t, f>
        && details::RuntimeOverflowCheckAllowedWhenNeeded<ovfBx, ovfCheckNeeded>
    )
    constexpr q(QFrom const &from) noexcept : value( q::fromQ<ovfBx>(from).value ) {}

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
        && details::ScalingIsPossible<base_t, QFrom::f, base_t, f>
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
    template< std::integral BaseTC, scaling_t fC, double realVMinC, double realVMaxC, Overflow ovfC,
        // include overflow check if value range of cast-type is smaller than range of this type
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different base types and q's
        //       cannot be compared so easily
        bool ovfCheckNeeded = (realVMin < realVMinC || realVMaxC < realVMax || is_ovf_stricter_v<ovfC, ovfBx>) >
    requires (
        !std::is_same_v<base_t, BaseTC>
        && details::ScalingIsPossible<base_t, f, BaseTC, fC>
        && details::RuntimeOverflowCheckAllowedWhenNeeded<ovfC, ovfCheckNeeded>
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
        details::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
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
    && (details::CanBaseTypeOverflow<typename QC::base_t, ovfBxOverride>
        || details::ScalingIsPossible<typename QFrom::base_t, QFrom::f, typename QC::base_t, QC::f>)
    && details::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
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
    && details::ScalingIsPossible<typename QFrom::base_t, QFrom::f, typename QC::base_t, QC::f>
    && ovfBxOverride != Overflow::noCheck
    && details::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, true>
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
template< std::integral BaseT, scaling_t f, char ...charArray >
consteval auto qFromLiteral() {
    constexpr std::size_t length = sizeof...(charArray);
    constexpr char chars[length]{ charArray... };
    static_assert(std::all_of(chars, chars + length, [](char c) { return isdigit(c); }), "The argument to _q must be a positive integer");
    constexpr double value = static_cast<double>( details::charArrayTo<BaseT, length>(chars) );
    return q<BaseT, f, value, value, Ovf::forbidden>::template fromReal<value>;
}

/// Concept which checks whether a value with the given Q type can be constructed from the given real value.
template< class Q, double realValue, Overflow ovfBxOverride = Q::ovfBx >
concept CanConstructQFromReal = requires {
    { Q::template fromReal<realValue, ovfBxOverride> } -> std::same_as<Q const &>;  // false if expression cannot be compiled
};

/// Concept which checks whether a value with the given Q type can be constructed from the given scaled value.
template< class Q, Q::base_t scaledValue, Overflow ovfBxOverride = Q::ovfBx >
concept CanConstructQFromScaled = requires {
    { Q::template fromScaled<scaledValue, ovfBxOverride> } -> std::same_as<Q const &>;  // false if expression cannot be compiled
};

/// Concept which checks whether an instance of a given Q type can be converted to an instance of a
// given Sq type.
template< class Q, class Sq, Overflow ovfBxOverride = Q::ovfBx >
concept CanConvertQToSq = requires (Q q) {
    { q.template toSq<Sq::realVMin, Sq::realVMax, ovfBxOverride>() } -> std::same_as<Sq>;  // false if expression cannot be compiled
};

/// Concept which checks whether an instance of a given Sq type can be converted to an instance of a
/// given Q type.
template< class Sq, class Q >
concept CanConvertSqToQ = requires (Sq sq) {
    { Q::template fromSq(sq) } -> std::same_as<Q>;  // false if expression cannot be compiled
};

/// Concept which checks whether an instance of a given Q type can be static_cast to an instance of
/// another given Q type.
/// \note If this returns false, try static_q_cast with an overflow override option.
template< class QSrc, class QTarget >
concept CanCastQToQ = requires (QSrc qSrc) {
    { static_cast<QTarget>(qSrc) } -> std::same_as<QTarget>;  // false if expression cannot be compiled
};

}  // end of fpm
/**\}*/

namespace std {

/// Provides the bare, real numeric limits for the given q type.
template< std::integral BaseT, fpm::scaling_t f, double realVMin, double realVMax, fpm::Overflow ovfBx >
class numeric_limits<fpm::q<BaseT, f, realVMin, realVMax, ovfBx>> {
    using QType = fpm::q<BaseT, f, realVMin, realVMax, ovfBx>;
public:
    /// \returns the minimum real value that can be represented by the q type.
    /// \note In contrast to q::realVMin, this does not return the minimum value specified by the
    /// user, but the absolute minimum that can be represented by the underlying q type with respect
    /// to its base type and scaling. This can be significantly smaller that the actual user minimum.
    template< typename T = double >
    static constexpr T min() noexcept {
        return fpm::v2s<T, -QType::f>( numeric_limits<typename QType::base_t>::min() );
    }

    /// \returns the maximum real value that can be represented by the q type.
    /// \note In contrast to q::realVMax, this does not return the maximum value specified by the
    /// user, but the absolute maximum that can be represented by the underlying q type with respect
    /// to its base type and scaling. This can be significantly larger that the actual user maximum.
    template< typename T = double >
    static constexpr T max() noexcept {
        return fpm::v2s<T, -QType::f>( numeric_limits<typename QType::base_t>::max() );
    }

    constexpr static bool is_specialized = true;
    constexpr static bool is_signed = numeric_limits<typename QType::base_t>::is_signed;
    constexpr static bool is_bounded = true;
    constexpr static bool traps = true;
#   if defined FPM_USE_SH
    constexpr static auto round_style = std::round_toward_neg_infinity;
#   else
    constexpr static auto round_style = std::round_toward_zero;
#   endif
    constexpr static int radix = numeric_limits<typename QType::base_t>::radix;
    constexpr static int digits = numeric_limits<typename QType::base_t>::digits - QType::f;
    constexpr static int digits10 = static_cast<int>( std::log10(radix) * digits );
};

}

#endif
// EOF
