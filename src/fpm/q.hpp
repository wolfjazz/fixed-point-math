/** \file
 * Implementation of the Q class template.
 */

#ifndef FPM_FPM_Q_HPP_D7920966_3ECD_4275_96CB_7B5D755A3B18
#define FPM_FPM_Q_HPP_D7920966_3ECD_4275_96CB_7B5D755A3B18

#include "sq.hpp"


namespace fpm::q {
/** \ingroup grp_fpm
 * \defgroup grp_fpmQ Q Type
 * \{ */

using fpm::detail::QType;
using fpm::detail::SqType;


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
    double realMin_ = fpm::realMin<BaseT, f_>,  ///< minimum real value represented by the type
    double realMax_ = fpm::realMax<BaseT, f_>,  ///< maximum real value represented by the type
    Overflow ovfBx_ = Overflow::error >  ///< overflow behavior
requires fpm::detail::QRequirements<BaseT, f_, realMin_, realMax_, ovfBx_>
class Q final {
public:
    static constexpr bool isQType = true;  ///< identifier for the QType concept
    using base_t = BaseT;  /// integral base type
    static constexpr scaling_t f = f_;  ///< number of fraction bits
    static constexpr double realMin = realMin_;  ///< minimum real value
    static constexpr double realMax = realMax_;  ///< maximum real value
    static constexpr base_t scaledMin = fpm::scaled<base_t, f>(realMin_);  ///< minimum value of scaled integer value range
    static constexpr base_t scaledMax = fpm::scaled<base_t, f>(realMax_);  ///< maximum value of scaled integer value range
    static constexpr double resolution = fpm::resolution<f>();  ///< real resolution
    static constexpr Overflow ovfBx = ovfBx_;  ///< overflow behavior

    /// Corresponding (related) Sq type.
    template< double realMinSq = realMin, double realMaxSq = realMax >
    using Sq = sq::Sq< base_t, f, realMinSq, realMaxSq >;

    /// Create a new Q type with the same base type and scaling but a different real value range
    /// and overflow behavior. The default overflow behavior is Ovf::clamp, or the behavior of the
    /// source type if it is stricter.
    template< double newRealMin, double newRealMax,
        Overflow ovfBxOvrd = is_ovf_stricter_v<ovfBx, Ovf::clamp> ? ovfBx : Ovf::clamp >
    struct clamp { using type = Q< base_t, f, newRealMin, newRealMax, ovfBxOvrd >; };

    /// Type alias for clamp::type.
    template< double newRealMin, double newRealMax, Overflow ovfBxOvrd = ovfBx >
    using clamp_t = clamp<newRealMin, newRealMax, ovfBxOvrd>::type;

private:
    // Implementation helper structs.
    // The public variants are designed to hide implementation specifics
    // so that hover help texts and API descriptions are not too confusing.

    /// Implements the compile-time construction of this Q type from a real value.
    template< double real, Overflow ovfBxOvrd >
    struct FromR {
        using base_t = typename Q::base_t;
        static constexpr scaling_t f = Q::f;
        static constexpr double realMin = Q::realMin;
        static constexpr double realMax = Q::realMax;
        static constexpr base_t scaled = fpm::scaled<base_t, f>(real);
        static constexpr Overflow ovfBx = (Q::scaledMin <= scaled && scaled <= Q::scaledMax) ? Overflow::unchecked : ovfBxOvrd;
        static constexpr bool innerConstraints = fpm::detail::CompileTimeConstructionAllowed<ovfBx>;
        static constexpr base_t value() noexcept { return fpm::scaled<base_t, f>(real); }
    };

    /// Implements the compile-time construction of this Q type from a scaled value.
    template< base_t scaled, Overflow ovfBxOvrd >
    struct FromS {
        using base_t = typename Q::base_t;
        static constexpr scaling_t f = Q::f;
        static constexpr double realMin = Q::realMin;
        static constexpr double realMax = Q::realMax;
        static constexpr Overflow ovfBx = (Q::scaledMin <= scaled && scaled <= Q::scaledMax) ? Overflow::unchecked : ovfBxOvrd;
        static constexpr bool innerConstraints = fpm::detail::CompileTimeConstructionAllowed<ovfBx>;
        static constexpr base_t value() noexcept { return scaled; }
    };

