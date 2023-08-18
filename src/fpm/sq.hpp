/** \file
 * Implementation of the Sq class template.
 */

#ifndef FPM_FPM_SQ_HPP_
#define FPM_FPM_SQ_HPP_

#include "fpm.hpp"


// forward declare fpm::q::Q so that it can be friended by fpm::sq::Sq
namespace fpm::q {
template< std::integral BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
requires details::QRequirements<BaseTQ, fQ, realVMinQ, realVMaxQ, ovfQ>
class Q;
}


namespace fpm::sq {
/** \ingroup grp_fpm
 * \defgroup grp_fpmSq Static Q-Type
 * \{ */

using details::SqType;

/// Static (safe) Q type. Implements mathematical operations and checks at compile-time whether these
/// operations can be performed for the specified value range without running into overflow issues.
/// \note Usually Sq objects are constructed indirectly via Q objects, but direct construction from
/// a real or scaled constexpr value is also possible.
/// \warning Two Sq types which differ only by the sign of the 0 in a limit are not equal by design,
///          although the underlying integral value will be the same when 0. This fact is not corrected
///          for, because the outcome might be different (wrong) when the compiler is doing its
///          calculations. It's highly recommended to use -0 when a type is declared that has only
///          negative numbers in its range.
template<
    std::integral BaseT,  ///< type of the scaled integer stored in memory
    scaling_t f_,         ///< number of fraction bits (precision 2^(-f))
    double realVMin_ = details::lowestRealVMin<BaseT, f_>,  ///< minimum real value represented by the type
    double realVMax_ = details::highestRealVMax<BaseT, f_> >  ///< maximum real value represented by the type
requires details::SqRequirements<BaseT, f_, realVMin_, realVMax_>
class Sq final {
public:
    static constexpr bool isSqType = true;  ///< identifier for the SqType concept
    using base_t = BaseT;  ///< integral base type
    static constexpr scaling_t f = f_;  ///< number of fraction bits
    static constexpr double realVMin = realVMin_;  ///< minimum real value
    static constexpr double realVMax = realVMax_;  ///< maximum real value
    static constexpr base_t vMin = v2s<base_t, f>(realVMin_);  ///< minimum value of integer value range
    static constexpr base_t vMax = v2s<base_t, f>(realVMax_);  ///< maximum value of integer value range
    static constexpr double resolution = v2s<double, -f>(1);  ///< real resolution

    /// Create a new Sq type with the same base type and scaling but a different real value range.
    template< double newRealVMin, double newRealVMax >
    struct clamp { using type = Sq< base_t, f, newRealVMin, newRealVMax >; };

    /// Type alias for clamp::type.
    template< double newRealVMin, double newRealVMax >
    using clamp_t = clamp<newRealVMin, newRealVMax>::type;

    /// Named compile-time-only "constructor" from a floating-point value. This will use v2s to scale
    /// the given floating-point value at compile-time before the Sq value is constructed with the
    /// scaled integer value.
    /// \note: When a real value is wrapped into an Sq value, there is an inherent rounding error due
    /// to the limited resolution. This error is called 'representation error' and it refers to the
    /// deviation from the initial real value when an Sq value is unscaled to a real value again.
    /// Usually the representation error is in the order of the resolution of the Sq type.
    /// \note Does not compile if the value is outside the value range.
    template< double realValue >
    requires ( realVMin <= realValue && realValue <= realVMax )  // must not overflow
    static constexpr
    Sq fromReal = Sq( v2s<base_t, f>(realValue) );

    /// Named compile-time-only "constructor" from a scaled integer value.
    /// \note Does not compile if the value is outside the value range.
    template< base_t value >
    requires ( vMin <= value && value <= vMax )  // must not overflow
    static constexpr
    Sq fromScaled = Sq( value );

    /// Named "Copy-Constructor" from another Sq type value with the same base type.
    /// \note When an Sq value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the Sq value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< /* deduced: */ SqType SqFrom >
    requires details::ImplicitlyConvertible<SqFrom, Sq>
    static constexpr
    Sq fromSq(SqFrom const &from) noexcept {
        return Sq( s2s<base_t, SqFrom::f, f>(from.reveal()) );
    }

