/** \file
 * Implementation of the Sq class template.
 */

#ifndef FPM_FPM_SQ_HPP_
#define FPM_FPM_SQ_HPP_

#include "fpm.hpp"
#include <compare>


// forward declare fpm::q::Q so that it can be friended by fpm::sq::Sq
namespace fpm::q {
template< std::integral BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
requires detail::QRequirements<BaseTQ, fQ, realVMinQ, realVMaxQ, ovfQ>
class Q;
}


namespace fpm::sq {
/** \ingroup grp_fpm
 * \defgroup grp_fpmSq Static Q-Type
 * \{ */

using detail::SqType;

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
    double realVMin_ = detail::lowestRealVMin<BaseT, f_>,  ///< minimum real value represented by the type
    double realVMax_ = detail::highestRealVMax<BaseT, f_> >  ///< maximum real value represented by the type
requires detail::SqRequirements<BaseT, f_, realVMin_, realVMax_>
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
    requires detail::ImplicitlyConvertible<SqFrom, Sq>
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
               && detail::ImplicitlyConvertible<SqFrom, Sq> )
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
    requires ( detail::Scalable<base_t, f, BaseTC, fC> && realVMinC <= realVMin && realVMax <= realVMaxC )
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
    /// \returns the sum, wrapped into a new Sq type with the larger scaling (higher precision) and
    /// the value ranges added together. For the base type of the result a common type is determined
    /// that can hold the lowest and highest value of the resulting value range.
    template< /* deduced: */ SqType SqRhs,
        scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = realVMin + SqRhs::realVMin, double realVMaxR = realVMax + SqRhs::realVMax,
        std::integral BaseTR = detail::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires detail::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
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
    /// \returns the negated value, wrapped into a new Sq type with negated limits.
    template< /* deduced: */ double realVMinR = -realVMax, double realVMaxR = -realVMin,
        std::integral BaseTR = detail::common_q_base_t<base_t, std::make_signed_t<base_t>, f, realVMinR, realVMaxR> >
    requires detail::Absolutizable<base_t, vMin>
    constexpr
    auto operator -() const noexcept {
        using SqR = Sq<BaseTR, f, realVMinR, realVMaxR>;
        return SqR( static_cast<typename SqR::base_t>(-value) );
    }

    /// Subtracts the rhs value from the lhs value.
    /// \returns the difference, wrapped into a new Sq type with the larger scaling (higher precision)
    /// and the value ranges subtracted. For the base type of the result a common type is determined
    /// that can hold the lowest and highest value of the resulting value range.
    /// \note For a chain of n additions, the propagated error is approximately n * 2^(-f) plus the
    /// representation error of the initial value.
    template< /* deduced: */ SqType SqRhs,
        scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = std::min(realVMin - SqRhs::realVMax, SqRhs::realVMin - realVMax),
        double realVMaxR = std::max(realVMax - SqRhs::realVMin, SqRhs::realVMax - realVMin),
        std::integral BaseTR = detail::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires detail::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a-b-c.
    auto operator -(Sq const lhs, SqRhs const &rhs) noexcept {
        using SqR = Sq<BaseTR, fR, realVMinR, realVMaxR>;

        // subtract rhs value from lhs value
        auto result = s2s<BaseTR, f, SqR::f>(lhs.value) - s2s<BaseTR, SqRhs::f, SqR::f>(rhs.reveal());
        return SqR( static_cast<SqR::base_t>(result) );
    }

    /// Multiplies the lhs value with the rhs value.
    /// \returns the product, wrapped into a new Sq type with the larger scaling (higher precision)
    /// and the value ranges multiplied. For the base type of the result a common type is determined
    /// that can hold the lowest and highest value of the resulting value range.
    /// \note The error propagation is complicated. When a number x is multiplied with itself n times,
    /// the real error is (n+1)*x^n * 2^(-f). For example, for a chain x*x*x (n=2) the real error is
    /// 3*x^2 * 2^(-f). Higher terms O( 2^(-m*f) ), m > 1 also occur for chains, but normally these
    /// are very close to 0 (when f is large enough) and can be ignored.
    template< /* deduced: */ SqType SqRhs,
        scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = std::min(std::min(realVMin * SqRhs::realVMax, SqRhs::realVMin * realVMax),
                                    std::min(realVMin * SqRhs::realVMin, realVMax * SqRhs::realVMax)),
        double realVMaxR = std::max(std::max(realVMax * SqRhs::realVMin, SqRhs::realVMax * realVMin),
                                    std::max(realVMin * SqRhs::realVMin, realVMax * SqRhs::realVMax)),
        std::integral BaseTR = detail::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires detail::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR>
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a*b*c.
    auto operator *(Sq const lhs, SqRhs const &rhs) noexcept {
        using SqR = Sq<BaseTR, fR, realVMinR, realVMaxR>;
        using interm_m_t = interm_t<BaseTR>;

        // multiply lhs with rhs in intermediate type and correct scaling to obtain result
        // a*b <=> (a * 2^f) * (b * 2^f) / 2^f = a*b * 2^f
        auto intermediate = s2s<interm_m_t, f, SqR::f>(lhs.value) * s2s<interm_m_t, SqRhs::f, SqR::f>(rhs.reveal());
        auto result = s2s<typename SqR::base_t, 2*SqR::f, SqR::f>(intermediate);
        return SqR( result );
    }

    /// Divides the lhs value by the rhs value.
    /// \returns the quotient, wrapped into a new Sq type with the larger scaling (higher precision)
    /// and the value ranges divided. For the base type of the result a common type is determined that
    /// can hold the lowest and highest value of the resulting value range.
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
        std::integral BaseTR = detail::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires ( detail::CanBeUsedAsDivisor<SqRhs>
               && detail::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR> )
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a/b/c.
    auto operator /(Sq const lhs, SqRhs const &rhs) noexcept {
        using SqR = Sq<BaseTR, fR, realVMinR, realVMaxR>;
        using interm_m_t = interm_t<BaseTR>;

        // divide lhs by rhs in intermediate type and correct scaling to obtain result
        // a/b <=> (a * 2^(2f)) / (b * 2^f) = a/b * 2^f
        auto intermediate = s2s<interm_m_t, f, 2*SqR::f>(lhs.value) / s2s<interm_m_t, SqRhs::f, SqR::f>(rhs.reveal());
        auto result = static_cast<typename SqR::base_t>(intermediate);
        return SqR( result );
    }

    /// Divides the lhs value by the rhs value and returns the remainder of the division.
    /// The remainder of the division operation x/y calculated by this function is exactly the value
    /// x - n*y, where n is x/y with its fractional part truncated. The returned value has the same
    /// sign as x and is less or equal to y in magnitude.
    /// \returns the remainder of the division, wrapped into a new Sq type with the larger scaling
    /// (higher precision) and with the value ranges adopted. If the base types are different, a common
    /// type is determined that can hold the lowest and highest value of the resulting value range.
    /// \warning Arithmetic underflow can happen if the result is smaller than the target resolution.
    /// \warning To ensure that compile-time overflow checks are not required, the rhs type must not
    ///          have values between -resolution and +resolution in its value range.
    /// \note The error propagation is similar to that of the division operator.
    template< /* deduced: */ SqType SqRhs,
        scaling_t fR = std::max(SqRhs::f, f),
        double realVMinR = std::max(realVMin, detail::signum(realVMin) *
                                    std::max(detail::abs(SqRhs::realVMin), detail::abs(SqRhs::realVMax))),
        double realVMaxR = std::min(realVMax, detail::signum(realVMax) *
                                    std::max(detail::abs(SqRhs::realVMin), detail::abs(SqRhs::realVMax))),
        std::integral BaseTR = detail::common_q_base_t<base_t, typename SqRhs::base_t, fR, realVMinR, realVMaxR> >
    requires ( detail::CanBeUsedAsModulusDivisor<SqRhs>
               && detail::RealLimitsInRangeOfBaseType<BaseTR, fR, realVMinR, realVMaxR> )
    friend constexpr
    // Note: Passing lhs by value helps optimize chained a/b/c.
    auto operator %(Sq const lhs, SqRhs const &rhs) noexcept {
        using SqR = Sq<BaseTR, fR, realVMinR, realVMaxR>;
        using interm_m_t = interm_t<BaseTR>;

        // divide lhs by rhs in intermediate type and correct scaling to obtain result
        // a%b <=> (a * 2^f) % (b * 2^f) = a%b * 2^f
        auto intermediate = s2s<interm_m_t, f, SqR::f>(lhs.value) % s2s<interm_m_t, SqRhs::f, SqR::f>(rhs.reveal());
        auto result = static_cast<typename SqR::base_t>(intermediate);
        return SqR( result );
    }

    /// Primary operator to compare for equality. The inequality operator (!=) is synthesized from
    /// this operator. The input values are scaled to the lower resolution of the two input types
    /// before being compared.
    /// \note Comparison is possible if both types have the same signedness, or if the size of the
    /// lhs type is larger than the size of the rhs type if the signedness is different.
    /// The common type is used for the comparison in these cases.
    /// \warning If two values ​​are compared that are closer together than the higher resolution, the
    ///          result may be true instead of false.
    template< /* deduced: */ SqType SqRhs,
        typename BaseTR = std::common_type_t<base_t, typename SqRhs::base_t>,
        scaling_t fMin = std::min(f, SqRhs::f),
        scaling_t fMax = std::max(f, SqRhs::f) >
    requires detail::Comparable<base_t, typename SqRhs::base_t>
    constexpr
    bool operator ==(SqRhs const &rhs) const noexcept {
        // two values are considered equal if the values, scaled to the lower resolution, are equivalent
        return s2s<BaseTR, f, fMin>(value) == s2s<BaseTR, SqRhs::f, fMin>(rhs.value);
    }

    /// Convenient three-way ordering operator. The secondary relational operators <, <=, > and >=
    /// are synthesized from this operator. The input values are scaled to the greater resolution
    /// before being compared.
    /// \note Comparison is possible if both types have the same signedness, or if the size of the
    /// lhs type is larger than the size of the rhs type if the signedness is different.
    /// The common type is used for the comparison in these cases.
    /// \warning If two values ​​are compared that are closer together than the resolution, the result
    ///          may be false instead of true (i.e. the values may be considered equal).
    template< /* deduced: */ SqType SqRhs,
        typename BaseTR = std::common_type_t<base_t, typename SqRhs::base_t>,
        scaling_t fMin = std::min(f, SqRhs::f),
        scaling_t fMax = std::max(f, SqRhs::f) >
    requires detail::Comparable<base_t, typename SqRhs::base_t>
    constexpr
    std::strong_ordering operator <=>(SqRhs const &rhs) const noexcept {
        // the two values are compared with the higher resolution
        return s2s<BaseTR, f, fMax>(value) <=> s2s<BaseTR, SqRhs::f, fMax>(rhs.value);
    }

    /// Left-Shifts lhs by the number of bits given by the rhs integral constant.
    /// \note Shifting is possible if the shifted limits cannot exceed the base type's value range.
    /// \warning Arithmetic underflow can happen if the result is smaller than the target resolution.
    /// \returns the shifted value, wrapped into a new Sq type with shifted limits.
    template< /* deduced: */ std::integral T, T v,
        double realVMinR = v2s<double, -f>(static_cast<fpm::interm_t<base_t>>(vMin) << v),
        double realVMaxR = v2s<double, -f>(static_cast<fpm::interm_t<base_t>>(vMax) << v) >
    requires ( std::is_unsigned_v<T>
               && detail::RealLimitsInRangeOfBaseType<base_t, f, realVMinR, realVMaxR> )
    friend constexpr
    auto operator <<(Sq const &lhs, std::integral_constant<T, v> const) noexcept {
        using SqR = typename Sq::clamp_t<realVMinR, realVMaxR>;
        return SqR( lhs.value << v );
    }

    /// Right-Shifts lhs by the number of bits given by the rhs integral constant.
    /// \note Shifting is possible if the shifted limits cannot exceed the base type's value range.
    /// \returns the shifted value, wrapped into a new Sq type with shifted limits.
    template< /* deduced: */ std::integral T, T v,
        double realVMinR = v2s<double, -f>(static_cast<fpm::interm_t<base_t>>(vMin) >> v),
        double realVMaxR = v2s<double, -f>(static_cast<fpm::interm_t<base_t>>(vMax) >> v) >
    requires ( std::is_unsigned_v<T>
               && detail::RealLimitsInRangeOfBaseType<base_t, f, realVMinR, realVMaxR> )
    friend constexpr
    auto operator >>(Sq const &lhs, std::integral_constant<T, v> const) noexcept {
        using SqR = typename Sq::clamp_t<realVMinR, realVMaxR>;
        return SqR( lhs.value >> v );
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
    requires detail::SqRequirements<BaseTSq, fSq, realVMinSq, realVMaxSq>
    friend class Sq;

    // friend Q type so that it can access the private members of a Sq type to construct it
    template< std::integral BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
    requires detail::QRequirements<BaseTQ, fQ, realVMinQ, realVMaxQ, ovfQ>
    friend class q::Q;

    // friend some methods so that they can access private members of a Sq type to construct new
    // variants of it

    template< SqType SqT, std::integral BaseTR, double realVMinR, double realVMaxR >
    requires detail::Absolutizable<typename SqT::base_t, SqT::vMin>
    friend constexpr
    auto abs(SqT const &sqValue) noexcept;

    template< /* deduced: */ typename BaseTR = std::common_type_t<int32_t, base_t>,
        double realVMinR = (std::is_signed_v<base_t> && vMin < 0 && vMax > 0)
            ? 0.0  // use 0 as new minimum if signed input type has a range of negative and positive values
            : std::min(realVMin*realVMin, realVMax*realVMax),
        double realVMaxR = std::max(realVMin*realVMin, realVMax*realVMax) >
    requires detail::RealLimitsInRangeOfBaseType<BaseTR, f, realVMinR, realVMaxR>
    friend constexpr
    auto squareImpl(Sq const &x) noexcept {
        using SqR = Sq<BaseTR, f, realVMinR, realVMaxR>;
        using interm_v_t = interm_t<BaseTR>;

        // x^2 <=> [ (x*2^f)*(x*2^f) / 2^f ] = x*x*2^f
        auto xIntm = static_cast<interm_v_t>(x.value);
        return SqR( static_cast<BaseTR>( xIntm*xIntm / v2s<interm_v_t, SqR::f>(1) ) );
    }

    template< /* deduced: */
        double realVMinR = detail::floor( detail::sqrt(realVMin) ),  // round limits to be more tolerant towards approximation
        double realVMaxR = detail::ceil( detail::sqrt(realVMax) ) >
    requires ( sizeof(base_t) <= sizeof(uint32_t)
            && realVMin >= 0.
            && f <= 30  //< ceil of upper q31 limit would round up to 2.0 which is out of value range
            && detail::RealLimitsInRangeOfBaseType<base_t, f, realVMinR, realVMaxR> )
    friend constexpr
    auto sqrtImpl(Sq const &x) noexcept {
        using SqR = clamp_t<realVMinR, realVMaxR>;
        using interm_v_t = interm_t<base_t>;

        // negative value has imaginary root, real part is 0
        base_t y = x.value <= 0
            ? 0
            // take root of corrected number; result can be cast to base_t without truncation
            // sqrt(x) <=> [ ((x*2^f) * 2^f)^1/2 ] = x^1/2 * 2^f
            : static_cast<base_t>( detail::isqrt( static_cast<interm_v_t>(x.value) * v2s<interm_v_t, SqR::f>(1) ) );
        return SqR( y );
    }

    /// \copydoc fpm::sq::rsqrt
    template< /* deduced: */ double thMax = detail::highestRealVMax<base_t, f>(),
        double realVMinR = detail::floor( detail::rsqrt(realVMax) ),
        double realVMaxR = std::min( thMax, detail::ceil( detail::rsqrt(realVMin) ) ) >
    requires ( sizeof(base_t) <= sizeof(int32_t)
            && realVMin > 0.
            && f <= 30  //< ceil of upper q31 limit would round up to 2.0 which is out of value range
            && detail::RealLimitsInRangeOfBaseType<base_t, f, realVMinR, realVMaxR> )
    friend constexpr
    auto rsqrtImpl(Sq const &x) noexcept {
        using SqR = clamp_t<realVMinR, realVMaxR>;
        using interm_v_t = interm_t<base_t>;

        // too small number results in theoretical maximum; this is the case if x*2^f <= (2^f / max^2)
        constexpr base_t limit = fpm::v2s<base_t, f>( 1. / thMax / thMax );
        base_t y = x.value < limit
            ? v2s<base_t, SqR::f>(thMax)
            // 1/sqrt(x) <=> [ 2^(2f) / ((x*2^f) * 2^f)^1/2 ] = 2^f / sqrt(x)
            : static_cast<base_t>( v2s<interm_v_t, 2*SqR::f>(1) / static_cast<interm_v_t>( sqrt(x).value ) );
        return SqR( y );
    }

    /// \copydoc fpm::sq::cube
    template< /* deduced: */ typename BaseTR = std::common_type_t<int32_t, base_t>,
        double l1 = realVMin*realVMin*realVMin,
        double l2 = realVMin*realVMin*realVMax,
        double l3 = realVMin*realVMax*realVMax,
        double l4 = realVMax*realVMax*realVMax,
        double realVMinR = std::min(std::min(std::min(l1, l2), l3), l4),
        double realVMaxR = std::max(std::max(std::max(l1, l2), l3), l4) >
    requires detail::RealLimitsInRangeOfBaseType<BaseTR, f, realVMinR, realVMaxR>
    friend constexpr
    auto cubeImpl(Sq const &x) noexcept {
        using SqR = Sq<BaseTR, f, realVMinR, realVMaxR>;
        using interm_v_t = interm_t<BaseTR>;

        // x^3 <=> [ (x*2^f)*(x*2^f) / 2^f * (x*2^f) / 2^f ] = [ square(x)*(x*2^f) / 2^f ] = x*x*x*2^f
        auto xIntm = static_cast<interm_v_t>(x.value);
        auto xSqr = static_cast<interm_v_t>(square(x).value);
        return SqR( static_cast<BaseTR>( xSqr*xIntm / v2s<interm_v_t, SqR::f>(1) ) );
    }

    /// \copydoc fpm::sq::cbrt
    template< /* deduced: */
        double realVMinR = (realVMin < 0. ? 0. : detail::floor( detail::cbrt(realVMin) )),  // round limits to be more tolerant towards approximation
        double realVMaxR = (realVMax < 0. ? 0. : detail::ceil( detail::cbrt(realVMax) )) >
    requires ( sizeof(base_t) <= sizeof(int32_t)
               && f <= 16  // limit scaling to prevent overflow
               && realVMin >= 0.
               && detail::RealLimitsInRangeOfBaseType<base_t, f, realVMinR, realVMaxR> )
    friend constexpr
    auto cbrtImpl(Sq const &x) noexcept {
        using SqR = clamp_t<realVMinR, realVMaxR>;
        using interm_v_t = interm_t<base_t>;

        // negative number is out of scope for hardware algorithm
        base_t y = x.value < 0
            ? 0
            // cbrt(x) <=> [ ((x*2^f) * 2^f * 2^f)^1/3 ] = x^1/3 * 2^f
            : static_cast<base_t>(
                detail::icbrt( static_cast<interm_v_t>(x.value) * v2s<interm_v_t, SqR::f>(1) * v2s<interm_v_t, SqR::f>(1) ) );
        return SqR( y );
    }

    template< SqType SqV, SqType SqLo, SqType SqHi >
    requires detail::Clampable<SqV, SqLo, SqHi>
    friend constexpr
    auto clamp(SqV const &value, SqLo const &lo, SqHi const &hi) noexcept;

    template< SqType SqV, SqType SqLo >
    requires detail::ImplicitlyConvertible<SqLo, SqV>
    friend constexpr
    auto clampLower(SqV const &value, SqLo const &lo) noexcept;

    template< SqType SqV, SqType SqHi >
    requires detail::ImplicitlyConvertible<SqHi, SqV>
    friend constexpr
    auto clampUpper(SqV const &value, SqHi const &hi) noexcept;

    template< double realLo, double realHi, SqType SqV >
    requires detail::RealLimitsInRangeOfBaseType<typename SqV::base_t, SqV::f, realLo, realHi>
    friend constexpr
    auto clamp(SqV const &value) noexcept;

    template< double realLo, SqType SqV >
    requires detail::RealLimitsInRangeOfBaseType<typename SqV::base_t, SqV::f, realLo, SqV::realVMax>
    friend constexpr
    auto clampLower(SqV const &value) noexcept;

    template< double realHi, SqType SqV >
    requires detail::RealLimitsInRangeOfBaseType<typename SqV::base_t, SqV::f, SqV::realVMin, realHi>
    friend constexpr
    auto clampUpper(SqV const &value) noexcept;

    template< SqType Sq1, SqType Sq2, double realVMinMin, double realVMaxMin >
    requires ( detail::Similar<Sq1, Sq2>
               && detail::RealLimitsInRangeOfBaseType<typename Sq1::base_t, Sq1::f, realVMinMin, realVMaxMin> )
    friend constexpr
    auto min(Sq1 const &first, Sq2 const &second) noexcept;

    template< SqType Sq1, SqType Sq2, double realVMinMax, double realVMaxMax >
    requires ( detail::Similar<Sq1, Sq2>
               && detail::RealLimitsInRangeOfBaseType<typename Sq1::base_t, Sq1::f, realVMinMax, realVMaxMax> )
    friend constexpr
    auto max(Sq1 const &first, Sq2 const &second) noexcept;

    /// scaled integer value that represents a fixed-point value; stored in memory
    base_t const value;
};

