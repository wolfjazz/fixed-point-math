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

using fpm::detail::QType;
using fpm::detail::SqType;

namespace detail {

template< class QT, Overflow ovf >
concept Constructable = requires(typename QT::base_t v) { { QT::template constructImpl<ovf>(v) } -> std::same_as<QT>; };

template< class QT, class QF, Overflow ovf >
concept CopyConstructable = requires(QF const &qFrom) { { QT::template fromQImpl<ovf>(qFrom) } -> std::same_as<QT>; };

template< class QT, class SqF, Overflow ovf >
concept FromSqConstructable = requires(SqF const &sqFrom) { { QT::template fromSqImpl<ovf>(sqFrom) } -> std::same_as<QT>; };

template< class QT, class QC >
concept Castable = requires(QT q) { { q.template castImpl<QC>() } -> std::same_as<QC>; };

template< class QT, double realVMinSq, double realVMaxSq, Overflow ovf >
concept SqConvertable = requires(QT q) {
    { q.template toSqImpl<realVMinSq, realVMaxSq, ovf>() } -> std::same_as< typename QT::Sq<realVMinSq, realVMaxSq> >;
};

}

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
///          calculations. It's highly recommended to use -0.0 when a type is declared that has only
///          negative numbers up to 0 in its range.
template<
    std::integral BaseT,  ///< type of the scaled integer stored in memory
    scaling_t f_,         ///< number of fraction bits (precision 2^(-f))
    double realVMin_ = fpm::detail::lowestRealVMin<BaseT, f_>,   ///< minimum real value represented by the type
    double realVMax_ = fpm::detail::highestRealVMax<BaseT, f_>,  ///< maximum real value represented by the type
    Overflow ovfBx_ = Overflow::forbidden >  ///< overflow behavior; overflow is forbidden by default