    /// Copy-Constructor from another Sq type value with the same base type.
    /// Same as Sq::fromSq(), however a bit stricter (Sq types have to be different).
    /// \note When an Sq value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the Sq value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< /* deduced: */ SqType SqFrom >
    requires ( !std::is_same_v< Sq, SqFrom >  // when the same, default copy constructor should be used
               && details::ImplicitlyConvertible<SqFrom, Sq> )
    constexpr Sq(SqFrom const &from) noexcept : value( s2s<base_t, SqFrom::f, f>(from.reveal()) ) {}

    /// Copy-Constructor from the same type.
    constexpr Sq(Sq const &) noexcept = default;

    /// Move-Constructor from the same type.
    constexpr Sq(Sq&&) noexcept = default;

    /// Destructor.
    constexpr ~Sq() {}

    /// Explicit static and safe cast to a different Sq type with a different base type.
    /// Only possible if the value can be cast safely without any potential overflow, i.e. if the
    /// target value range is equal to or larger than the value range of this class.
    /// \note If a cast does not work it's most probably due to unfulfilled requirements.
    template< /* deduced: */ std::integral BaseTC, scaling_t fC, double realVMinC, double realVMaxC >
    requires ( details::Scalable<base_t, f, BaseTC, fC> && realVMinC <= realVMin && realVMax <= realVMaxC )
    explicit constexpr
    operator Sq<BaseTC, fC, realVMinC, realVMaxC>() const noexcept {
        using SqC = Sq<BaseTC, fC, realVMinC, realVMaxC>;

        // scale value
        auto cValue = s2s<typename SqC::base_t, f, SqC::f>(value);
        return SqC(cValue);
    }

    /// Unary plus operator. Integral promotion does not make any sense, so this just creates a copy.
    /// \returns a copy of the value with the same type.
    constexpr
    Sq operator +() const noexcept {
        return Sq( value );
    }

    /// Adds two Sq values.
    /// \returns a value of a new Sq type with the larger scaling (higher precision) and the value
    /// ranges added together. For the base type of the result a common type is determined that can
    /// hold the lowest and highest value of the resulting value range.
    template< /* deduced: */ SqType SqRhs,
        scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = realVMin + SqRhs::realVMin, double realVMaxR = realVMax + SqRhs::realVMax,
        std::integral BaseTR = details::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires details::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a+b+c.
    auto operator +(Sq const lhs, SqRhs const &rhs) noexcept {
        using SqR = Sq<BaseTR, fR, realVMinR, realVMaxR>;

        // add values
        auto result = s2s<BaseTR, f, SqR::f>(lhs.value) + s2s<BaseTR, SqRhs::f, SqR::f>(rhs.reveal());
        return SqR( static_cast<SqR::base_t>(result) );
    }

    /// Unary minus operator. Negates the type and its value.
    /// \note A signed type can be negated if the corresponding INT_MIN value is not in the value
    /// range of the type. An unsigned type is promoted to a new type with a sized base integer that
    /// has either the same size if possible, or twice the size (e.g. u16 -> i32).
    /// \note For a chain of n subtractions, the propagated error is approximately n * 2^(-f) plus the
    /// representation error of the initial value.
    /// \warning a +0.0 in the limits will be negated to -0.0 and vice versa. Note that two Sq types
    ///          which differ only by the sign of the 0 in a limit are not equal by design!
    /// \returns a value of a new Sq type with negated limits and value.
    template< /* deduced: */ double realVMinR = -realVMax, double realVMaxR = -realVMin,
        std::integral BaseTR = details::common_q_base_t<base_t, std::make_signed_t<base_t>, f, realVMinR, realVMaxR> >
    requires details::Absolutizable<base_t, vMin>
    constexpr
    auto operator -() const noexcept {
        using SqR = Sq<BaseTR, f, realVMinR, realVMaxR>;
        return SqR( static_cast<typename SqR::base_t>(-value) );
    }

