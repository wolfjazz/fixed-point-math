/** \file
 * Implementation of the Q class template.
 */

#ifndef FPM_FPM_Q_HPP_
#define FPM_FPM_Q_HPP_

#include "sq.hpp"


namespace fpm::q {
/** \ingroup grp_fpm
 * \defgroup grp_fpmQ Q-Type
 * \{ */

using detail::QType;
using detail::SqType;

/// Normal Q type. This type can be used to construct a new Q variable of any specialization.
/// Depending on the configured overflow behavior, a Q type provides overflow protection with the
/// least possible amount of runtime checks against overflow.
/// It implements conversion operators and -functions to/from similar types and to/from static Sq types.
/// \note The Q type itself does not have arithmetic operators, as runtime overflow checks would be
/// required to guarantee overflow safety. Arithmetics are implemented by the static Sq type only.
/// \note A Q variable stores runtime values which may change, whereas an Sq variable stores a temporary,
/// constant value used in a single mathematical operation that results in a new Sq type.
/// \warning Two Q types which differ only by the sign of the 0 in a limit are not equal by design,
///          although the underlying integral value will be the same when 0. This fact is not corrected
///          for, because the outcome might be different (wrong) when the compiler is doing its
///          calculations. It's highly recommended to use -0 when a type is declared that has only
///          negative numbers in its range.
template<
    std::integral BaseT,  ///< type of the scaled integer stored in memory
    scaling_t f_,         ///< number of fraction bits (precision 2^(-f))
    double realVMin_ = detail::lowestRealVMin<BaseT, f_>,   ///< minimum real value represented by the type
    double realVMax_ = detail::highestRealVMax<BaseT, f_>,  ///< maximum real value represented by the type
    Overflow ovfBx_ = Overflow::forbidden >  ///< overflow behavior; overflow is forbidden by default
requires detail::QRequirements<BaseT, f_, realVMin_, realVMax_, ovfBx_>
class Q final {
public:
    static constexpr bool isQType = true;  ///< identifier for the QType concept
    using base_t = BaseT;  /// integral base type
    static constexpr scaling_t f = f_;  ///< number of fraction bits
    static constexpr double realVMin = realVMin_;  ///< minimum real value
    static constexpr double realVMax = realVMax_;  ///< maximum real value
    static constexpr base_t vMin = v2s<base_t, f>(realVMin_);  ///< minimum value of integer value range
    static constexpr base_t vMax = v2s<base_t, f>(realVMax_);  ///< maximum value of integer value range
    static constexpr double resolution = v2s<double, -f>(1);  ///< real resolution
    static constexpr Overflow ovfBx = ovfBx_;  ///< overflow behavior

    /// Corresponding (related) Sq type.
    template< double realVMinSq = realVMin, double realVMaxSq = realVMax >
    using Sq = sq::Sq< base_t, f, realVMinSq, realVMaxSq >;

    /// Create a new Q type with the same base type and scaling but a different real value range
    /// and overflow behavior. The default overflow behavior is Ovf::clamp, or the behavior of the
    /// source type if it is stricter.
    template< double newRealVMin, double newRealVMax,
        Overflow ovfBxOverride = is_ovf_stricter_v<ovfBx, Ovf::clamp> ? ovfBx : Ovf::clamp >
    struct clamp { using type = Q< base_t, f, newRealVMin, newRealVMax, ovfBxOverride >; };

    /// Type alias for clamp::type.
    template< double newRealVMin, double newRealVMax, Overflow ovfBxOverride = ovfBx >
    using clamp_t = clamp<newRealVMin, newRealVMax, ovfBxOverride>::type;

    /// Named "constructor" from a runtime variable (lvalue) or a constant (rvalue).
    /// \note Overflow check is always included unless explicitly disabled.
    template< Overflow ovfBxOverride = ovfBx >
    requires detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride>
    static constexpr
    Q construct(base_t value) noexcept {
        // perform overflow check
        detail::checkOverflow<ovfBxOverride, base_t>(value, vMin, vMax);

        return Q(value);
    }