requires fpm::detail::QRequirements<BaseT, f_, realVMin_, realVMax_, ovfBx_>
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
    template< Overflow ovfBxOverride = ovfBx > requires detail::Constructable<Q, ovfBxOverride>
    static constexpr
    Q construct(base_t value) noexcept { return constructImpl<ovfBxOverride>(value); }

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
    requires ( fpm::detail::RealValueScaledFitsBaseType<base_t, f, realValue>
               && Overflow::assert != ovfBxOverride
               && fpm::detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxEffective> )
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
               && fpm::detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxEffective> )
    static constexpr
    Q fromScaled = construct<ovfBxEffective>( value );

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
    template< Overflow ovfBxOverride = ovfBx, /* deduced: */ QType QFrom >
    requires detail::CopyConstructable<Q, QFrom, ovfBxOverride>
    static constexpr
    Q fromQ(QFrom const &from) noexcept { return fromQImpl<ovfBxOverride>(from); }

    /// Named "constructor" from a Sq variable with the same base type.
    /// \note The representation error to be expected is comparable to that of Q::fromQ.
    /// \note An overflow check is included if the range of the Sq type is not entirely within the
    /// range of the Q type.
    template< Overflow ovfBxOverride = ovfBx, /* deduced: */ SqType SqFrom >
    requires detail::FromSqConstructable<Q, SqFrom, ovfBxOverride>
    static constexpr
    Q fromSq(SqFrom const &fromSq) noexcept { return fromSqImpl<ovfBxOverride>(fromSq); }

    /// Alias for Q::fromSq<Overflow::forbidden>(.)
    template< /* deduced: */ SqType SqFrom >
    requires detail::FromSqConstructable<Q, SqFrom, Overflow::forbidden>
    static constexpr
    Q fromSqNOvf(SqFrom const &fromSq) noexcept { return Q::template fromSq<Overflow::forbidden>(fromSq); }

    /// Alias for Q::fromSq<Overflow::clamp>(.)
    template< /* deduced: */ SqType SqFrom >
    requires detail::FromSqConstructable<Q, SqFrom, Overflow::clamp>
    static constexpr
    Q fromSqClamp(SqFrom const &fromSq) noexcept { return Q::template fromSq<Overflow::clamp>(fromSq); }

    /// Alias for Q::fromSq<Overflow::noCheck>(.) and Q::fromSq<Overflow::allowed>(.)
    template< /* deduced: */ SqType SqFrom >
    requires detail::FromSqConstructable<Q, SqFrom, Overflow::noCheck>
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
               && fpm::detail::Scalable<base_t, QFrom::f, base_t, f>
               && fpm::detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBx, ovfCheckNeeded> )
    constexpr
    Q(QFrom const &from) noexcept : value( Q::fromQ<ovfBx>(from).value ) {}

    /// Implicit copy constructor from a Sq type with the same or a narrower range.
    /// \note This works as long as no overflow check is necessary.
    /// \note The representation error to be expected is comparable to that of Q::fromSq.
    template< /* deduced: */ SqType SqFrom >
    requires fpm::detail::ImplicitlyConvertible<SqFrom, Q>
    constexpr
    Q(SqFrom const &from) noexcept : value( s2s<base_t, SqFrom::f, f>(from.value) ) {}

    /// Copy-Constructor from the same type.
    constexpr
    Q(Q const &) noexcept = default;

    /// Copy-Assignment from the same type.
    constexpr
    Q& operator =(Q const &) noexcept = default;

    /// Move-Constructor from the same type.
    constexpr
    Q(Q&&) noexcept = default;

    /// Move-Assignment from the same type.
    constexpr
    Q& operator =(Q&&) noexcept = default;

    /// Destructor.
    constexpr
    ~Q() {}

    /// Copy-Assignment from a different Q type with the same base type.
    /// \note The rhs type must have a range that is fully within the range of the target type.
    /// If this is not the case, use the static fromQ constructor function.
    template< /* deduced: */ QType QFrom >
    requires ( !std::is_same_v< Q, QFrom >
               && fpm::detail::Scalable<base_t, QFrom::f, base_t, f>
               && realVMin <= QFrom::realVMin && QFrom::realVMax <= realVMax )
    constexpr
    Q& operator =(QFrom const &from) noexcept {
        value = fromQ(from).value;
        return *this;
    }

    /// Explicit (static-) cast to a different Q type with a potentially different base type.
    /// \note If a cast does not work it's most probably due to unfulfilled requirements. Double check
    ///       whether a runtime overflow check is needed and make sure that it is allowed!
    template< /* deduced: */ QType QC >
    requires detail::Castable<Q, QC>
    explicit constexpr
    operator QC() const noexcept { return castImpl<QC>(); }

    /// Explicit conversion to the related Sq type.
    /// \note If the limits are the same and the overflow behavior is not overridden, the unary plus
    /// operator can be applied to the q value instead to achieve the same conversion.
    template< double realVMinSq = realVMin, double realVMaxSq = realVMax, Overflow ovfBxOverride = ovfBx >
    requires detail::SqConvertable<Q, realVMinSq, realVMaxSq, ovfBxOverride>
    constexpr
    auto toSq() const noexcept { return toSqImpl<realVMinSq, realVMaxSq, ovfBxOverride>(); }

    /// Reveals the integer value stored in memory.
    constexpr
    base_t reveal() const noexcept { return value; }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning This conversion is expensive if the target type is a floating-point type.
    ///          If the target type is an integral type, there can be a significant loss of precision.
    ///          Use carefully!
#   if defined FPM_USE_SH
    template< typename TargetT = int >
#   else
    template< typename TargetT = double >
#   endif
    constexpr
    TargetT toReal() const noexcept { return v2s<TargetT, -f>(value); }
    /// Implicit conversion of a Q value back into its double representation. Allows using a
    /// value+unit literal where a double is expected. Compile-time only!
    consteval
    operator double() const noexcept { return toReal<double>(); }