/// Explicit static cast to another Sq type with a different base type.
/// Uses static_cast internally. Exists for consistency reasons.
template< SqType SqC, /* deduced: */ SqType SqFrom >
requires detail::CastableWithoutChecks<SqFrom, SqC>
constexpr
SqC static_sq_cast(SqFrom from) noexcept {
    return static_cast<SqC>(from);
}

/// Explicit safe cast to another Sq type with a different base type.
/// Uses static_cast internally. Exists for consistency reasons.
template< SqType SqC, /* deduced: */ SqType SqFrom >
requires detail::CastableWithoutChecks<SqFrom, SqC>
constexpr
SqC safe_sq_cast(SqFrom from) noexcept {
    return static_cast<SqC>(from);
}

/// Takes the absolute value of the given Sq value.
/// \note The absolute value of a signed type can be taken if the corresponding INT_MIN value is
/// not in the value range of the Sq type.
/// \returns the absolute value, wrapped into a new unsigned Sq type with a modified range.
template< /* deduced: */ SqType SqT,
    std::integral BaseTR = std::make_unsigned_t<typename SqT::base_t>,
    double realVMinR = (std::is_signed_v<typename SqT::base_t> && SqT::vMin < 0 && SqT::vMax > 0)
        ? 0.0  // use 0 as new minimum if signed input type has a range of negative and positive values
        : std::min(detail::abs((double)SqT::realVMin), detail::abs((double)SqT::realVMax)),
    double realVMaxR = std::max(detail::abs((double)SqT::realVMin), detail::abs((double)SqT::realVMax)) >