    /// Subtracts the rhs value from the lhs value.
    /// \returns a value of a new Sq type with the larger scaling (higher precision) and the value
    /// ranges subtracted. For the base type of the result a common type is determined that can hold
    /// the lowest and highest value of the resulting value range.
    /// \note For a chain of n additions, the propagated error is approximately n * 2^(-f) plus the
    /// representation error of the initial value.
    template< /* deduced: */ SqType SqRhs,
        scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = std::min(realVMin - SqRhs::realVMax, SqRhs::realVMin - realVMax),
        double realVMaxR = std::max(realVMax - SqRhs::realVMin, SqRhs::realVMax - realVMin),
        std::integral BaseTR = details::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires details::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a-b-c.
    auto operator -(Sq const lhs, SqRhs const &rhs) noexcept {
        using SqR = Sq<BaseTR, fR, realVMinR, realVMaxR>;

        // subtract rhs value from lhs value
        auto result = s2s<BaseTR, f, SqR::f>(lhs.value) - s2s<BaseTR, SqRhs::f, SqR::f>(rhs.reveal());
        return SqR( static_cast<SqR::base_t>(result) );
    }

    /// Multiplies the lhs value with the rhs value.
    /// \returns a value of a new Sq type with the larger scaling (higher precision) and the value
    /// ranges multiplied. For the base type of the result a common type is determined that can hold
    /// the lowest and highest value of the resulting value range.
    /// \note The error propagation is complicated. When a number x is multiplied with itself n times,
    /// the real error is of order O( (n+1)*x^n * 2^(-f) ). For example, for a chain x*x*x (n=2) the
    /// real error is of order O( 3*x^2 * 2^(-f) ). Higher terms O( 2^(-m*f) ), m > 1 also occur for
    /// chains, but normally these are very close to 0 (when Q is large enough) and can be ignored.
    template< /* deduced: */ SqType SqRhs,
        scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = std::min(std::min(realVMin * SqRhs::realVMax, SqRhs::realVMin * realVMax),
                                    std::min(realVMin * SqRhs::realVMin, realVMax * SqRhs::realVMax)),
        double realVMaxR = std::max(std::max(realVMax * SqRhs::realVMin, SqRhs::realVMax * realVMin),
                                    std::max(realVMin * SqRhs::realVMin, realVMax * SqRhs::realVMax)),
        std::integral BaseTR = details::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires details::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a*b*c.
    auto operator *(Sq const lhs, SqRhs const &rhs) noexcept {
        using SqR = Sq<BaseTR, fR, realVMinR, realVMaxR>;
        using interm_m_t = interm_t<BaseTR>;

        // multiply lhs with rhs in intermediate type and correct scaling to obtain result
        // (a * 2^f) * (b * 2^f) / 2^f = a*b * 2^f
        auto intermediate = s2s<interm_m_t, f, SqR::f>(lhs.value) * s2s<interm_m_t, SqRhs::f, SqR::f>(rhs.reveal());
        auto result = s2s<typename SqR::base_t, 2*SqR::f, SqR::f>(intermediate);
        return SqR( result );
    }

