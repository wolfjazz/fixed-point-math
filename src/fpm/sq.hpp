/** \file
 * Implementation of the sq class template.
 */

#ifndef _FPM___SQ_HPP_
#define _FPM___SQ_HPP_

#include "fpm.hpp"


/** \addtogroup grp_fpm
 * \{ */
namespace fpm {

/// Static (safe) q type. Implements mathematical operations and checks at compile-time whether these
/// operations can be performed for the specified value range without running into overflow issues.
/// \note sq values cannot be constructed directly at runtime; only indirect construction via q types
///       is possible in this case.
template<
    typename BaseT,  ///< type of the scaled integer stored in memory
    scaling_t f_,    ///< number of fraction bits (precision 2^-f)
    double realVMin_ = v2s<double, -f_>((double)std::numeric_limits<BaseT>::min()),  ///< minimum real value represented by this type
    double realVMax_ = v2s<double, -f_>((double)std::numeric_limits<BaseT>::max())   ///< maximum real value represented by this type
>
requires (
    ValidBaseType<BaseT>
    && ValidScaling<BaseT, f_>
    && RealLimitsInRangeOfBaseType<BaseT, f_, realVMin_, realVMax_>
)
class sq final {
public:
    using base_t = BaseT;  /// integral base type
    static constexpr scaling_t f = f_;  ///< number of fraction bits
    static constexpr double realVMin = realVMin_;  ///< minimum real value
    static constexpr double realVMax = realVMax_;  ///< maximum real value
    static constexpr base_t vMin = v2s<base_t, f>(realVMin_);  ///< minimum value of integer value range
    static constexpr base_t vMax = v2s<base_t, f>(realVMax_);  ///< maximum value of integer value range
    static constexpr double resolution = v2s<double, -f>(1);  ///< real resolution of this type

    // friend all sq types so that private members of similar types can be accessed for construction
    template< typename BaseTSq, scaling_t fSq, double realVMinSq, double realVMaxSq >
    requires (
        ValidBaseType<BaseTSq>
        && ValidScaling<BaseTSq, fSq>
        && RealLimitsInRangeOfBaseType<BaseTSq, fSq, realVMinSq, realVMaxSq>
    )
    friend class sq;

    // friend q type so that it can access the private members of this type to construct it
    // Note: As of May 2023, partial specializations cannot be friended, so we friend q in general.
    template< typename BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
    requires (
        ValidBaseType<BaseTQ>
        && ValidScaling<BaseTQ, fQ>
        && RealLimitsInRangeOfBaseType<BaseTQ, fQ, realVMinQ, realVMaxQ>
    )
    friend class q;

    /// Create a new sq type with the same base type and scaling but a different real value range.
    template< double newRealVMin, double newRealVMax >
    struct relimit { using type = sq< base_t, f, newRealVMin, newRealVMax >; };

    /// Type alias for relimit::type.
    template< double newRealVMin, double newRealVMax >
    using relimit_t = relimit<newRealVMin, newRealVMax>::type;

    /// Named compile-time-only "constructor" from a floating-point value. This will use v2s to scale
    /// the given floating-point value at compile-time before the sq value is constructed with the
    /// scaled integer value.
    /// \note: When a real value is wrapped into an sq value, there is an inherent rounding error due
    /// to the limited resolution. This error is called 'representation error' and it refers to the
    /// deviation from the initial real value when an sq value is unscaled to a real value again.
    /// Usually the representation error is in the order of the resolution of the sq type.
    /// \note Does not compile if the value is outside the value range.
    template< double realValue >
    static constexpr
    sq fromReal = sq( v2s<base_t, f>(realValue) );

    /// Named compile-time-only "constructor" from a scaled integer value.
    /// \note Does not compile if the value is outside the value range.
    template< base_t value >
    requires ( vMin <= value && value <= vMax )  // must not overflow
    static constexpr
    sq fromScaled = sq( value );

    /// Named "Copy-Constructor" from another sq type value with the same base type.
    /// \note When an sq value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the sq value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< double realVMinFrom, double realVMaxFrom, scaling_t fFrom >
    requires (
        // rescaling is only possible if the real value can be represented (must not overflow)
        realVMin <= realVMinFrom && realVMaxFrom <= realVMax
    )
    static constexpr
    sq fromSq(sq<base_t, fFrom, realVMinFrom, realVMaxFrom> const &from) noexcept {
        return sq( s2s<base_t, fFrom, f>(from.reveal()) );
    }

    /// Copy-Constructor from the same type.
    constexpr sq(sq const &) noexcept = default;

    /// Move-Constructor from the same type.
    constexpr sq(sq&&) noexcept = default;