requires detail::Absolutizable<typename SqT::base_t, SqT::vMin>
constexpr
auto abs(SqT const &of) noexcept {
    return Sq<BaseTR, SqT::f, realVMinR, realVMaxR>( std::abs(of.value) );
}

/// \returns the squared value of the given number x, wrapped into a new Sq type with at least
/// 32 bits base type and squared limits. The base type of the resulting value is the common type
/// of int32 and the base type of the given number.
/// \note The error propagation is similar to that of the multiplication operator: When a number
/// x is multiplied with itself n times, the maximum real error is (n+1)*x^n * 2^(-f).
/// For the square function (n=1) this gives 2x * 2^(-f) at most.
template< /* deduced: */ SqType SqT >
constexpr auto square(SqT const &x) noexcept { return squareImpl(x); }

/// \returns the computed square root of the given number x, wrapped into a new Sq type with the
/// square roots of the limits.
/// \note A binary search algorithm is used to calculate the square root in logarithmic time.
template< /* deduced: */ SqType SqT >
constexpr auto sqrt(SqT const &x) noexcept { return sqrtImpl(x); }

/// \returns the computed reciprocal square root of the given number x, wrapped into a new Sq type
/// with the reciprocal square root of the limits. The maximum limit of the resulting type does not
/// exceed the value range of its base type.
/// \warning This uses a division. May be expensive!
/// \note The maximum real error evaluates to 2^(-f) * ( 1/2*(1/x)^(3/2) - O( 2^(-f)*(1/x)^(5/2) ) ).
/// Unless x is very small ( x < 1 / 2^((2f+2)/3) ), it is usually enough to estimate the real
/// error with the resolution of the given number.
template< /* deduced: */ SqType SqT >
constexpr auto rsqrt(SqT const &x) noexcept { return rsqrtImpl(x); }