    /// Divides the lhs value by the rhs value.
    /// \returns a value of a new Sq type with the larger scaling (higher precision) and the value
    /// ranges divided. For the base type of the result a common type is determined that can hold
    /// the lowest and highest value of the resulting value range.
    /// \warning Arithmetic underflow can happen if the result is smaller than the target resolution.
    /// \warning To ensure that compile-time overflow checks are not required, the rhs type must not
    ///          have values between -1 and +1 in its value range.
    /// \note The error propagation is complicated. When a number x is divided by itself n times,
    /// the real error is roughly 2 * sum{k=0..n}( 2^(-f)*2^(k*f) / (|x|*2^(-f)+1)^k ), which approaches
    /// 2*2^(-f) for large enough |x|. When |x|->1, the error approaches the limit
    /// 2*2^(-f) * (1 + 2^f - 2^(nf)/(2^f+1)^n), which is 2*(1+2^(-f)) for large n.
    /// For |x|->0 the error would approach infinity, thus the limitation of the divisor's value range.
    /// For example, a chain x/x/x, where x is divided by itself two times (n=2), has an error of
    /// 2*2^(-f)  <  2*2^(-f) * (1 + 2^f/(x*2^f+1) + 2^(2f)/(x*2^f+1)^2)  <  2*(1+2^(-f)).
    /// However, for |x| >= 1 and n >= 2 the error can be approximated reasonably well with 3*n*2^(-f).
    template< /* deduced: */ SqType SqRhs,
        scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = std::min(std::min(realVMin / SqRhs::realVMax, realVMin / SqRhs::realVMin),
                                    std::min(realVMax / SqRhs::realVMin, realVMax / SqRhs::realVMax)),
        double realVMaxR = std::max(std::max(realVMin / SqRhs::realVMax, realVMin / SqRhs::realVMin),
                                    std::max(realVMax / SqRhs::realVMin, realVMax / SqRhs::realVMax)),
        std::integral BaseTR = details::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires ( details::CanBeUsedAsDivisor<SqRhs>
               && details::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR> )
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a/b/c.
    auto operator /(Sq const lhs, SqRhs const &rhs) noexcept {
        using SqR = Sq<BaseTR, fR, realVMinR, realVMaxR>;
        using interm_m_t = interm_t<BaseTR>;

        // divide lhs by rhs in intermediate type and correct scaling to obtain result
        // (a * 2^(2f)) / (b * 2^f) = a/b * 2^f
        auto intermediate = s2s<interm_m_t, f, 2*SqR::f>(lhs.value) / s2s<interm_m_t, SqRhs::f, SqR::f>(rhs.reveal());
        auto result = static_cast<typename SqR::base_t>(intermediate);
        return SqR( result );
    }

    /// Divides the lhs value by the rhs value and returns the remainder of the division.
    /// The remainder of the division operation x/y calculated by this function is exactly the value
    /// x - n*y, where n is x/y with its fractional part truncated. The returned value has the same
    /// sign as x and is less or equal to y in magnitude.
    /// \returns the remainder of the division, as a value of a new Sq type with the larger scaling
    /// (higher precision) and with the value ranges adopted. If the base types are different, a common
    /// type is determined that can hold the lowest and highest value of the resulting value range.
    /// \warning Arithmetic underflow can happen if the result is smaller than the target resolution.
    /// \warning To ensure that compile-time overflow checks are not required, the rhs type must not
    ///          have values between -resolution and +resolution in its value range.
    /// \note The error propagation is similar to that of the division operator.
    template< /* deduced: */ SqType SqRhs,
        scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = std::max(realVMin, details::signum(realVMin) *
                                    std::max(details::abs(SqRhs::realVMin), details::abs(SqRhs::realVMax))),
        double realVMaxR = std::min(realVMax, details::signum(realVMax) *
                                    std::max(details::abs(SqRhs::realVMin), details::abs(SqRhs::realVMax))),
        std::integral BaseTR = details::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires ( details::CanBeUsedAsModulusDivisor<SqRhs>
               && details::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR> )
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a/b/c.
    auto operator %(Sq const lhs, SqRhs const &rhs) noexcept {
        using SqR = Sq<BaseTR, fR, realVMinR, realVMaxR>;
        using interm_m_t = interm_t<BaseTR>;

        // divide lhs by rhs in intermediate type and correct scaling to obtain result
        // (a * 2^f) % (b * 2^f) = a%b * 2^f
        auto intermediate = s2s<interm_m_t, f, SqR::f>(lhs.value) % s2s<interm_m_t, SqRhs::f, SqR::f>(rhs.reveal());
        auto result = static_cast<typename SqR::base_t>(intermediate);
        return SqR( result );
    }