private:
    // delete default (runtime) constructor
    Q() = delete;

    /// Explicit, possibly compile-time constructor from integer value.
    explicit constexpr
    Q(base_t value) noexcept : value(value) {}

    // friend all Q types so that private members of similar types can be accessed for construction
    // Note: As of May 2023, partial specializations cannot be friended, so we friend Q in general.
    template< std::integral BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
    requires fpm::detail::QRequirements<BaseTQ, fQ, realVMinQ, realVMaxQ, ovfQ>
    friend class Q;

    //
    // actual implementation methods; the public variants are mostly just to hide implementation
    // specifics so that hover help texts and API descriptions are not too confusing
    //

    template< Overflow ovfBxOverride = ovfBx >
    static constexpr
    Q constructImpl(base_t value) noexcept {
        // perform overflow check
        fpm::detail::checkOverflow<ovfBxOverride, base_t>(value, vMin, vMax);

        return Q( value );
    }

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
               && fpm::detail::Scalable<base_t, QFrom::f, base_t, f>
               && fpm::detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded> )
    static constexpr
    Q fromQImpl(QFrom const &from) noexcept {
        using interm_b_t = interm_t<base_t>;
        interm_b_t fromValueScaled = s2s<interm_b_t, QFrom::f, f>( from.reveal() );

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            fpm::detail::checkOverflow<ovfBxOverride, interm_b_t>(fromValueScaled, vMin, vMax);
        }

        return Q( static_cast<base_t>(fromValueScaled) );
    }

    template< Overflow ovfBxOverride = ovfBx,
        /* deduced: */
        SqType SqFrom,
        // include overflow check if value range of source is not fully within range of target
        bool ovfCheckNeeded = (SqFrom::realVMin < realVMin || realVMax < SqFrom::realVMax) >
    requires ( std::is_same_v<base_t, typename SqFrom::base_t>
               && fpm::detail::Scalable<base_t, SqFrom::f, base_t, f>
               && fpm::detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded> )
    static constexpr
    Q fromSqImpl(SqFrom const &fromSq) noexcept {
        base_t qValue = s2s<base_t, SqFrom::f, f>(fromSq.value);

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            fpm::detail::checkOverflow<ovfBxOverride, base_t>(qValue, vMin, vMax);
        }

        return Q( qValue );
    }

    template< QType QC,
        /* deduced: */
        // include overflow check if value range of source type is not fully within range of target
        // type, or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different base types and Q's
        //       cannot be compared so easily
        bool ovfCheckNeeded = (realVMin < QC::realVMin || QC::realVMax < realVMax || is_ovf_stricter_v<QC::ovfBx, ovfBx>) >
    requires ( fpm::detail::Scalable<base_t, f, typename QC::base_t, QC::f>
               && fpm::detail::RuntimeOverflowCheckAllowedWhenNeeded<QC::ovfBx, ovfCheckNeeded> )
    constexpr
    auto castImpl() const noexcept {
        using interm_b_t = interm_t<base_t>;
        using interm_c_t = interm_t<typename QC::base_t>;

        // first cast, then scale value
        auto cValue = s2s<interm_c_t, f, QC::f>(value);

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            fpm::detail::checkOverflow<QC::ovfBx, interm_c_t, interm_b_t>(cValue, QC::vMin, QC::vMax);
        }

        // create target value; disable overflow check to avoid that value is checked again
        return QC( static_cast<typename QC::base_t>(cValue) );
    }

    template< double realVMinSq, double realVMaxSq, Overflow ovfBxOverride,
        /* deduced: */
        class SqTo = Sq<realVMinSq, realVMaxSq>,
        // include overflow check if the value range of Sq is narrower
        bool ovfCheckNeeded = (vMin < SqTo::vMin || SqTo::vMax < vMax) >
    requires fpm::detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded>
    constexpr
    SqTo toSqImpl() const noexcept {
        base_t sqValue = value;

        // perform overflow check if needed
        if constexpr (ovfCheckNeeded) {
            fpm::detail::checkOverflow<ovfBxOverride, base_t>(sqValue, SqTo::vMin, SqTo::vMax);
        }

        return SqTo( sqValue );
    }

    /// Explicit (static-) cast to another Q type with a potentially different base type.
    /// Allows to override the default overflow behavior.
    template< QType QC, Overflow ovfBxOverride = QC::ovfBx,
        /* deduced: */
        // include overflow check if value range of source type is not fully within range of target
        // type, or if different overflow properties could result in overflow if not checked
        // note: real limits are compared because scaled integers with different base types and Q's
        //       cannot be compared so easily
        bool ovfCheckNeeded = (realVMin < QC::realVMin || QC::realVMax < realVMax
                               || is_ovf_stricter_v<ovfBxOverride, ovfBx>
                               || is_ovf_stricter_v<QC::ovfBx, ovfBxOverride>) >
    requires ( (fpm::detail::BaseTypeCanOverflowWhenAllowed<typename QC::base_t, ovfBxOverride>
                || fpm::detail::Scalable<base_t, f, typename QC::base_t, QC::f>)
               && fpm::detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, ovfCheckNeeded> )
    friend constexpr
    QC static_q_cast(Q from) noexcept {
        using interm_f_t = interm_t<base_t>;
        using interm_c_t = interm_t<typename QC::base_t>;

        // static_cast can be reused if overflow is not overwritten
        if constexpr (ovfBxOverride == QC::ovfBx) {
            return static_cast<QC>(from);
        }
        // if overflow override is specified, compile overflow checks
        else {
            // scale source value and cast it to the intermediate type with the sign of the target type
            auto cValue = s2s<interm_c_t, f, QC::f>(from.value);

            // perform overflow check if needed
            if constexpr (ovfCheckNeeded) {
                fpm::detail::checkOverflow<ovfBxOverride, interm_c_t, interm_f_t>(cValue, QC::vMin, QC::vMax);
            }

            // create target value; disable overflow check to avoid that value is checked again
            return QC::template construct<Overflow::noCheck>( static_cast<QC::base_t>(cValue) );
        }
    }

    /// Explicit, safe cast of a Q type to another Q type with a potentially different base type.
    /// This will always perform overflow checks, even if not necessarily needed.
    /// \note Overflow::allowed is not possible - use static_cast instead.
    template< QType QC, Overflow ovfBxOverride = QC::ovfBx >
    requires ( fpm::detail::Scalable<base_t, f, typename QC::base_t, QC::f>
               && ovfBxOverride != Overflow::noCheck
               && fpm::detail::RuntimeOverflowCheckAllowedWhenNeeded<ovfBxOverride, true> )
    friend constexpr
    QC safe_q_cast(Q from) noexcept {
        using interm_f_t = interm_t<base_t>;
        using interm_c_t = interm_t<typename QC::base_t>;

        // scale source value and cast it to the intermediate type with the sign of the target type
        auto cValue = s2s<interm_c_t, f, QC::f>(from.value);

        // always perform overflow checks
        fpm::detail::checkOverflow<ovfBxOverride, interm_c_t, interm_f_t>(cValue, QC::vMin, QC::vMax);

        // finally, create target value; disable overflow check to avoid that value is checked again
        return QC::template construct<Overflow::noCheck>( static_cast<QC::base_t>(cValue) );
    }

    /// Explicit, force cast of a Q type to a different Q type.
    /// This will simply reinterpret the value of the source Q type as a value of the target Q type
    /// without performing any scaling or overflow checks at all (overflow attributes are all ignored).
    template< QType QC >
    friend constexpr
    QC force_q_cast(Q from) noexcept {
        return QC::template construct<Overflow::noCheck>( static_cast<QC::base_t>(from.value) );
    }

    //
    // memory
    //

    /// scaled integer value that represents a fixed-point value; stored in memory
    base_t value;
};