/// \returns the cube of the given number x, wrapped into a new Sq type with at least 32 bits
/// base type and cubed limits. The base type of the resulting value is the common type of int32
/// and the base type of the given number.
/// \note The error propagation is similar to that of the multiplication operator: When a number
/// x is multiplied with itself n times, the maximum real error is (n+1)*x^n * 2^(-f).
/// For the cube function (n=2) this gives 3x^2 * 2^(-f) at most.
template< /* deduced: */ SqType SqT >
constexpr auto cube(SqT const &x) noexcept { return cubeImpl(x); }

/// \returns the computed cube root of the given number x, wrapped into a new Sq type with the
/// cube roots of the limits.
/// \note A hardware algorithm is used to calculate the cube root of the number, the cube root
/// of the limits is approximated via binary search.
template< /* deduced: */ SqType SqT >
constexpr auto cbrt(SqT const &x) noexcept { return cbrtImpl(x); }

/// If value compares less than lo, lo is returned; otherwise if hi compares less than value, hi is
/// returned; otherwise value is returned.
/// \returns the clamped value, wrapped into a new Sq type with the lower limit of SqLo and the upper
/// limit of SqHi.
template< /* deduced: */ SqType SqV, SqType SqLo, SqType SqHi >
requires detail::Clampable<SqV, SqLo, SqHi>
constexpr
auto clamp(SqV const &value, SqLo const &lo, SqHi const &hi) noexcept {
    using SqR = typename SqV::clamp_t<SqLo::realVMin, SqHi::realVMax>;
    // lo and hi are scaled via constructor from similar type if used; value's value just can be taken as is
    return (value < lo) ? SqR(lo) : (hi < value) ? SqR(hi) : SqR(value.value);
}