    /// Named compile-time-only "constructor" from a floating-point value. This will use v2s to scale
    /// the given floating-point value at compile-time before the Q value is constructed with the
    /// scaled integer value.
    /// \note When the value is within bounds, no overflow check is needed. If it is out of range,
    ///       an overflow check is performed at compile-time according to the overflow settings.
    /// \note: When a real value is wrapped into a Q value, there is an inherent rounding error due
    /// to the limited resolution. This error is called 'representation error' and it refers to the
    /// deviation from the initial real value when the Q value is unscaled to a real value again.
    /// Usually the scaling error is in the order of the resolution of the Q type.
    template< double realValue, Overflow ovfBxOverride = ovfBx,
        /* deduced: */
        base_t scaledValue = v2s<base_t, f>(realValue),
        Overflow ovfBxEffective = (vMin <= scaledValue && scaledValue <= vMax) ? Overflow::noCheck : ovfBxOverride >
    requires ( detail::RealValueScaledFitsBaseType<base_t, f, realValue>
               && Overflow::assert != ovfBxOverride
               && detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxEffective> )
    static constexpr
    Q fromReal = Q::construct<ovfBxEffective>( scaledValue );

    /// Alias for Q::fromReal<., Overflow::forbidden>
    template< double realValue >
    static constexpr
    Q fromRealNOvf = fromReal<realValue, Overflow::forbidden>;

    /// Alias for Q::fromReal<., Overflow::clamp>
    template< double realValue >
    static constexpr
    Q fromRealClamp = fromReal<realValue, Overflow::clamp>;

    /// Alias for Q::fromReal<., Overflow::allowed> and Q::fromReal<., Overflow::noCheck>
    template< double realValue >
    static constexpr
    Q fromRealOvf = fromReal<realValue, Overflow::allowed>;

    /// Named compile-time-only "constructor" from a scaled integer value. This can be used to
    /// construct a well-behaved Q value at compile-time without a redundant overflow check.
    /// \note When the value is within bounds, no overflow check is needed. If it is out of range,
    ///       an overflow check is performed at compile-time according to the overflow settings.
    template< base_t value, Overflow ovfBxOverride = ovfBx,
        /* deduced: */
        Overflow ovfBxEffective = (vMin <= value && value <= vMax) ? Overflow::noCheck : ovfBxOverride >
    requires ( ovfBxOverride != Overflow::assert
               && detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxEffective> )
    static constexpr
    Q fromScaled = Q::construct<ovfBxEffective>( value );

    /// Alias for Q::fromScaled<., Overflow::forbidden>
    template< base_t value >
    static constexpr
    Q fromScaledNOvf = fromScaled<value, Overflow::forbidden>;

    /// Alias for Q::fromScaled<., Overflow::clamp>
    template< base_t value >
    static constexpr
    Q fromScaledClamp = fromScaled<value, Overflow::clamp>;

    /// Alias for Q::fromScaled<., Overflow::noCheck> and Q::fromScaled<., Overflow::allowed>
    template< base_t value >
    static constexpr
    Q fromScaledOvf = fromScaled<value, Overflow::noCheck>;

    /// Named "Copy-Constructor" from another Q type with the same base type.
    /// \note When a Q value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the Q value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< Overflow ovfBxOverride = ovfBx,
        /* deduced: */
        QType QFrom,
        // include overflow check if value range of from-type is not fully within range of target type,
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different Q's cannot be compared so easily
        bool ovfCheckNeeded = (QFrom::realVMin < realVMin || realVMax < QFrom::realVMax
                               || is_ovf_stricter_v<ovfBxOverride, QFrom::ovfBx>
                               || is_ovf_stricter_v<ovfBx, ovfBxOverride>) >
    requires ( std::is_same_v<base_t, typename QFrom::base_t>
               && detail::Scalable<base_t, QFrom::f, base_t, f>
               && detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded> )
    static constexpr
    Q fromQ(QFrom const &from) noexcept {
        using interm_b_t = interm_t<base_t>;
        interm_b_t fromValueScaled = s2s<interm_b_t, QFrom::f, f>( from.reveal() );

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            detail::checkOverflow<ovfBxOverride, interm_b_t>(fromValueScaled, vMin, vMax);
        }

        return Q(static_cast<base_t>(fromValueScaled));
    }