/// Helper operations for mixed Sq and Q types. Converts Q to Sq and performs the operations on Sq.
/// \note The result is always some value of Sq type! If implicit conversion does not work, one of
/// Q's explicit conversion methods can be used to convert a value of Sq type to a value of Q type.
///\{

// Unary
constexpr auto operator +(QType auto const &q) noexcept { return q.toSq(); }  // note: conversion to similar Sq type
constexpr auto operator -(QType auto const &q) noexcept { return -q.toSq(); }
constexpr auto abs(QType auto const &q) noexcept { return abs( +q ); }

// Addition
constexpr auto operator +(QType auto const q, SqType auto const &sq) noexcept { return +q + sq; }
constexpr auto operator +(SqType auto const sq, QType auto const &q) noexcept { return sq + +q; }
constexpr auto operator +(QType auto const q1, QType auto const &q2) noexcept { return +q1 + +q2; }

// Subtraction
constexpr auto operator -(QType auto const q, SqType auto const &sq) noexcept { return +q - sq; }
constexpr auto operator -(SqType auto const sq, QType auto const &q) noexcept { return sq - +q; }
constexpr auto operator -(QType auto const q1, QType auto const &q2) noexcept { return +q1 - +q2; }

// Multiplication
constexpr auto operator *(QType auto const q, SqType auto const &sq) noexcept { return +q * sq; }
constexpr auto operator *(SqType auto const sq, QType auto const &q) noexcept { return sq * +q; }
constexpr auto operator *(QType auto const q1, QType auto const &q2) noexcept { return +q1 * +q2; }
template< /* deduced: */ std::integral T, T v >
constexpr auto operator *(QType auto const q, std::integral_constant<T, v> const ic) noexcept { return +q * ic; }
template< /* deduced: */ std::integral T, T v >
constexpr auto operator *(std::integral_constant<T, v> const ic, QType auto const &q) noexcept { return ic * +q; }