    /// Compares lhs to rhs and returns true if the value of lhs is smaller than the value of rhs.
    /// Before comparison, both values are scaled to the larger f.
    /// \note Comparison is possible if both types are the same, or if the size of the lhs type is
    /// larger than the size of the rhs type. The common type is used for the comparison in these cases.
    /// \warning If two values ​​are compared that are closer together than the resolution, the result
    ///          may be false instead of true. The result is false if the values are identical, and
    ///          it is never true if it should be false.
    /// \returns true if the value of lhs is smaller than the value of rhs, and false otherwise.
    template< /* deduced: */ SqType SqRhs,
        typename BaseTC = std::common_type_t<base_t, typename SqRhs::base_t>,
        scaling_t fC = std::max(f, SqRhs::f) >
    requires details::Comparable<base_t, typename SqRhs::base_t>
    friend constexpr
    bool operator <(Sq const &lhs, SqRhs const &rhs) noexcept {
        return s2s<BaseTC, f, fC>(lhs.value) < s2s<BaseTC, SqRhs::f, fC>(rhs.value);
    }

    /// Compares lhs to rhs and returns true if the value of lhs is larger than the value of rhs.
    /// Before comparison, both values are scaled to the larger f.
    /// \note Comparison is possible if both types are the same, or if the size of the lhs type is
    /// larger than the size of the rhs type. The common type is used for the comparison in these cases.
    /// \warning If two values ​​are compared that are closer together than the resolution, the result
    ///          may be false instead of true. The result is false if the values are identical, and
    ///          it is never true if it should be false.
    /// \returns true if the value of lhs is larger than the value of rhs, and false otherwise.
    template< /* deduced: */ SqType SqRhs,
        typename BaseTC = std::common_type_t<base_t, typename SqRhs::base_t>,
        scaling_t fC = std::max(f, SqRhs::f) >
    requires details::Comparable<base_t, typename SqRhs::base_t>
    friend constexpr
    bool operator >(Sq const &lhs, SqRhs const &rhs) noexcept {
        return s2s<BaseTC, f, fC>(lhs.value) > s2s<BaseTC, SqRhs::f, fC>(rhs.value);
    }

    /// Compares lhs to rhs and returns true if the value of lhs is equal to the value of rhs.
    /// \note For this comparison to work reliably both types need to have the same scaling.
    /// \note Comparison is possible if base types are the same, or if the size of the lhs type is
    /// larger than the size of the rhs type. The common type is used for the comparison in these cases.
    /// \warning If two values ​​are compared that are closer together than the resolution, the result
    ///          may be true instead of false. The result is true if the values are identical, and
    ///          it is never false if it should be true.
    /// \returns true if the value of lhs is equal to the value of rhs, and false otherwise.
    template< /* deduced: */ SqType SqRhs, typename BaseTC = std::common_type_t<base_t, typename SqRhs::base_t> >
    requires ( details::Comparable<base_t, typename SqRhs::base_t> && f == SqRhs::f )
    friend constexpr
    bool operator ==(Sq const &lhs, SqRhs const &rhs) noexcept {
        return static_cast<BaseTC>(lhs.value) == static_cast<BaseTC>(rhs.value);
    }

    /// Compares lhs to rhs and returns true if the value of lhs is not equal to the value of rhs.
    /// \note For this comparison to work reliably both types need to have the same scaling.
    /// \note Comparison is possible if base types are the same, or if the size of the lhs type is
    /// larger than the size of the rhs type. The common type is used for the comparison in these cases.
    /// \warning If two values ​​are compared that are closer together than the resolution, the result
    ///          may be true instead of false. The result is true if the values are identical, and
    ///          it is never false if it should be true.
    /// \returns true if the value of lhs is not equal to the value of rhs, and false otherwise.
    template< /* deduced: */ SqType SqRhs, typename BaseTC = std::common_type_t<base_t, typename SqRhs::base_t> >
    requires ( details::Comparable<base_t, typename SqRhs::base_t> && f == SqRhs::f )
    friend constexpr
    bool operator !=(Sq const &lhs, SqRhs const &rhs) noexcept {
        return static_cast<BaseTC>(lhs.value) != static_cast<BaseTC>(rhs.value);
    }