    /// Named "constructor" from a Sq variable with the same base type.
    /// \note The representation error to be expected is comparable to that of Q::fromQ.
    /// \note An overflow check is included if the range of the Sq type is not entirely within the
    /// range of the Q type.
    template< Overflow ovfBxOverride = ovfBx,
        /* deduced: */
        SqType SqFrom,
        // include overflow check if value range of source is not fully within range of target
        bool ovfCheckNeeded = (SqFrom::realVMin < realVMin || realVMax < SqFrom::realVMax) >
    requires ( std::is_same_v<base_t, typename SqFrom::base_t>
               && detail::Scalable<base_t, SqFrom::f, base_t, f>
               && detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded> )
    static constexpr
    Q fromSq(SqFrom const &fromSq) noexcept {
        base_t qValue = s2s<base_t, SqFrom::f, f>(fromSq.value);

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            detail::checkOverflow<ovfBxOverride, base_t>(qValue, vMin, vMax);
        }

        return Q(qValue);
    }

    /// Alias for Q::fromSq<Overflow::forbidden>(.)
    template< /* deduced: */ SqType SqFrom >
    static constexpr
    Q fromSqNOvf(SqFrom const &fromSq) noexcept { return Q::template fromSq<Overflow::forbidden>(fromSq); }

    /// Alias for Q::fromSq<Overflow::clamp>(.)
    template< /* deduced: */ SqType SqFrom >
    static constexpr
    Q fromSqClamp(SqFrom const &fromSq) noexcept { return Q::template fromSq<Overflow::clamp>(fromSq); }

    /// Alias for Q::fromSq<Overflow::noCheck>(.) and Q::fromSq<Overflow::allowed>(.)
    template< /* deduced: */ SqType SqFrom >
    static constexpr
    Q fromSqOvf(SqFrom const &fromSq) noexcept { return Q::template fromSq<Overflow::noCheck>(fromSq); }

    /// Copy-Constructor from another Q type with the same base type.
    /// Similar to Q::fromQ(), however a bit stricter (Q types have to be different) and there
    /// is no possibility to override the overflow behavior. If this is desired, use the named
    /// constructor Q::fromQ() instead.
    /// \note When a Q value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the Q value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< /* deduced: */ QType QFrom,
        // include overflow check if value range of from-type is not fully within range of target type,
        // or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different Q's cannot be compared so easily
        bool ovfCheckNeeded = (QFrom::realVMin < realVMin || realVMax < QFrom::realVMax
                               || is_ovf_stricter_v<ovfBx, QFrom::ovfBx>) >
    requires ( !std::is_same_v< Q, QFrom >  // when the same, default copy constructor should be used
               && std::is_same_v<base_t, typename QFrom::base_t>
               && detail::Scalable<base_t, QFrom::f, base_t, f>
               && detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBx, ovfCheckNeeded> )
    constexpr Q(QFrom const &from) noexcept : value( Q::fromQ<ovfBx>(from).value ) {}

    /// Implicit copy constructor from a Sq type with the same or a narrower range.
    /// \note This works as long as no overflow check is necessary.
    /// \note The representation error to be expected is comparable to that of Q::fromSq.
    template< /* deduced: */ SqType SqFrom >
    requires detail::ImplicitlyConvertible<SqFrom, Q>
    constexpr Q(SqFrom const &from) noexcept : value( s2s<base_t, SqFrom::f, f>(from.value) ) {}

    /// Copy-Constructor from the same type.
    constexpr Q(Q const &) noexcept = default;

    /// Copy-Assignment from the same type.
    constexpr Q& operator =(Q const &) noexcept = default;

    /// Move-Constructor from the same type.
    constexpr Q(Q&&) noexcept = default;

    /// Move-Assignment from the same type.
    constexpr Q& operator =(Q&&) noexcept = default;

    /// Destructor.
    constexpr ~Q() {}

    /// Copy-Assignment from a different Q type with the same base type.
    /// \note The rhs type must have a range that is fully within the range of the target type.
    /// If this is not the case, use the static fromQ constructor function.
    template< /* deduced: */ QType QFrom >
    requires ( !std::is_same_v< Q, QFrom >
               && detail::Scalable<base_t, QFrom::f, base_t, f>
               && realVMin <= QFrom::realVMin && QFrom::realVMax <= realVMax )
    constexpr
    Q& operator =(QFrom const &from) noexcept {
        value = Q::fromQ(from).value;
        return *this;
    }

    /// Explicit (static-) cast to a different Q type with a potentially different base type.
    /// \note If a cast does not work it's most probably due to unfulfilled requirements. Double check
    ///       whether a runtime overflow check is needed and make sure that it is allowed!
    template< /* deduced: */ std::integral BaseTC, scaling_t fC, double realVMinC, double realVMaxC, Overflow ovfC,
        // include overflow check if value range of source type is not fully within range of target
        // type, or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different base types and Q's
        //       cannot be compared so easily
        bool ovfCheckNeeded = (realVMin < realVMinC || realVMaxC < realVMax || is_ovf_stricter_v<ovfC, ovfBx>) >
    requires ( detail::Scalable<base_t, f, BaseTC, fC>
               && detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfC, ovfCheckNeeded> )
    explicit constexpr
    operator Q<BaseTC, fC, realVMinC, realVMaxC, ovfC>() const noexcept {
        using QC = Q<BaseTC, fC, realVMinC, realVMaxC, ovfC>;
        using interm_b_t = interm_t<base_t>;
        using interm_c_t = interm_t<BaseTC>;

        // first cast, then scale value
        auto cValue = s2s<interm_c_t, f, QC::f>(value);

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            detail::checkOverflow<QC::ovfBx, interm_c_t, interm_b_t>(cValue, QC::vMin, QC::vMax);
        }

        // create target value; disable overflow check to avoid that value is checked again
        return QC( static_cast<QC::base_t>(cValue) );
    }

    /// Explicit conversion to the related Sq type.
    /// \note If the limits are the same and the overflow behavior is not overridden, the unary plus
    /// operator can be applied to the q value instead to achieve the same conversion.
    template< double realVMinSq = realVMin, double realVMaxSq = realVMax, Overflow ovfBxOverride = ovfBx,
        /* deduced: */
        class SqTo = Sq<realVMinSq, realVMaxSq>,
        // include overflow check if the value range of Sq is narrower
        bool ovfCheckNeeded = (vMin < SqTo::vMin || SqTo::vMax < vMax) >
    requires detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
    constexpr
    SqTo toSq() const noexcept {
        base_t sqValue = value;

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            detail::checkOverflow<ovfBxOverride, base_t>(sqValue, SqTo::vMin, SqTo::vMax);
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
    Q() = delete;

    /// Explicit, possibly compile-time constructor from integer value.
    explicit constexpr Q(base_t value) noexcept : value(value) {}

    // friend all Q types so that private members of similar types can be accessed for construction
    // Note: As of May 2023, partial specializations cannot be friended, so we friend Q in general.
    template< std::integral BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
    requires detail::QRequirements<BaseTQ, fQ, realVMinQ, realVMaxQ, ovfQ>
    friend class Q;

    /// scaled integer value that represents a fixed-point value; stored in memory
    base_t value;
};