// Division
constexpr auto operator /(QType auto const q, SqType auto const &sq) noexcept { return +q / sq; }
constexpr auto operator /(SqType auto const sq, QType auto const &q) noexcept { return sq / +q; }
constexpr auto operator /(QType auto const q1, QType auto const &q2) noexcept { return +q1 / +q2; }
template< /* deduced: */ std::integral T, T v >
constexpr auto operator /(QType auto const q, std::integral_constant<T, v> const ic) noexcept { return +q / ic; }
template< /* deduced: */ std::integral T, T v >
constexpr auto operator /(std::integral_constant<T, v> const ic, QType auto const &q) noexcept { return ic / +q; }

// Modulus
constexpr auto operator %(QType auto const q, SqType auto const &sq) noexcept { return +q % sq; }
constexpr auto operator %(SqType auto const sq, QType auto const &q) noexcept { return sq % +q; }
constexpr auto operator %(QType auto const q1, QType auto const &q2) noexcept { return +q1 % +q2; }

// Comparison
constexpr bool operator ==(QType auto const &q, SqType auto const &sq) noexcept { return +q == sq; }
constexpr bool operator ==(SqType auto const &sq, QType auto const &q) noexcept { return sq == +q; }
constexpr bool operator ==(QType auto const &q1, QType auto const &q2) noexcept { return +q1 == +q2; }

// Ordering
constexpr std::strong_ordering operator <=>(QType auto const &q, SqType auto const &sq) noexcept { return +q <=> sq; }
constexpr std::strong_ordering operator <=>(SqType auto const &sq, QType auto const &q) noexcept { return sq <=> +q; }
constexpr std::strong_ordering operator <=>(QType auto const &q1, QType auto const &q2) noexcept { return +q1 <=> +q2; }

// Shift
template< /* deduced: */ std::integral T, T v >
constexpr bool operator <<(QType auto const &q, std::integral_constant<T, v> const ic) noexcept { return +q << ic; }
template< /* deduced: */ std::integral T, T v >
constexpr bool operator >>(QType auto const &q, std::integral_constant<T, v> const ic) noexcept { return +q >> ic; }