    /// Implements the conversion from another Q type with the same base type.
    template< QType QFrom, Overflow ovfBxOvrd >
    requires ( std::is_same_v<Q::base_t, typename QFrom::base_t>
               && fpm::detail::Scalable<typename QFrom::base_t, QFrom::f, typename Q::base_t, Q::f> )
    struct FromQ {
        using base_t = typename Q::base_t;
        static constexpr scaling_t f = Q::f;
        static constexpr double realMin = Q::realMin;
        static constexpr double realMax = Q::realMax;
        static constexpr Overflow ovfBx = ovfBxOvrd;
        // include overflow check if value range of from-type is not fully within range of target type,
        // or if different overflow properties could result in overflow if not checked
        static constexpr bool ovfCheckNeeded = (QFrom::realMin < realMin || realMax < QFrom::realMax
                                                || is_ovf_stricter_v<ovfBx, QFrom::ovfBx> || is_ovf_stricter_v<Q::ovfBx, ovfBx>);
        static constexpr bool innerConstraints = fpm::detail::OvfCheckAllowedWhenNeeded<ovfBx, ovfCheckNeeded>;
        static constexpr base_t value(typename QFrom::base_t from) noexcept {
            base_t value = s2s<base_t, QFrom::f, f>( from );
            // perform overflow check if needed
            if constexpr (ovfCheckNeeded) {
                fpm::detail::checkOverflow<ovfBxOvrd>( value, Q::scaledMin, Q::scaledMax );
            }
            return value;
        }
    };

    /// Implements the conversion from an Sq type with the same base type.
    template< SqType SqFrom, Overflow ovfBxOvrd >
    requires ( std::is_same_v<Q::base_t, typename SqFrom::base_t>
               && fpm::detail::Scalable<typename SqFrom::base_t, SqFrom::f, typename Q::base_t, Q::f> )
    struct FromSq {
        using base_t = typename Q::base_t;
        static constexpr scaling_t f = Q::f;
        static constexpr double realMin = Q::realMin;
        static constexpr double realMax = Q::realMax;
        static constexpr Overflow ovfBx = ovfBxOvrd;
        // include overflow check if value range of source is not fully within range of target
        static constexpr bool ovfCheckNeeded = (SqFrom::realMin < realMin || realMax < SqFrom::realMax);
        static constexpr bool innerConstraints = fpm::detail::OvfCheckAllowedWhenNeeded<ovfBx, ovfCheckNeeded>;
        static constexpr base_t value(typename SqFrom::base_t fromSq) noexcept {
            base_t value = s2s<base_t, SqFrom::f, f>(fromSq);
            // perform overflow check if needed
            if constexpr (ovfCheckNeeded) {
                fpm::detail::checkOverflow<ovfBxOvrd, base_t>(value, Q::scaledMin, Q::scaledMax);
            }
            return value;
        }
    };

    /// Implements the conversion from a Q type to the related Sq typed.
    template< SqType SqTo, Overflow ovfBxOvrd >
    requires ( std::is_same_v<typename Q::base_t, typename SqTo::base_t> && Q::f == SqTo::f )
    struct ToSq {
        using base_t = typename SqTo::base_t;
        static constexpr scaling_t f = SqTo::f;
        static constexpr double realMin = SqTo::realMin;
        static constexpr double realMax = SqTo::realMax;
        // include overflow check if the value range of Sq is narrower
        static constexpr bool ovfCheckNeeded = (Q::scaledMin < SqTo::scaledMin || SqTo::scaledMax < Q::scaledMax);
        static constexpr bool innerConstraints = fpm::detail::OvfCheckAllowedWhenNeeded<ovfBxOvrd, ovfCheckNeeded>;
        static constexpr base_t value(typename Q::base_t qValue) noexcept {
            base_t sqValue = qValue;
            // perform overflow check if needed
            if constexpr (ovfCheckNeeded) {
                fpm::detail::checkOverflow<ovfBxOvrd, base_t>(sqValue, SqTo::scaledMin, SqTo::scaledMax);
            }
            return sqValue;
        }
    };

    /// Implements the cast of Q to type QC.
    template< QType QC >
    requires fpm::detail::Scalable<typename Q::base_t, Q::f, typename QC::base_t, QC::f>
    struct Cast {
        using base_t = typename QC::base_t;
        // cast type is twice the size of the target type to allow for proper cropping
        using cast_t = fpm::detail::fit_type_t<sizeof(base_t) * 2u, std::is_signed_v<base_t>>;
        static constexpr scaling_t f = QC::f;
        static constexpr double realMin = QC::realMin;
        static constexpr double realMax = QC::realMax;
        static constexpr Overflow ovfBx = QC::ovfBx;
        // include overflow check if value range of source type Q is not fully within range of target
        // type QC, or if different overflow properties could result in overflow if not checked
        //       cannot be compared so easily
        static constexpr bool ovfCheckNeeded = (Q::realMin < realMin || realMax < Q::realMax
                                                || is_ovf_stricter_v<ovfBx, Q::ovfBx>);
        static constexpr bool innerConstraints = fpm::detail::OvfCheckAllowedWhenNeeded<ovfBx, ovfCheckNeeded>;
        static constexpr base_t value(typename Q::base_t from) noexcept {
            // scale source value and cast it to the cast type with the sign of the target type
            auto cValue = s2s<cast_t, Q::f, f>(from);
            // perform overflow check if needed
            if constexpr (ovfCheckNeeded) {
                fpm::detail::checkOverflow<ovfBx, cast_t, typename Q::base_t>(cValue, QC::scaledMin, QC::scaledMax);
            }
            return static_cast<base_t>(cValue);
        }
    };

