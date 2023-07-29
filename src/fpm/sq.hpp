/** \file
 * Implementation of the sq class template.
 */

#ifndef FPM_FPM_SQ_HPP_
#define FPM_FPM_SQ_HPP_

#include "fpm.hpp"


/** \addtogroup grp_fpm
 * \{ */
namespace fpm {

/// Concept of a sq-like type.
/// \warning Does not guarantee that T is actually of type sq. Only checks for the basic properties.
template< class T >
concept SqType = requires (T t) {
    { std::bool_constant<T::isSqType>() } -> std::same_as<std::true_type>;
    typename T::base_t;
    std::is_integral_v<typename T::base_t>;
    std::is_same_v<scaling_t, decltype(T::f)>;
    std::is_same_v<double, decltype(T::realVMin)>;
    std::is_same_v<double, decltype(T::realVMax)>;
    std::is_same_v<typename T::base_t, decltype(T::vMin)>;
    std::is_same_v<typename T::base_t, decltype(T::vMax)>;
    std::is_same_v<double, decltype(T::resolution)>;
    { t.reveal() } -> std::same_as<typename T::base_t>;
};

/// Static (safe) q type. Implements mathematical operations and checks at compile-time whether these
/// operations can be performed for the specified value range without running into overflow issues.
/// \note Usually sq objects are constructed indirectly via q objects, but direct construction from
/// a real or scaled constexpr value is also possible.
/// \warning Two sq types which differ only by the sign of the 0 in a limit are not equal by design,
/// although the underlying integral value will be the same when 0. This fact is not corrected for,
/// because the outcome might be different (wrong) when the compiler is doing its calculations.
/// It's recommended to use -0 when a type is declared that has only negative numbers in its range.
template<
    std::integral BaseT,  ///< type of the scaled integer stored in memory
    scaling_t f_,         ///< number of fraction bits (precision 2^-f)
    double realVMin_ = details::lowestRealVMin<BaseT, f_>,  ///< minimum real value represented by this type
    double realVMax_ = details::highestRealVMax<BaseT, f_>  ///< maximum real value represented by this type
>
requires (
    details::ValidBaseType<BaseT>
    && details::ValidScaling<BaseT, f_>
    && details::RealLimitsInRangeOfBaseType<BaseT, f_, realVMin_, realVMax_>
)
class sq final {
public:
    static constexpr bool isSqType = true;  ///< identifier for the SqType concept
    using base_t = BaseT;  ///< integral base type
    static constexpr scaling_t f = f_;  ///< number of fraction bits
    static constexpr double realVMin = realVMin_;  ///< minimum real value
    static constexpr double realVMax = realVMax_;  ///< maximum real value
    static constexpr base_t vMin = v2s<base_t, f>(realVMin_);  ///< minimum value of integer value range
    static constexpr base_t vMax = v2s<base_t, f>(realVMax_);  ///< maximum value of integer value range
    static constexpr double resolution = v2s<double, -f>(1);  ///< real resolution of this type

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
    requires ( realVMin <= realValue && realValue <= realVMax )  // must not overflow
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
    template< /* deduced: */ SqType SqFrom >
    requires (
        std::is_same_v<base_t, typename SqFrom::base_t>
        // scaling is only possible if the f difference allows scaling and the real value
        // can be represented by the target type
        && details::ScalingIsPossible<base_t, SqFrom::f, base_t, f>
        && realVMin <= SqFrom::realVMin && SqFrom::realVMax <= realVMax
    )
    static constexpr
    sq fromSq(SqFrom const &from) noexcept {
        return sq( s2s<base_t, SqFrom::f, f>(from.reveal()) );
    }