// Square(-Root)
constexpr auto square(QType auto const &q) noexcept { return square( +q ); }
constexpr auto sqrt(QType auto const &q) noexcept { return sqrt( +q ); }
constexpr auto rsqrt(QType auto const &q) noexcept { return rsqrt( +q ); }

// Cube(-Root)
constexpr auto cube(QType auto const &q) noexcept { return cube( +q ); }
constexpr auto cbrt(QType auto const &q) noexcept { return cbrt( +q ); }
//constexpr auto rcbrt(QType auto const &q) noexcept { return rcbrt( +q ); }

// Clamp
constexpr auto clamp(QType auto const &value, QType auto const &lo, QType auto const &hi) { return clamp( +value, +lo, +hi ); }
constexpr auto clamp(SqType auto const &value, QType auto const &lo, QType auto const &hi) { return clamp( value, +lo, +hi ); }
constexpr auto clamp(QType auto const &value, SqType auto const &lo, QType auto const &hi) { return clamp( +value, lo, +hi ); }
constexpr auto clamp(QType auto const &value, QType auto const &lo, SqType auto const &hi) { return clamp( +value, +lo, hi ); }
constexpr auto clamp(SqType auto const &value, SqType auto const &lo, QType auto const &hi) { return clamp( value, lo, +hi ); }
constexpr auto clamp(SqType auto const &value, QType auto const &lo, SqType auto const &hi) { return clamp( value, +lo, hi ); }
constexpr auto clamp(QType auto const &value, SqType auto const &lo, SqType auto const &hi) { return clamp( +value, lo, hi ); }
constexpr auto clampLower(QType auto const &value, QType auto const &lo) { return clampLower( +value, +lo ); }
constexpr auto clampLower(SqType auto const &value, QType auto const &lo) { return clampLower( value, +lo ); }
constexpr auto clampLower(QType auto const &value, SqType auto const &lo) { return clampLower( +value, lo ); }
constexpr auto clampUpper(QType auto const &value, QType auto const &hi) { return clampUpper( +value, +hi ); }
constexpr auto clampUpper(SqType auto const &value, QType auto const &hi) { return clampUpper( value, +hi ); }
constexpr auto clampUpper(QType auto const &value, SqType auto const &hi) { return clampUpper( +value, hi ); }
template< double realLo, double realHi > constexpr auto clamp(QType auto const &value) { return clamp<realLo, realHi>( +value ); }
template< double realLo > constexpr auto clampLower(QType auto const &value) { return clampLower<realLo>( +value ); }
template< double realHi > constexpr auto clampUpper(QType auto const &value) { return clampUpper<realHi>( +value ); }

// Min
constexpr auto min(QType auto const &q, SqType auto const &sq) noexcept { return min(+q, sq); }
constexpr auto min(SqType auto const &sq, QType auto const &q) noexcept { return min(sq, +q); }
constexpr auto min(QType auto const &q1, QType auto const &q2) noexcept { return min(+q1, +q2); }

// Max
constexpr auto max(QType auto const &q, SqType auto const &sq) noexcept { return max(+q, sq); }
constexpr auto max(SqType auto const &sq, QType auto const &q) noexcept { return max(sq, +q); }
constexpr auto max(QType auto const &q1, QType auto const &q2) noexcept { return max(+q1, +q2); }

///\}

/// Converts a literal number into the corresponding best-fit Q type.
/// Best-fit means that the literal number represents both limits and the value.
template< QType Q, char ...charArray >
consteval auto fromLiteral() {
    constexpr double value = fpm::detail::doubleFromLiteral<charArray...>();
    return Q::template clamp_t<value, value>::template fromReal<value>;
}
/// Associates a Q type with a literal.
#define FPM_Q_BIND_LITERAL(_q, _literal) \
    template< char ...chars > consteval auto operator "" ## _ ## _literal () { return fpm::q::fromLiteral<_q, chars...>(); }

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