/// If value compares less than lo, lo is returned; otherwise value is returned.
/// \returns the clamped value, wrapped into a new Sq type with the lower limit of SqLo.
template< /* deduced: */ SqType SqV, SqType SqLo >
requires detail::ImplicitlyConvertible<SqLo, SqV>
constexpr
auto clampLower(SqV const &value, SqLo const &lo) noexcept {
    using SqR = typename SqV::clamp_t<SqLo::realVMin, SqV::realVMax>;
    // lo is scaled via constructor from similar type if used; value's value just can be taken as is
    return (value < lo) ? SqR(lo) : SqR(value.value);
}

/// If hi compares less than value, hi is returned; otherwise value is returned.
/// \returns the clamped value, wrapped into a new Sq type with the upper limit of SqHi.
template< /* deduced: */ SqType SqV, SqType SqHi >
requires detail::ImplicitlyConvertible<SqHi, SqV>
constexpr
auto clampUpper(SqV const &value, SqHi const &hi) noexcept {
    using SqR = typename SqV::clamp_t<SqV::realVMin, SqHi::realVMax>;
    // hi is scaled via constructor from similar type if used; value's value just can be taken as is
    return (hi < value) ? SqR(hi) : SqR(value.value);
}

/// Version of clamp() for limits known at compile-time: if value compares less than lo, lo is returned;
/// otherwise if hi compares less than value, hi is returned; otherwise value is returned.
/// \returns the clamped value, wrapped into a new Sq type with the value range from the template
/// parameters.
template< double realLo, double realHi, /* deduced: */ SqType SqV >
requires detail::RealLimitsInRangeOfBaseType<typename SqV::base_t, SqV::f, realLo, realHi>
constexpr
auto clamp(SqV const &value) noexcept {
    using SqR = typename SqV::clamp_t<realLo, realHi>;
    constexpr auto sqLo = SqR::template fromReal<realLo>;
    constexpr auto sqHi = SqR::template fromReal<realHi>;
    return (value < sqLo) ? sqLo : (sqHi < value) ? sqHi : SqR(value.value);
}