/// Explicit (static-) cast to another Q type with a potentially different base type.
/// Allows to override the default overflow behavior.
template< QType QC, Overflow ovfBxOverride = QC::ovfBx,
    /* deduced: */
    QType QFrom,
    // include overflow check if value range of source type is not fully within range of target
    // type, or if different overflow properties could result in overflow if not checked
    // note: real limits are compared because scaled integers with different base types and Q's
    //       cannot be compared so easily
    bool ovfCheckNeeded = (QFrom::realVMin < QC::realVMin || QC::realVMax < QFrom::realVMax
                           || is_ovf_stricter_v<ovfBxOverride, QFrom::ovfBx>
                           || is_ovf_stricter_v<QC::ovfBx, ovfBxOverride>) >
requires ( (detail::BaseTypeCanOverflowWhenAllowed<typename QC::base_t, ovfBxOverride>
            || detail::Scalable<typename QFrom::base_t, QFrom::f, typename QC::base_t, QC::f>)
           && detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded> )
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
            detail::checkOverflow<ovfBxOverride, interm_c_t, interm_f_t>(cValue, QC::vMin, QC::vMax);
        }

        // create target value; disable overflow check to avoid that value is checked again
        return QC::template construct<Overflow::noCheck>( static_cast<QC::base_t>(cValue) );
    }
}