    /// Copy-Constructor from another sq type value with the same base type.
    /// Same as sq::fromSq(), however a bit stricter (sq types have to be different).
    /// \note When an sq value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the sq value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< /* deduced: */ SqType SqFrom >
    requires (
        !std::is_same_v< sq, SqFrom >  // when the same, default copy constructor should be used
        && std::is_same_v<base_t, typename SqFrom::base_t>
        // scaling is only possible if the f difference allows scaling and the real value
        // can be represented by the target type
        && details::ScalingIsPossible<base_t, SqFrom::f, base_t, f>
        && realVMin <= SqFrom::realVMin && SqFrom::realVMax <= realVMax
    )
    constexpr sq(SqFrom const &from) noexcept : value( s2s<base_t, SqFrom::f, f>(from.reveal()) ) {}

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
    template< /* deduced: */ std::integral BaseTC, scaling_t fC, double realVMinC, double realVMaxC >
    requires (
        !std::is_same_v<base_t, BaseTC>
        // scaling is only possible if the f difference allows scaling and the real value
        // can be represented by the target type
        && details::ScalingIsPossible<base_t, f, BaseTC, fC>
        && realVMinC <= realVMin && realVMax <= realVMaxC
    )
    explicit constexpr
    operator sq<BaseTC, fC, realVMinC, realVMaxC>() const noexcept {
        using SqC = sq<BaseTC, fC, realVMinC, realVMaxC>;

        // scale value
        auto cValue = s2s<typename SqC::base_t, f, SqC::f>(value);

        // create target sq
        return SqC(cValue);
    }

    /// Unary plus operator. Integral promotion does not make any sense, so this just creates a copy.
    /// \returns a copy of the value with the same type.
    constexpr
    sq operator+() const noexcept {
        return sq( value );
    }

    /// Adds two sq values.
    /// \returns a value of a new sq type with the larger scaling (higher precision) and the user value
    /// ranges added together. If the base types are different, integral promotion rules will be applied.
    template< /* deduced: */ SqType SqRhs,
        // common type is larger type, or unsigned type if same size, or type if same types
        std::integral BaseTR = std::common_type_t<base_t, typename SqRhs::base_t>, scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = realVMin + SqRhs::realVMin, double realVMaxR = realVMax + SqRhs::realVMax >
    requires details::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a+b+c.
    auto operator+(sq const lhs, SqRhs const &rhs) noexcept {
        using SqResult = sq<BaseTR, fR, realVMinR, realVMaxR>;

        // add values
        auto result = s2s<BaseTR, f, SqResult::f>(lhs.value) + s2s<BaseTR, SqRhs::f, SqResult::f>(rhs.reveal());

        return SqResult( static_cast<SqResult::base_t>(result) );
    }

    /// Unary minus operator. Negates this type and its value.
    /// \note A signed type can be negated if the corresponding INT_MIN value is not in the value
    /// range of the type. An unsigned type is promoted to a new type with a sized base integer that
    /// has twice the size (e.g. u16 is promoted to i32).
    /// \warning a +0.0 in the limits will be negated to -0.0 and vice versa. Note that two sq types
    ///          which differ only by the sign of the 0 in a limit are not equal by design!
    /// \returns a value of a new sq type with negated limits and value.
    template< /* deduced: */ double realVMinR = -realVMax, double realVMaxR = -realVMin,
        std::integral BaseTR = std::conditional_t< std::is_signed_v<base_t>,
            base_t,  // signed type stays
            // unsigned type is promoted to signed type with twice the size
            details::fit_signed_t<base_t, std::numeric_limits<base_t>::max()> > >
    requires details::CanAbsolutize<base_t, vMin>
    constexpr
    auto operator-() const noexcept {
        using SqResult = sq<BaseTR, f, realVMinR, realVMaxR>;
        return SqResult( static_cast<typename SqResult::base_t>(-value) );
    }

    /// Subtracts the rhs value from the lhs value.
    /// \returns a value of a new sq type with the larger scaling (higher precision) and the user value
    /// ranges subtracted. If the base types are different, integral promotion rules will be applied.
    template< /* deduced: */ SqType SqRhs,
        // common type is larger type, or unsigned type if same size, or type if same types
        std::integral BaseTR = std::common_type_t<base_t, typename SqRhs::base_t>, scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = std::min(realVMin - SqRhs::realVMax, SqRhs::realVMin - realVMax),
        double realVMaxR = std::max(realVMax - SqRhs::realVMin, SqRhs::realVMax - realVMin) >
    requires details::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a-b-c.
    auto operator-(sq const lhs, SqRhs const &rhs) noexcept {
        using SqResult = sq<BaseTR, fR, realVMinR, realVMaxR>;

        // subtract rhs value from lhs value
        auto result = s2s<BaseTR, f, SqResult::f>(lhs.value) - s2s<BaseTR, SqRhs::f, SqResult::f>(rhs.reveal());

        return SqResult( static_cast<SqResult::base_t>(result) );
    }