    /// Destructor.
    constexpr ~sq() {}

    /// Explicit static and safe cast to a different sq type with a different base type.
    /// Only possible if the value can be cast safely without any potential overflow, i.e. if the
    /// target value range is equal to or larger than the value range of this class.
    /// \note If a cast does not work it's most probably due to unfulfilled requirements.
    template< typename BaseTC, scaling_t fC, double realVMinC, double realVMaxC >
    requires (
        !std::is_same_v<base_t, BaseTC>
        // scaling is only possible if the f difference allows scaling and the real value
        // can be represented by the target type
        && ScalingIsPossible<base_t, f, BaseTC, fC>
        && realVMinC <= realVMin && realVMax <= realVMaxC
    )
    explicit
    operator sq<BaseTC, fC, realVMinC, realVMaxC>() const noexcept {
        using target_sq = sq<BaseTC, fC, realVMinC, realVMaxC>;

        // scale value
        auto cValue = s2s<typename target_sq::base_t, f, target_sq::f>(value);

        // create target sq
        return target_sq(cValue);
    }

    /// Adds two sq values.
    /// \returns a value of a new sq type with the larger scaling (higher precision) and the user value
    /// ranges added together. If the base types are different, integral promotion rules will be applied.
    template< class SqRhs,
        typename BaseTRhs = SqRhs::base_t, scaling_t fRhs = SqRhs::f,
        double realVMinRhs = SqRhs::realVMin, double realVMaxRhs = SqRhs::realVMax,
        // common type is larger type, or unsigned type if same size, or type if same types
        typename BaseTR = std::common_type_t<base_t, BaseTRhs>,
        scaling_t fR = _i::max(fRhs, f),
        double realVMinR = realVMin + realVMinRhs,
        double realVMaxR = realVMax + realVMaxRhs >
    requires (
        RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    )
    friend constexpr
    sq<BaseTR, fR, realVMinR, realVMaxR>
    // Note: Passing lhs by value helps optimize chained a+b+c.
    operator+(sq const lhs, SqRhs const &rhs) noexcept {
        using rhs_sq = sq<BaseTRhs, fRhs, realVMinRhs, realVMaxRhs>;
        using result_sq = sq<BaseTR, fR, realVMinR, realVMaxR>;

        // add values
        auto result = s2s<BaseTR, f, result_sq::f>(lhs.value) + s2s<BaseTR, rhs_sq::f, result_sq::f>(rhs.reveal());
        return result_sq( static_cast<result_sq::base_t>(result) );
    }

    /// Subtracts the rhs value from the lhs value.
    /// \returns a value of a new sq type with the larger scaling (higher precision) and the user value
    /// ranges subtracted. If the base types are different, integral promotion rules will be applied.
    template< class SqRhs,
        typename BaseTRhs = SqRhs::base_t, scaling_t fRhs = SqRhs::f,
        double realVMinRhs = SqRhs::realVMin, double realVMaxRhs = SqRhs::realVMax,
        // common type is larger type, or unsigned type if same size, or type if same types
        typename BaseTR = std::common_type_t<base_t, BaseTRhs>,
        scaling_t fR = _i::max(fRhs, f),
        double realVMinR = _i::min(realVMin - realVMaxRhs, realVMinRhs - realVMax),
        double realVMaxR = _i::max(realVMax - realVMinRhs, realVMaxRhs - realVMin) >
    requires (
        RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    )
    friend constexpr
    sq<BaseTR, fR, realVMinR, realVMaxR>
    // Note: Passing lhs by value helps optimize chained a-b-c.
    operator-(sq const lhs, SqRhs const &rhs) noexcept {
        using rhs_sq = sq<BaseTRhs, fRhs, realVMinRhs, realVMaxRhs>;
        using result_sq = sq<BaseTR, fR, realVMinR, realVMaxR>;

        // subtract rhs value from lhs value
        auto result = s2s<BaseTR, f, result_sq::f>(lhs.value) - s2s<BaseTR, rhs_sq::f, result_sq::f>(rhs.reveal());
        return result_sq( static_cast<result_sq::base_t>(result) );
    }