/// Explicit, safe cast of a Q type to another Q type with a potentially different base type.
/// This will always perform overflow checks, even if not necessarily needed.
/// \note Overflow::allowed is not possible - use static_cast instead.
template< QType QC, Overflow ovfBxOverride = QC::ovfBx, /* deduced: */ QType QFrom >
requires ( detail::Scalable<typename QFrom::base_t, QFrom::f, typename QC::base_t, QC::f>
           && ovfBxOverride != Overflow::noCheck
           && detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, true> )
constexpr
QC safe_q_cast(QFrom from) noexcept {
    using interm_f_t = interm_t<typename QFrom::base_t>;
    using interm_c_t = interm_t<typename QC::base_t>;

    // scale source value and cast it to the intermediate type with the sign of the target type
    auto cValue = s2s<interm_c_t, QFrom::f, QC::f>(from.reveal());

    // always perform overflow checks
    detail::checkOverflow<ovfBxOverride, interm_c_t, interm_f_t>(cValue, QC::vMin, QC::vMax);

    // finally, create target value; disable overflow check to avoid that value is checked again
    return QC::template construct<Overflow::noCheck>( static_cast<QC::base_t>(cValue) );
}

/// Explicit, force cast of a Q type to a different Q type.
/// This will simply reinterpret the value of the source Q type as a value of the target Q type
/// without performing any scaling or overflow checks at all (overflow attributes are all ignored).
template< QType QC, /* deduced: */ QType QFrom >
constexpr
QC force_q_cast(QFrom from) noexcept {
    return QC::template construct<Overflow::noCheck>( static_cast<QC::base_t>(from.reveal()) );
}

/// Helper operations for mixed Sq and Q types. Converts Q to Sq and performs the operations on Sq.
/// \note The result is always some value of Sq type! If implicit conversion does not work, one of
/// Q's explicit conversion methods can be used to convert a value of Sq type to a value of Q type.
///\{

// Unary operations
template< /* deduced: */ QType Q > constexpr auto operator +(Q const &q) noexcept { return q.toSq(); }
template< /* deduced: */ QType Q > constexpr auto operator -(Q const &q) noexcept { return -q.toSq(); }
template< /* deduced: */ QType Q > constexpr auto abs(Q const &q) noexcept { return sq::abs( +q ); }