    /// Implements a static cast with overflow override from Q type to QC type.
    template< QType QC, Overflow ovfBxOvrd = QC::ovfBx >
    requires ( fpm::detail::Scalable<typename Q::base_t, Q::f, typename QC::base_t, QC::f>
               || (std::is_unsigned_v<typename QC::base_t> && ovfBxOvrd == Overflow::allowed) )
    struct StaticCast {
        using base_t = typename QC::base_t;
        // cast type is twice the size of the target type to allow for proper cropping
        using cast_t = fpm::detail::fit_type_t<sizeof(base_t) * 2u, std::is_signed_v<base_t>>;
        static constexpr scaling_t f = QC::f;
        static constexpr double realMin = QC::realMin;
        static constexpr double realMax = QC::realMax;
        static constexpr Overflow ovfBx = ovfBxOvrd;
        // include overflow check if value range of source type Q is not fully within range of target
        // type QC, or if different overflow properties could result in overflow if not checked
        static constexpr bool ovfCheckNeeded = (Q::realMin < QC::realMin || QC::realMax < Q::realMax
                                                || is_ovf_stricter_v<ovfBx, Q::ovfBx> || is_ovf_stricter_v<QC::ovfBx, ovfBxOvrd>);
        static constexpr bool innerConstraints = fpm::detail::OvfCheckAllowedWhenNeeded<ovfBx, ovfCheckNeeded>;
        static constexpr base_t value(typename Q::base_t from) noexcept {
            // scale source value and cast it to the cast type with the sign of the target type
            auto cValue = s2s<cast_t, Q::f, f>(from);
            // perform overflow check if needed
            if constexpr (ovfCheckNeeded) {
                fpm::detail::checkOverflow<ovfBx, cast_t, typename Q::base_t>(cValue, QC::scaledMin, QC::scaledMax);
            }
            // create target value
            return static_cast<base_t>(cValue);
        }
    };

    /// Implements a safe case from Q type to QC type. The overflow check is always performed.
    template< QType QC, Overflow ovfBxOvrd = QC::ovfBx >
    requires ( fpm::detail::Scalable<typename Q::base_t, Q::f, typename QC::base_t, QC::f>
               && ovfBxOvrd != Overflow::error && ovfBxOvrd != Overflow::unchecked )
    struct SafeCast {
        using base_t = typename QC::base_t;
        // cast type is twice the size of the target type to allow for proper cropping
        using cast_t = fpm::detail::fit_type_t<sizeof(base_t) * 2u, std::is_signed_v<base_t>>;
        static constexpr scaling_t f = QC::f;
        static constexpr double realMin = QC::realMin;
        static constexpr double realMax = QC::realMax;
        static constexpr Overflow ovfBx = ovfBxOvrd;
        static constexpr bool innerConstraints = true;
        static constexpr base_t value(typename Q::base_t from) noexcept {
            // scale source value and cast it to the cast type with the sign of the target type
            auto cValue = s2s<cast_t, Q::f, f>(from);
            // always perform overflow checks
            fpm::detail::checkOverflow<ovfBx, cast_t, typename Q::base_t>(cValue, QC::scaledMin, QC::scaledMax);
            // create target value
            return static_cast<base_t>(cValue);
        }
    };

public:
    /// Named "constructor" from a runtime variable (lvalue) or a constant (rvalue).
    /// \note Overflow check is always included unless explicitly discarded.
    template< Overflow ovfBxOvrd = ovfBx >
    static constexpr
    Q construct(base_t value) noexcept {
        fpm::detail::checkOverflow<ovfBxOvrd, base_t>(value, scaledMin, scaledMax);
        return Q( value );
    }