/// Version of clampLower() for lower limit known at compile-time: if value compares less than lo,
/// lo is returned; otherwise value is returned.
/// \returns the clamped value, wrapped into a new Sq type with the template parameter as lower limit.
template< double realLo, /* deduced: */ SqType SqV >
requires detail::RealLimitsInRangeOfBaseType<typename SqV::base_t, SqV::f, realLo, SqV::realVMax>
constexpr
auto clampLower(SqV const &value) noexcept {
    using SqR = typename SqV::clamp_t<realLo, SqV::realVMax>;
    constexpr auto sqLo = SqR::template fromReal<realLo>;
    return (value < sqLo) ? sqLo : SqR(value.value);
}

/// Version of clampUpper() for upper limit known at compile-time: if hi compares less than value,
/// hi is returned; otherwise value is returned.
/// \returns the clamped value, wrapped into a new Sq type with the template parameter as upper limit.
template< double realHi, /* deduced: */ SqType SqV >
requires detail::RealLimitsInRangeOfBaseType<typename SqV::base_t, SqV::f, SqV::realVMin, realHi>
constexpr
auto clampUpper(SqV const &value) noexcept {
    using SqR = typename SqV::clamp_t<SqV::realVMin, realHi>;
    constexpr auto sqHi = SqR::template fromReal<realHi>;
    return (sqHi < value) ? sqHi : SqR(value.value);
}