    /// Reveals the integer value stored in memory.
    constexpr base_t reveal() const noexcept {
        return value;
    }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning This conversion is expensive if the target type is a floating-point type.
    ///          If the target type is an integral type, there can be a significant loss of resolution.
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
    Sq() = delete;  // default constructor
    Sq& operator =(Sq const &) = delete;  // copy-assignment
    Sq& operator =(Sq&&) = delete;  // move-assignment

    /// Explicit, possibly compile-time constructor from scaled integer value.
    explicit constexpr Sq(base_t value) noexcept : value(value) {}

    // friend all Sq types so that private members of similar types can be accessed for construction
    template< std::integral BaseTSq, scaling_t fSq, double realVMinSq, double realVMaxSq >
    requires details::SqRequirements<BaseTSq, fSq, realVMinSq, realVMaxSq>
    friend class Sq;

    // friend Q type so that it can access the private members of a Sq type to construct it
    template< std::integral BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
    requires details::QRequirements<BaseTQ, fQ, realVMinQ, realVMaxQ, ovfQ>
    friend class q::Q;

    // friend some methods so that they can access private members of a Sq type to construct new
    // variants of it

    template< SqType SqT, std::integral BaseTR, double realVMinR, double realVMaxR >
    requires details::Absolutizable<typename SqT::base_t, SqT::vMin>
    friend constexpr
    auto abs(SqT const &sqValue) noexcept;

    template< SqType SqV, SqType SqLo, SqType SqHi >
    requires details::Clampable<SqV, SqLo, SqHi>
    friend constexpr
    auto clamp(SqV const &value, SqLo const &lo, SqHi const &hi) noexcept;

    template< double realLo, double realHi, SqType SqV >
    requires details::RealLimitsInRangeOfBaseType<typename SqV::base_t, SqV::f, realLo, realHi>
    friend constexpr
    auto clamp(SqV const &value) noexcept;