    /// Named COMPILE-TIME "constructor" from a floating-point value. This will use v2s to scale
    /// the given floating-point value at compile-time before the Q value is constructed with the
    /// scaled integer value.
    /// \note When the value is within bounds, no overflow check is needed. If it is out of range,
    ///       an overflow check is performed at compile-time according to the overflow settings.
    /// \note: When a real value is wrapped into a Q value, there is an inherent rounding error due
    /// to the limited resolution. This error is called 'representation error' and it refers to the
    /// deviation from the initial real value when the Q value is unscaled to a real value again.
    /// Usually the scaling error is in the order of the resolution of the Q type.
    template< double real, Overflow ovfBxOvrd = ovfBx >
    requires fpm::detail::ValidImplType< FromR<real, ovfBxOvrd> >
    static constexpr
    Q fromReal() { return Q::template construct< FromR<real, ovfBxOvrd>::ovfBx >( FromR<real, ovfBxOvrd>::value() ); }

    /// Named compile-time-only "constructor" from a scaled integer value. This can be used to
    /// construct a well-behaved Q value at compile-time without a redundant overflow check.
    /// \note When the value is within bounds, no overflow check is needed. If it is out of range,
    ///       an overflow check is performed at compile-time according to the overflow settings or
    ///       the specified overflow override.
    template< base_t scaled, Overflow ovfBxOvrd = ovfBx >
    requires fpm::detail::ValidImplType< FromS<scaled, ovfBxOvrd> >
    static constexpr
    Q fromScaled() { return Q::template construct< FromS<scaled, ovfBxOvrd>::ovfBx >( FromS<scaled, ovfBxOvrd>::value() ); }

    /// Named "Copy-Constructor" from another Q type with the same base type.
    /// \note When a Q value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the Q value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< Overflow ovfBxOvrd = ovfBx, /* deduced: */ QType QFrom >
    requires fpm::detail::ValidImplType< FromQ<QFrom, ovfBxOvrd> >
    static constexpr
    Q fromQ(QFrom const &from) noexcept { return Q( FromQ<QFrom, ovfBxOvrd>::value(from.value) ); }

    /// Named "constructor" from a Sq variable with the same base type.
    /// \note The representation error to be expected is comparable to that of Q::fromQ.
    /// \note An overflow check is included if the range of the Sq type is not entirely within the
    /// range of the Q type.
    template< Overflow ovfBxOvrd = ovfBx, /* deduced: */ SqType SqFrom >
    requires fpm::detail::ValidImplType< FromSq<SqFrom, ovfBxOvrd> >
    static constexpr
    Q fromSq(SqFrom const &fromSq) noexcept { return Q( FromSq<SqFrom, ovfBxOvrd>::value(fromSq.value) ); }

    /// Explicit conversion to the related Sq type.
    /// \note If the limits are the same and the overflow behavior is not overridden, the unary plus
    /// operator can be applied to the q value instead to achieve the same conversion.
    template< double realMinSq = realMin, double realMaxSq = realMax, Overflow ovfBxOvrd = ovfBx >
    requires fpm::detail::ValidImplType< ToSq< Sq<realMinSq, realMaxSq>, ovfBxOvrd > >
    constexpr
    auto toSq() const noexcept {
        return Sq<realMinSq, realMaxSq>( ToSq< Sq<realMinSq, realMaxSq>, ovfBxOvrd >::value(this->value) );
    }

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
    template< /* deduced: */ QType QFrom >
    requires requires(QFrom const &from) {
        !std::is_same_v< Q, QFrom >;  // when the same, default copy constructor should be used
        { Q::template fromQ<ovfBx>(from) } -> std::same_as<Q>;
    }
    constexpr
    Q(QFrom const &from) noexcept : value( Q::template fromQ<ovfBx>(from).value ) {}

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
    requires requires(QFrom const &from) {
        !std::is_same_v< Q, QFrom >;  // when the same, default copy constructor should be used
        { Q::template fromQ<ovfBx>(from) } -> std::same_as<Q>;
    }
    constexpr
    Q& operator =(QFrom const &from) noexcept {
        value = fromQ(from).value;
        return *this;
    }

    /// Explicit (static-) cast to a different Q type with a potentially different base type.
    /// \note If a cast does not work it's most probably due to unfulfilled requirements. Double check
    ///       whether a runtime overflow check is needed and make sure that it is allowed!
    template< /* deduced: */ QType QC >
    requires fpm::detail::ValidImplType< Cast<QC> >
    explicit constexpr
    operator QC() const noexcept {
        // discard overflow check when QC is constructed to avoid that value is checked again
        return QC::template construct<Overflow::unchecked>( Cast<QC>::value(this->value) );
    }