/// \returns the minimum value of the two given values, wrapped into a new Sq type with the minimum
/// of the limits. If both values are equivalent, the first value is returned.
template< /* deduced: */ SqType Sq1, SqType Sq2,
    double realVMinMin = std::min(Sq1::realVMin, Sq2::realVMin),
    double realVMaxMin = std::min(Sq1::realVMax, Sq2::realVMax) >
requires ( detail::Similar<Sq1, Sq2>
           && detail::RealLimitsInRangeOfBaseType<typename Sq1::base_t, Sq1::f, realVMinMin, realVMaxMin> )
constexpr
auto min(Sq1 const &first, Sq2 const &second) noexcept {
    using SqR = typename Sq1::clamp_t<realVMinMin, realVMaxMin>;
    return (first.value > second.value) ? SqR(second.value) : SqR(first.value);
}

/// \returns the maximum value of the two given values, wrapped into a new Sq type with the maximum
/// of the limits. If both values are equivalent, the first value is returned.
template< /* deduced: */ SqType Sq1, SqType Sq2,
    double realVMinMax = std::max(Sq1::realVMin, Sq2::realVMin),
    double realVMaxMax = std::max(Sq1::realVMax, Sq2::realVMax) >
requires ( detail::Similar<Sq1, Sq2>
           && detail::RealLimitsInRangeOfBaseType<typename Sq1::base_t, Sq1::f, realVMinMax, realVMaxMax> )
constexpr
auto max(Sq1 const &first, Sq2 const &second) noexcept {
    using SqR = typename Sq1::clamp_t<realVMinMax, realVMaxMax>;
    return (first.value < second.value) ? SqR(second.value) : SqR(first.value);
}

/// Converts a literal number into the corresponding best-fit sq type.
/// Best-fit means that the literal number represents both limits and the value.
template< SqType Sq, char ...charArray >
consteval auto sqFromLiteral() {
    constexpr double value = detail::doubleFromLiteral<charArray...>();
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