    /// Multiplies the lhs value with the rhs value.
    /// \returns a value of a new sq type with the larger scaling (higher precision) and the user value
    /// ranges multiplied. If the base types are different, integral promotion rules will be applied.
    /// \note The error propagation is complicated. When a number x is multiplied with itself n times,
    ///       the real error is of order O( n * x^(n-1) * 2^-q ). For example, for a chain x*x*x
    ///       the real error is of order O( 3*x^2 * 2^-q ). Higher terms O( 2^-mq ), m > 1 do occur
    ///       for such chains, but are very close to 0 for larger q and can usually be ignored.
    template< class SqRhs,
        typename BaseTRhs = SqRhs::base_t, scaling_t fRhs = SqRhs::f,
        double realVMinRhs = SqRhs::realVMin, double realVMaxRhs = SqRhs::realVMax,
        // common type is larger type, or unsigned type if same size, or type if same types
        typename BaseTR = std::common_type_t<base_t, BaseTRhs>,
        scaling_t fR = _i::max(fRhs, f),
        double realVMinR = _i::min(_i::min(realVMin * realVMaxRhs, realVMinRhs * realVMax),
                                       _i::min(realVMin * realVMinRhs, realVMax * realVMaxRhs)),
        double realVMaxR = _i::max(_i::max(realVMax * realVMinRhs, realVMaxRhs * realVMin),
                                       _i::max(realVMin * realVMinRhs, realVMax * realVMaxRhs)) >
    requires (
        RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    )
    friend constexpr
    sq<BaseTR, fR, realVMinR, realVMaxR>
    // Note: Passing lhs by value helps optimize chained a*b*c.
    operator*(sq const lhs, SqRhs const &rhs) noexcept {
        using rhs_sq = sq<BaseTRhs, fRhs, realVMinRhs, realVMaxRhs>;
        using result_sq = sq<BaseTR, fR, realVMinR, realVMaxR>;
        using interm_m_t = interm_t<BaseTR>;

        // multiply lhs with rhs in intermediate type and correct scaling to obtain result
        auto intermediate = s2s<interm_m_t, f, result_sq::f>(lhs.value) * s2s<interm_m_t, rhs_sq::f, result_sq::f>(rhs.reveal());
        auto result = s2s<typename result_sq::base_t, 2*result_sq::f, result_sq::f>(intermediate);
        return result_sq( result );
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
    // delete undesired special members
    sq() = delete;  // default constructor
    sq& operator=(sq const &) = delete;  // copy-assignment
    sq& operator=(sq&&) = delete;  // move-assignment

    /// Explicit, possibly compile-time constructor from scaled integer value.
    explicit constexpr sq(base_t value) noexcept : value(value) {}

    /// scaled integer value that represents a fixed-point value; stored in memory
    base_t const value;
};

/// Explicit static cast to another sq type with a different base type.
/// Uses static_cast internally. Exists for consistency reasons.
template< class SqC,
    typename BaseT, scaling_t f, double realVMin, double realVMax,
    typename BaseTC = SqC::base_t, scaling_t fC = SqC::f, double realVMinC = SqC::realVMin,
    double realVMaxC = SqC::realVMax >
requires (
    !std::is_same_v<BaseT, BaseTC>
    // scaling is only possible if the f difference allows scaling and the real value
    // can be represented by the target type
    && ScalingIsPossible<BaseT, f, BaseTC, fC>
    && realVMinC <= realVMin && realVMax <= realVMaxC
)
constexpr
SqC static_sq_cast(sq<BaseT, f, realVMin, realVMax> from) noexcept {
    return static_cast<SqC>(from);
}

/// Explicit safe cast to another sq type with a different base type.
/// Uses static_cast internally. Exists for consistency reasons.
template< class SqC,
    typename BaseT, scaling_t f, double realVMin, double realVMax,
    typename BaseTC = SqC::base_t, scaling_t fC = SqC::f, double realVMinC = SqC::realVMin, double realVMaxC = SqC::realVMax >
requires (
    !std::is_same_v<BaseT, BaseTC>
    // scaling is only possible if the f difference allows scaling and the real value
    // can be represented by the target type
    && ScalingIsPossible<BaseT, f, BaseTC, fC>
    && realVMinC <= realVMin && realVMax <= realVMaxC
)
constexpr
SqC safe_sq_cast(sq<BaseT, f, realVMin, realVMax> from) noexcept {
    return static_cast<SqC>(from);
}

/// Converts a literal integer into the corresponding best-fit sq type.
/// Best-fit means that the literal integer represents both limits and the value.
template< typename BaseT, scaling_t f, char ...charArray >
consteval auto sqFromLiteral() {
    constexpr std::size_t length = sizeof...(charArray);
    constexpr char chars[length]{ charArray... };
    static_assert(std::all_of(chars, chars + length, [](char c) { return isdigit(c); }), "The argument to _sq must be a positive integer");
    constexpr double value = static_cast<double>( _i::charArrayTo<BaseT, length>(chars) );
    return sq<BaseT, f, value, value>::template fromReal<value>;
}

}
/**\}*/

#endif
// EOF