    /// Explicit (static-) cast to another Q type with a potentially different base type.
    /// Allows to override the default overflow behavior.
    template< QType QC, Overflow ovfBxOvrd = QC::ovfBx >
    requires fpm::detail::ValidImplType< StaticCast<QC, ovfBxOvrd> >
    friend constexpr
    QC static_q_cast(Q const &from) noexcept {
        // static_cast can be reused if overflow is not overwritten
        if constexpr (ovfBxOvrd == QC::ovfBx) { return static_cast<QC>(from); }
        // if overflow override is specified, use StaticCast implementation type
        else {
            // discard overflow check when QC is constructed to avoid that value is checked again
            return QC::template construct<Overflow::unchecked>( StaticCast<QC, ovfBxOvrd>::value(from.value) );
        }
    }

    /// Explicit, safe cast of a Q type to another Q type with a potentially different base type.
    /// This will always perform overflow checks, even if not necessarily needed.
    /// \note Overflow::allowed is not possible - use static_cast instead.
    template< QType QC, Overflow ovfBxOvrd = QC::ovfBx >
    requires fpm::detail::ValidImplType< SafeCast<QC, ovfBxOvrd> >
    friend constexpr
    QC safe_q_cast(Q const &from) noexcept {
        // discard overflow check when QC is constructed to avoid that value is checked again
        return QC::template construct<Overflow::unchecked>( SafeCast<QC, ovfBxOvrd>::value(from.value) );
    }

    /// Explicit, force cast of a Q type to a different Q type.
    /// This will simply reinterpret the value of the source Q type as a value of the target Q type
    /// without performing any scaling or overflow checks at all (overflow attributes are all ignored).
    template< QType QC >
    friend constexpr
    QC force_q_cast(Q const &from) noexcept {
        return QC::template construct<Overflow::unchecked>( static_cast<typename QC::base_t>(from.value) );
    }

    /// Reveals the scaled integer value stored in memory.
    constexpr
    base_t scaled() const noexcept { return value; }

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
    TargetT real() const noexcept { return fpm::real<base_t, f, TargetT>(value); }
    /// Implicit conversion of a Q value back into its double representation. Allows using a
    /// value+unit literal where a double is expected. Compile-time only!
    consteval
    operator double() const noexcept { return real<double>(); }

private:
    // delete default (runtime) constructor
    Q() = delete;

    /// Explicit, possibly compile-time constructor from integer value.
    explicit constexpr
    Q(base_t value) noexcept : value(value) {}

    // friend all Q types so that private members of similar types can be accessed for construction
    // Note: As of May 2023, partial specializations cannot be friended, so we friend Q in general.
    template< std::integral BaseTQ, scaling_t fQ, double realMinQ, double realMaxQ, Overflow ovfQ >
    requires fpm::detail::QRequirements<BaseTQ, fQ, realMinQ, realMaxQ, ovfQ>
    friend class Q;

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
    return Q::template clamp_t<value, value>::template fromReal<value>();
}
/// Associates a Q type with a literal.
#define FPM_Q_BIND_LITERAL(_q, _literal) \
    template< char ...chars > consteval auto operator "" ## _ ## _literal () { return fpm::q::fromLiteral<_q, chars...>(); }

/**\}*/
}  // end of fpm::q

namespace std {

/// Provides the bare, real numeric limits for the given Q type.
template< /* deduced: */ std::integral BaseT, fpm::scaling_t f, double realMin, double realMax, fpm::Overflow ovfBx >
class numeric_limits<fpm::q::Q<BaseT, f, realMin, realMax, ovfBx>> {
    using QT = fpm::q::Q<BaseT, f, realMin, realMax, ovfBx>;
public:
    /// \returns the minimum real value that can be represented by the Q type.
    /// \note In contrast to Q::realMin, this does not return the minimum value specified by the
    /// user, but the absolute minimum that can be represented by the underlying Q type with respect
    /// to its base type and scaling. This can be significantly smaller that the actual user minimum.
    template< typename T = double >
    static constexpr T min() noexcept {
        return fpm::real<typename QT::base_t, QT::f, T>( numeric_limits<typename QT::base_t>::min() );
    }

    /// \returns the maximum real value that can be represented by the Q type.
    /// \note In contrast to Q::realMax, this does not return the maximum value specified by the
    /// user, but the absolute maximum that can be represented by the underlying Q type with respect
    /// to its base type and scaling. This can be significantly larger that the actual user maximum.
    template< typename T = double >
    static constexpr T max() noexcept {
        return fpm::real<typename QT::base_t, QT::f, T>( numeric_limits<typename QT::base_t>::max() );
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