// Addition operator
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator +(Q const q, Sq const &sq) noexcept { return +q + sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator +(Sq const sq, Q const &q) noexcept { return sq + +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr auto operator +(Q1 const q1, Q2 const &q2) noexcept { return +q1 + +q2; }

// Subtraction operator
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator -(Q const q, Sq const &sq) noexcept { return +q - sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator -(Sq const sq, Q const &q) noexcept { return sq - +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr auto operator -(Q1 const q1, Q2 const &q2) noexcept { return +q1 - +q2; }

// Multiplication operator
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator *(Q const q, Sq const &sq) noexcept { return +q * sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator *(Sq const sq, Q const &q) noexcept { return sq * +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr auto operator *(Q1 const q1, Q2 const &q2) noexcept { return +q1 * +q2; }

// Division operator
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator /(Q const q, Sq const &sq) noexcept { return +q / sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator /(Sq const sq, Q const &q) noexcept { return sq / +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr auto operator /(Q1 const q1, Q2 const &q2) noexcept { return +q1 / +q2; }

// Modulus operator
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator %(Q const q, Sq const &sq) noexcept { return +q % sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr auto operator %(Sq const sq, Q const &q) noexcept { return sq % +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr auto operator %(Q1 const q1, Q2 const &q2) noexcept { return +q1 % +q2; }

// Comparison operators
template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator <(Q const &q, Sq const &sq) noexcept { return +q < sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator <(Sq const &sq, Q const &q) noexcept { return sq < +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr bool operator <(Q1 const &q1, Q2 const &q2) noexcept { return +q1 < +q2; }

template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator <=(Q const &q, Sq const &sq) noexcept { return +q <= sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator <=(Sq const &sq, Q const &q) noexcept { return sq <= +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr bool operator <=(Q1 const &q1, Q2 const &q2) noexcept { return +q1 <= +q2; }

template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator >(Q const &q, Sq const &sq) noexcept { return +q > sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator >(Sq const &sq, Q const &q) noexcept { return sq > +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr bool operator >(Q1 const &q1, Q2 const &q2) noexcept { return +q1 > +q2; }

template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator >=(Q const &q, Sq const &sq) noexcept { return +q >= sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator >=(Sq const &sq, Q const &q) noexcept { return sq >= +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr bool operator >=(Q1 const &q1, Q2 const &q2) noexcept { return +q1 >= +q2; }

template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator ==(Q const &q, Sq const &sq) noexcept { return +q == sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator ==(Sq const &sq, Q const &q) noexcept { return sq == +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr bool operator ==(Q1 const &q1, Q2 const &q2) noexcept { return +q1 == +q2; }

template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator !=(Q const &q, Sq const &sq) noexcept { return +q != sq; }
template< /* deduced: */ QType Q, SqType Sq > constexpr bool operator !=(Sq const &sq, Q const &q) noexcept { return sq != +q; }
template< /* deduced: */ QType Q1, QType Q2 > constexpr bool operator !=(Q1 const &q1, Q2 const &q2) noexcept { return +q1 != +q2; }

// Shift operators
template< /* deduced: */ QType Q, std::integral T, T v > constexpr bool operator <<(Q const &q, std::integral_constant<T, v> const b) noexcept { return +q << b; }
template< /* deduced: */ QType Q, std::integral T, T v > constexpr bool operator >>(Q const &q, std::integral_constant<T, v> const b) noexcept { return +q >> b; }

// Clamp
template< /* deduced: */ QType Q1, QType Q2, QType Q3 > constexpr auto clamp(Q1 const &value, Q2 const &lo, Q3 const &hi) { return sq::clamp( +value, +lo, +hi ); }
template< /* deduced: */ SqType Sq1, QType Q2, QType Q3 > constexpr auto clamp(Sq1 const &value, Q2 const &lo, Q3 const &hi) { return sq::clamp( value, +lo, +hi ); }
template< /* deduced: */ QType Q1, SqType Sq2, QType Q3 > constexpr auto clamp(Q1 const &value, Sq2 const &lo, Q3 const &hi) { return sq::clamp( +value, lo, +hi ); }
template< /* deduced: */ QType Q1, QType Q2, SqType Sq3 > constexpr auto clamp(Q1 const &value, Q2 const &lo, Sq3 const &hi) { return sq::clamp( +value, +lo, hi ); }
template< /* deduced: */ SqType Sq1, SqType Sq2, QType Q3 > constexpr auto clamp(Sq1 const &value, Sq2 const &lo, Q3 const &hi) { return sq::clamp( value, lo, +hi ); }
template< /* deduced: */ SqType Sq1, QType Q2, SqType Sq3 > constexpr auto clamp(Sq1 const &value, Q2 const &lo, Sq3 const &hi) { return sq::clamp( value, +lo, hi ); }
template< /* deduced: */ QType Q1, SqType Sq2, SqType Sq3 > constexpr auto clamp(Q1 const &value, Sq2 const &lo, Sq3 const &hi) { return sq::clamp( +value, lo, hi ); }
template< /* deduced: */ QType Q1, QType Q2 > constexpr auto clampLower(Q1 const &value, Q2 const &lo) { return sq::clampLower( +value, +lo ); }
template< /* deduced: */ SqType Sq1, QType Q2 > constexpr auto clampLower(Sq1 const &value, Q2 const &lo) { return sq::clampLower( value, +lo ); }
template< /* deduced: */ QType Q1, SqType Sq2 > constexpr auto clampLower(Q1 const &value, Sq2 const &lo) { return sq::clampLower( +value, lo ); }
template< /* deduced: */ QType Q1, QType Q2 > constexpr auto clampUpper(Q1 const &value, Q2 const &hi) { return sq::clampUpper( +value, +hi ); }
template< /* deduced: */ SqType Sq1, QType Q2 > constexpr auto clampUpper(Sq1 const &value, Q2 const &hi) { return sq::clampUpper( value, +hi ); }
template< /* deduced: */ QType Q1, SqType Sq2 > constexpr auto clampUpper(Q1 const &value, Sq2 const &hi) { return sq::clampUpper( +value, hi ); }
template< double realLo, double realHi, /* deduced: */ QType Q1 > constexpr auto clamp(Q1 const &value) { return sq::clamp<realLo, realHi>( +value ); }
template< double realLo, /* deduced: */ QType Q1 > constexpr auto clampLower(Q1 const &value) { return sq::clampLower<realLo>( +value ); }
template< double realHi, /* deduced: */ QType Q1 > constexpr auto clampUpper(Q1 const &value) { return sq::clampUpper<realHi>( +value ); }

// Min
template< /* deduced: */ QType Q, SqType Sq > constexpr auto min(Q const &q, Sq const &sq) noexcept { return sq::min(+q, sq); }
template< /* deduced: */ QType Q, SqType Sq > constexpr auto min(Sq const &sq, Q const &q) noexcept { return sq::min(sq, +q); }
template< /* deduced: */ QType Q1, QType Q2 > constexpr auto min(Q1 const &q1, Q2 const &q2) noexcept { return sq::min(+q1, +q2); }

// Max
template< /* deduced: */ QType Q, SqType Sq > constexpr auto max(Q const &q, Sq const &sq) noexcept { return sq::max(+q, sq); }
template< /* deduced: */ QType Q, SqType Sq > constexpr auto max(Sq const &sq, Q const &q) noexcept { return sq::max(sq, +q); }
template< /* deduced: */ QType Q1, QType Q2 > constexpr auto max(Q1 const &q1, Q2 const &q2) noexcept { return sq::max(+q1, +q2); }

///\}

/// Converts a literal number into the corresponding best-fit Q type.
/// Best-fit means that the literal number represents both limits and the value.
template< QType Q, char ...charArray >
consteval auto qFromLiteral() {
    constexpr double value = detail::doubleFromLiteral<charArray...>();
    return Q::template clamp_t<value, value>::template fromReal<value>;
}

/**\}*/
}  // end of fpm::q

namespace std {

/// Provides the bare, real numeric limits for the given Q type.
template< /* deduced: */ std::integral BaseT, fpm::scaling_t f, double realVMin, double realVMax, fpm::Overflow ovfBx >
class numeric_limits<fpm::q::Q<BaseT, f, realVMin, realVMax, ovfBx>> {
    using QT = fpm::q::Q<BaseT, f, realVMin, realVMax, ovfBx>;
public:
    /// \returns the minimum real value that can be represented by the Q type.
    /// \note In contrast to Q::realVMin, this does not return the minimum value specified by the
    /// user, but the absolute minimum that can be represented by the underlying Q type with respect
    /// to its base type and scaling. This can be significantly smaller that the actual user minimum.
    template< typename T = double >
    static constexpr T min() noexcept {
        return fpm::v2s<T, -QT::f>( numeric_limits<typename QT::base_t>::min() );
    }

    /// \returns the maximum real value that can be represented by the Q type.
    /// \note In contrast to Q::realVMax, this does not return the maximum value specified by the
    /// user, but the absolute maximum that can be represented by the underlying Q type with respect
    /// to its base type and scaling. This can be significantly larger that the actual user maximum.
    template< typename T = double >
    static constexpr T max() noexcept {
        return fpm::v2s<T, -QT::f>( numeric_limits<typename QT::base_t>::max() );
    }

    constexpr static bool is_specialized = true;
    constexpr static bool is_signed = numeric_limits<typename QT::base_t>::is_signed;
    constexpr static bool is_bounded = true;
    constexpr static bool traps = true;
#   if defined FPM_USE_SH
    constexpr static auto round_style = std::round_toward_neg_infinity;
#   else
    constexpr static auto round_style = std::round_toward_zero;
#   endif
    constexpr static int radix = numeric_limits<typename QT::base_t>::radix;
    constexpr static int digits = numeric_limits<typename QT::base_t>::digits - QT::f;
    constexpr static int digits10 = static_cast<int>( std::log10(radix) * digits );
};

}

#endif
// EOF