    /// scaled integer value that represents a fixed-point value; stored in memory
    base_t const value;
};

/// Explicit static cast to another Sq type with a different base type.
/// Uses static_cast internally. Exists for consistency reasons.
template< SqType SqC, /* deduced: */ SqType SqFrom >
requires details::CastableWithoutChecks<SqFrom, SqC>
constexpr
SqC static_sq_cast(SqFrom from) noexcept {
    return static_cast<SqC>(from);
}

/// Explicit safe cast to another Sq type with a different base type.
/// Uses static_cast internally. Exists for consistency reasons.
template< SqType SqC, /* deduced: */ SqType SqFrom >
requires details::CastableWithoutChecks<SqFrom, SqC>
constexpr
SqC safe_sq_cast(SqFrom from) noexcept {
    return static_cast<SqC>(from);
}

/// Takes the absolute value of the given Sq value.
/// \note The absolute value of a signed type can be taken if the corresponding INT_MIN value is
/// not in the value range of the Sq type.
/// \returns a new unsigned Sq type with a modified range that holds the absolute value.
template< /* deduced: */ SqType SqT,
    std::integral BaseTR = std::make_unsigned_t<typename SqT::base_t>,
    double realVMinR = (std::is_signed_v<typename SqT::base_t> && SqT::vMin < 0 && SqT::vMax > 0)
        ? 0.0  // use 0 as new minimum if signed input type has a range of negative and positive values
        : std::min(details::abs((double)SqT::realVMin), details::abs((double)SqT::realVMax)),
    double realVMaxR = std::max(details::abs((double)SqT::realVMin), details::abs((double)SqT::realVMax)) >
requires details::Absolutizable<typename SqT::base_t, SqT::vMin>
constexpr
auto abs(SqT const &of) noexcept {
    return Sq<BaseTR, SqT::f, realVMinR, realVMaxR>( std::abs(of.value) );
}

/// If value compares less than lo, lo is returned; otherwise if hi compares less than value, hi is
/// returned; otherwise value is returned.
/// \returns a new Sq type with the lower limit of SqLo and the upper limit of SqHi and the clamped
/// value.
template< /* deduced: */ SqType SqV, SqType SqLo, SqType SqHi >
requires details::Clampable<SqV, SqLo, SqHi>
constexpr
auto clamp(SqV const &value, SqLo const &lo, SqHi const &hi) noexcept {
    using SqR = Sq<typename SqV::base_t, SqV::f, SqLo::realVMin, SqHi::realVMax>;
    // lo and hi are scaled via constructor from similar type if used; value's value just can be taken as is
    return (value < lo) ? SqR(lo) : (hi < value) ? SqR(hi) : SqR(value.value);
}

/// Version of clamp() for limits known at compile-time: if value compares less than lo, lo is returned;
/// otherwise if hi compares less than value, hi is returned; otherwise value is returned.
/// \returns a new Sq type with the value range defined via template parameters and the clamped value.
template< double realLo, double realHi, /* deduced: */ SqType SqV >
requires details::RealLimitsInRangeOfBaseType<typename SqV::base_t, SqV::f, realLo, realHi>
constexpr
auto clamp(SqV const &value) noexcept {
    using SqR = typename SqV::clamp_t<realLo, realHi>;
    constexpr auto sqLo = SqR::template fromReal<realLo>;
    constexpr auto sqHi = SqR::template fromReal<realHi>;
    return (value < sqLo) ? sqLo : (sqHi < value) ? sqHi : SqR(value.value);
}

/// Converts a literal number into the corresponding best-fit sq type.
/// Best-fit means that the literal number represents both limits and the value.
template< SqType Sq, char ...charArray >
consteval auto sqFromLiteral() {
    constexpr double value = details::doubleFromLiteral<charArray...>();
    return Sq::template clamp_t<value, value>::template fromReal<value>;
}

/**\}*/
}  // end of fpm::sq


namespace std {

/// Provides the bare, real numeric limits for the given Sq type.
template< /* deduced: */ std::integral BaseT, fpm::scaling_t f, double realVMin, double realVMax >
class numeric_limits<fpm::sq::Sq<BaseT, f, realVMin, realVMax>> {
    using SqT = fpm::sq::Sq<BaseT, f, realVMin, realVMax>;
public:
    /// \returns the minimum real value that can be represented by the Sq type.
    /// \note In contrast to Sq::realVMin, this does not return the minimum value specified by the
    /// user, but the absolute minimum that can be represented by the underlying Sq type with respect
    /// to its base type and scaling. This can be significantly smaller that the actual user minimum.
    template< typename T = double >
    static constexpr T min() noexcept {
        return fpm::v2s<T, -SqT::f>( numeric_limits<typename SqT::base_t>::min() );
    }

    /// \returns the maximum real value that can be represented by the Sq type.
    /// \note In contrast to Sq::realVMax, this does not return the maximum value specified by the
    /// user, but the absolute maximum that can be represented by the underlying Sq type with respect
    /// to its base type and scaling. This can be significantly larger that the actual user maximum.
    template< typename T = double >
    static constexpr T max() noexcept {
        return fpm::v2s<T, -SqT::f>( numeric_limits<typename SqT::base_t>::max() );
    }

    constexpr static bool is_specialized = true;
    constexpr static bool is_signed = numeric_limits<typename SqT::base_t>::is_signed;
    constexpr static bool is_bounded = true;
    constexpr static bool traps = true;
#   if defined FPM_USE_SH
    constexpr static auto round_style = std::round_toward_neg_infinity;
#   else
    constexpr static auto round_style = std::round_toward_zero;
#   endif
    constexpr static int radix = numeric_limits<typename SqT::base_t>::radix;
    constexpr static int digits = numeric_limits<typename SqT::base_t>::digits - SqT::f;
    constexpr static int digits10 = static_cast<int>( std::log10(radix) * digits );
};

}

#endif
// EOF