    /// Multiplies the lhs value with the rhs value.
    /// \returns a value of a new sq type with the larger scaling (higher precision) and the user value
    /// ranges multiplied. If the base types are different, integral promotion rules will be applied.
    /// \note The error propagation is complicated. When a number x is multiplied with itself n times,
    ///       the real error is of order O( n * x^(n-1) * 2^-q ). For example, for a chain x*x*x
    ///       the real error is of order O( 3*x^2 * 2^-q ). Higher terms O( 2^-mq ), m > 1 do occur
    ///       for such chains, but are very close to 0 for larger q and can usually be ignored.
    template< /* deduced: */ SqType SqRhs,
        // common type is larger type, or unsigned type if same size, or type if same types
        std::integral BaseTR = std::common_type_t<base_t, typename SqRhs::base_t>, scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = std::min(std::min(realVMin * SqRhs::realVMax, SqRhs::realVMin * realVMax),
                                    std::min(realVMin * SqRhs::realVMin, realVMax * SqRhs::realVMax)),
        double realVMaxR = std::max(std::max(realVMax * SqRhs::realVMin, SqRhs::realVMax * realVMin),
                                    std::max(realVMin * SqRhs::realVMin, realVMax * SqRhs::realVMax)) >
    requires details::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a*b*c.
    auto operator*(sq const lhs, SqRhs const &rhs) noexcept {
        using SqResult = sq<BaseTR, fR, realVMinR, realVMaxR>;
        using interm_m_t = interm_t<BaseTR>;

        // multiply lhs with rhs in intermediate type and correct scaling to obtain result
        auto intermediate = s2s<interm_m_t, f, SqResult::f>(lhs.value) * s2s<interm_m_t, SqRhs::f, SqResult::f>(rhs.reveal());
        auto result = s2s<typename SqResult::base_t, 2*SqResult::f, SqResult::f>(intermediate);
        return SqResult( result );
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
    // delete undesired special members
    sq() = delete;  // default constructor
    sq& operator=(sq const &) = delete;  // copy-assignment
    sq& operator=(sq&&) = delete;  // move-assignment

    /// Explicit, possibly compile-time constructor from scaled integer value.
    explicit constexpr sq(base_t value) noexcept : value(value) {}

    // friend all sq types so that private members of similar types can be accessed for construction
    template< std::integral BaseTSq, scaling_t fSq, double realVMinSq, double realVMaxSq >
    requires (
        details::ValidBaseType<BaseTSq>
        && details::ValidScaling<BaseTSq, fSq>
        && details::RealLimitsInRangeOfBaseType<BaseTSq, fSq, realVMinSq, realVMaxSq>
    )
    friend class sq;

    // friend q type so that it can access the private members of this type to construct it
    // Note: As of May 2023, partial specializations cannot be friended, so we friend q in general.
    template< std::integral BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
    requires (
        details::ValidBaseType<BaseTQ>
        && details::ValidScaling<BaseTQ, fQ>
        && details::RealLimitsInRangeOfBaseType<BaseTQ, fQ, realVMinQ, realVMaxQ>
    )
    friend class q;

    // friend abs() function so that it can access the private members of this type to construct new
    // variants of it
    template< SqType Sq, std::integral BaseTR, double realVMinR, double realVMaxR >
    requires details::CanAbsolutize<typename Sq::base_t, Sq::vMin>
    friend constexpr
    auto abs(Sq const &sqValue) noexcept;

    /// scaled integer value that represents a fixed-point value; stored in memory
    base_t const value;
};

/// Explicit static cast to another sq type with a different base type.
/// Uses static_cast internally. Exists for consistency reasons.
template< SqType SqC, /* deduced: */ SqType SqFrom >
requires (
    !std::is_same_v<typename SqFrom::base_t, typename SqC::base_t>
    // scaling is only possible if the f difference allows scaling and the real value
    // can be represented by the target type
    && details::ScalingIsPossible<typename SqFrom::base_t, SqFrom::f, typename SqC::base_t, SqC::f>
    && SqC::realVMin <= SqFrom::realVMin && SqFrom::realVMax <= SqC::realVMax
)
constexpr
SqC static_sq_cast(SqFrom from) noexcept {
    return static_cast<SqC>(from);
}

/// Explicit safe cast to another sq type with a different base type.
/// Uses static_cast internally. Exists for consistency reasons.
template< SqType SqC, /* deduced: */ SqType SqFrom >
requires (
    !std::is_same_v<typename SqFrom::base_t, typename SqC::base_t>
    // scaling is only possible if the f difference allows scaling and the real value
    // can be represented by the target type
    && details::ScalingIsPossible<typename SqFrom::base_t, SqFrom::f, typename SqC::base_t, SqC::f>
    && SqC::realVMin <= SqFrom::realVMin && SqFrom::realVMax <= SqC::realVMax
)
constexpr
SqC safe_sq_cast(SqFrom from) noexcept {
    return static_cast<SqC>(from);
}

/// Takes the absolute value of the given sq value.
/// \note The absolute value of a signed type can be taken if the corresponding INT_MIN value is
/// not in the value range of the sq type.
/// \returns a new unsigned sq type with a modified range that holds the absolute value.
template< /* deduced: */ SqType Sq,
    std::integral BaseTR = std::make_unsigned_t<typename Sq::base_t>,
    double realVMinR = (std::is_signed_v<typename Sq::base_t> && Sq::vMin < 0 && Sq::vMax > 0)
        ? 0.0  // use 0 as new minimum if signed input type has a range of negative and positive values
        : std::min(details::abs((double)Sq::realVMin), details::abs((double)Sq::realVMax)),
    double realVMaxR = std::max(details::abs((double)Sq::realVMin), details::abs((double)Sq::realVMax)) >
requires details::CanAbsolutize<typename Sq::base_t, Sq::vMin>
constexpr
auto abs(Sq const &of) noexcept {
    return sq<BaseTR, Sq::f, realVMinR, realVMaxR>( std::abs(of.value) );
}

/// Converts a literal number into the corresponding best-fit sq type.
/// Best-fit means that the literal number represents both limits and the value.
template< std::integral BaseT, scaling_t f, char ...charArray >
consteval auto sqFromLiteral() {
    constexpr double value = details::doubleFromLiteral<charArray...>();
    return sq<BaseT, f, value, value>::template fromReal<value>;
}

}  // end of fpm
/**\}*/

namespace std {

/// Provides the bare, real numeric limits for the given sq type.
template< /* deduced: */ std::integral BaseT, fpm::scaling_t f, double realVMin, double realVMax >
class numeric_limits<fpm::sq<BaseT, f, realVMin, realVMax>> {
    using SqType = fpm::sq<BaseT, f, realVMin, realVMax>;
public:
    /// \returns the minimum real value that can be represented by the sq type.
    /// \note In contrast to sq::realVMin, this does not return the minimum value specified by the
    /// user, but the absolute minimum that can be represented by the underlying sq type with respect
    /// to its base type and scaling. This can be significantly smaller that the actual user minimum.
    template< typename T = double >
    static constexpr T min() noexcept {
        return fpm::v2s<T, -SqType::f>( numeric_limits<typename SqType::base_t>::min() );
    }

    /// \returns the maximum real value that can be represented by the sq type.
    /// \note In contrast to sq::realVMax, this does not return the maximum value specified by the
    /// user, but the absolute maximum that can be represented by the underlying sq type with respect
    /// to its base type and scaling. This can be significantly larger that the actual user maximum.
    template< typename T = double >
    static constexpr T max() noexcept {
        return fpm::v2s<T, -SqType::f>( numeric_limits<typename SqType::base_t>::max() );
    }

    constexpr static bool is_specialized = true;
    constexpr static bool is_signed = numeric_limits<typename SqType::base_t>::is_signed;
    constexpr static bool is_bounded = true;
    constexpr static bool traps = true;
#   if defined FPM_USE_SH
    constexpr static auto round_style = std::round_toward_neg_infinity;
#   else
    constexpr static auto round_style = std::round_toward_zero;
#   endif
    constexpr static int radix = numeric_limits<typename SqType::base_t>::radix;
    constexpr static int digits = numeric_limits<typename SqType::base_t>::digits - SqType::f;
    constexpr static int digits10 = static_cast<int>( std::log10(radix) * digits );
};

}

#endif
// EOF
