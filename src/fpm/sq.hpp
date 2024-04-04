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
requires fpm::detail::QRequirements<BaseTQ, fQ, realVMinQ, realVMaxQ, ovfQ>
class Q;
}


namespace fpm::sq {
/** \ingroup grp_fpm
 * \defgroup grp_fpmSq Static Q-Type
 * \{ */

using fpm::detail::SqType;

namespace detail {



};


/// Static (safe) Q-type variant. Implements mathematical operations and checks at compile-time whether
/// these operations can be performed for the specified value range without running into overflow issues.
/// \note Usually Sq objects are constructed indirectly via Q objects, but direct construction from
/// a real or scaled constexpr value is also possible.
/// \warning Two Sq types which differ only by the sign of the 0 in a limit are not equal by design,
///          although the underlying integral value will be the same when 0. This fact is not corrected
///          for, because the outcome might be different (wrong) when the compiler is doing its
///          calculations. It's highly recommended to use -0.0 when a type is declared that has only
///          negative numbers up to 0 in its range.
template<
    std::integral BaseT,  ///< type of the scaled integer stored in memory
    scaling_t f_,         ///< number of fraction bits (precision 2^(-f))
    double realVMin_ = fpm::detail::lowestRealVMin<BaseT, f_>,  ///< minimum real value represented by the type
    double realVMax_ = fpm::detail::highestRealVMax<BaseT, f_> >  ///< maximum real value represented by the type
requires fpm::detail::SqRequirements<BaseT, f_, realVMin_, realVMax_>
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
    using clamp_t = typename clamp<newRealVMin, newRealVMax>::type;

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
    Sq fromReal() { return Sq( v2s<base_t, f>(realValue) ); }

    /// Named compile-time-only "constructor" from a scaled integer value.
    /// \note Does not compile if the value is outside the value range.
    template< base_t value >
    requires ( vMin <= value && value <= vMax )  // must not overflow
    static constexpr
    Sq fromScaled() { return Sq( value ); }

    /// Named "Copy-Constructor" from another Sq type value with the same base type.
    /// \note When an Sq value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the Sq value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< /* deduced: */ SqType SqFrom >
    requires fpm::detail::ImplicitlyConvertible<SqFrom, Sq>
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
               && fpm::detail::ImplicitlyConvertible<SqFrom, Sq> )
    constexpr
    Sq(SqFrom const &from) noexcept : value( s2s<base_t, SqFrom::f, f>(from.reveal()) ) {}

    /// Copy-Constructor from the same type.
    constexpr
    Sq(Sq const &) noexcept = default;

    /// Move-Constructor from the same type.
    constexpr
    Sq(Sq&&) noexcept = default;

    /// Destructor.
    constexpr
    ~Sq() {}

    /// Reveals the integer value stored in memory.
    constexpr
    base_t reveal() const noexcept { return value; }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning This conversion is expensive if the target type is a floating-point type.
    ///          If the target type is an integral type, there can be a significant loss of resolution.
    ///          Use carefully!
#   if defined FPM_USE_SH
    template< typename TargetT = int >
#   else
    template< typename TargetT = double >
#   endif
    constexpr
    TargetT toReal() const noexcept { return v2s<TargetT, -f>(this->value); }

    /// Implicit conversion of a Sq value back into its double representation at compile-time.
    /// Allows using a value+unit literal where a double is expected.
    consteval
    operator double() const noexcept { return toReal<double>(); }

private:
    // Implementation helper structs.
    // The public variants are designed to hide implementation specifics
    // so that hover help texts and API descriptions are not too confusing.

    /// Implements the cast of Sq to type SqC.
    template< class SqC >
    struct Cast {
        using base_t = typename SqC::base_t;
        static constexpr scaling_t f = SqC::f;
        static constexpr double realVMin = SqC::realVMin;
        static constexpr double realVMax = SqC::realVMax;
        static constexpr base_t from(typename Sq::base_t v) noexcept {
            return s2s<base_t, Sq::f, f>(v);  // scale value
        }
    };

    /// Implements the unary minus operator.
    struct UnaryM {
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = -Sq::realVMax;
        static constexpr double realVMax = -Sq::realVMin;
        using base_t = fpm::detail::common_q_base_t<typename Sq::base_t, std::make_signed_t<typename Sq::base_t>, f, realVMin, realVMax>;
        static constexpr base_t op(typename Sq::base_t v) noexcept {
            return static_cast<base_t>( -v );
        }
    };

    /// Implements the addition operator.
    template< SqType SqRhs >
    struct Add {
        static constexpr scaling_t f = std::max(Sq::f, SqRhs::f);
        static constexpr double realVMin = Sq::realVMin + SqRhs::realVMin;
        static constexpr double realVMax = Sq::realVMax + SqRhs::realVMax;
        using base_t = fpm::detail::common_q_base_t<typename Sq::base_t, typename SqRhs::base_t, f, realVMin, realVMax>;
        static constexpr base_t op(typename Sq::base_t lv, typename SqRhs::base_t rv) noexcept {
            return static_cast<base_t>( s2s<base_t, Sq::f, f>(lv) + s2s<base_t, SqRhs::f, f>(rv) );  // add values
        }
    };

    /// Implements the subtraction operator.
    template< SqType SqRhs >
    struct Sub {
        static constexpr scaling_t f = std::max(Sq::f, SqRhs::f);
        static constexpr double realVMin = std::min(Sq::realVMin - SqRhs::realVMax, SqRhs::realVMin - Sq::realVMax);
        static constexpr double realVMax = std::max(Sq::realVMax - SqRhs::realVMin, SqRhs::realVMax - Sq::realVMin);
        using base_t = fpm::detail::common_q_base_t<typename Sq::base_t, typename SqRhs::base_t, f, realVMin, realVMax>;
        static constexpr base_t op(typename Sq::base_t lv, typename SqRhs::base_t rv) noexcept {
            return static_cast<base_t>( s2s<base_t, Sq::f, f>(lv) - s2s<base_t, SqRhs::f, f>(rv) );  // subtract rv from lv
        }
    };

    /// Implements the multiplication operator.
    template< SqType SqRhs >
    struct Mult {
        static constexpr scaling_t f = std::max(Sq::f, SqRhs::f);
        static constexpr double realVMin = std::min(std::min(Sq::realVMin * SqRhs::realVMax, SqRhs::realVMin * Sq::realVMax),
                                                    std::min(Sq::realVMin * SqRhs::realVMin, Sq::realVMax * SqRhs::realVMax));
        static constexpr double realVMax = std::max(std::max(Sq::realVMax * SqRhs::realVMin, SqRhs::realVMax * Sq::realVMin),
                                                    std::max(Sq::realVMin * SqRhs::realVMin, Sq::realVMax * SqRhs::realVMax));
        using base_t = fpm::detail::common_q_base_t<typename Sq::base_t, typename SqRhs::base_t, f, realVMin, realVMax>;
        static constexpr base_t op(typename Sq::base_t lv, typename SqRhs::base_t rv) noexcept {
            // multiply lhs with rhs in intermediate type and correct scaling to obtain result
            // a*b <=> (a * 2^f) * (b * 2^f) / 2^f = a*b * 2^f
            return s2s<base_t, 2*f, f>( s2s<interm_t<base_t>, Sq::f, f>(lv) * s2s<interm_t<base_t>, SqRhs::f, f>(rv) );
        }
    };

    /// Implements the multiplication operators for the multiplication of Sq with an integer constant.
    template< std::integral T, T ic >
    struct MultIc {
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = std::min(Sq::realVMin * ic, Sq::realVMax * ic);
        static constexpr double realVMax = std::max(Sq::realVMin * ic, Sq::realVMax * ic);
        using base_t = fpm::detail::common_q_base_t<typename Sq::base_t, T, f, realVMin, realVMax>;
        static constexpr base_t opR(typename Sq::base_t const lv, std::integral_constant<T, ic>) noexcept {
            return static_cast<base_t>(lv) * static_cast<base_t>(ic);  // multiply lhs value with the integral constant
        }
        static constexpr base_t opL(std::integral_constant<T, ic>, typename Sq::base_t const rv) noexcept {
            return static_cast<base_t>(ic) * static_cast<base_t>(rv);  // multiply rhs value with the integral constant
        }
    };

    /// Implements the division operator of Sq by SqRhs.
    template< SqType SqRhs >
    struct Div {
        static constexpr scaling_t f = std::max(Sq::f, SqRhs::f);
        static constexpr double realVMin = std::min(std::min(Sq::realVMin / SqRhs::realVMax, Sq::realVMin / SqRhs::realVMin),
                                                    std::min(Sq::realVMax / SqRhs::realVMin, Sq::realVMax / SqRhs::realVMax));
        static constexpr double realVMax = std::max(std::max(Sq::realVMin / SqRhs::realVMax, Sq::realVMin / SqRhs::realVMin),
                                                    std::max(Sq::realVMax / SqRhs::realVMin, Sq::realVMax / SqRhs::realVMax));
        using base_t = fpm::detail::common_q_base_t<typename Sq::base_t, typename SqRhs::base_t, f, realVMin, realVMax>;
        static constexpr base_t op(typename Sq::base_t lv, typename SqRhs::base_t rv) noexcept {
            // divide lhs by rhs in intermediate type and correct scaling to obtain result
            // a/b <=> (a * 2^(2f)) / (b * 2^f) = a/b * 2^f
            return static_cast<base_t>( s2s<interm_t<base_t>, Sq::f, 2*f>(lv) / s2s<interm_t<base_t>, SqRhs::f, f>(rv) );
        };
    };

    /// Implements the division operator for the division of Sq by an integer constant.
    template< std::integral T, T ic >
    struct DivIcR {
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = std::min(Sq::realVMin / ic, Sq::realVMax / ic);
        static constexpr double realVMax = std::max(Sq::realVMin / ic, Sq::realVMax / ic);
        using base_t = fpm::detail::common_q_base_t<typename Sq::base_t, T, f, realVMin, realVMax>;
        static constexpr base_t op(typename Sq::base_t lv, std::integral_constant<T, ic>) noexcept {
            return lv / static_cast<base_t>(ic);  // divide lhs value by the integral constant
        }
    };

    /// Implements the division operator for the division of an integer constant by Sq.
    template< std::integral T, T ic >
    struct DivIcL {
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = std::min(ic / Sq::realVMin, ic / Sq::realVMax);
        static constexpr double realVMax = std::max(ic / Sq::realVMin, ic / Sq::realVMax);
        using base_t = fpm::detail::common_q_base_t<typename Sq::base_t, T, f, realVMin, realVMax>;
        using interm_t = interm_t<base_t>;
        static constexpr base_t op(std::integral_constant<T, ic>, typename Sq::base_t rv) noexcept {
            // ic * 2^(2f) / (v*2^f) = ic/v * 2^f
            return static_cast<base_t>( v2s<interm_t, 2*f>(ic) / static_cast<interm_t>(rv) );
        }
    };

    /// Implements the modulo operator of Sq by SqRhs.
    template< SqType SqRhs >
    struct Mod {
        static constexpr scaling_t f = std::max(Sq::f, SqRhs::f);
        static constexpr double realVMin = std::max(Sq::realVMin, fpm::detail::signum(Sq::realVMin) *
                                                    std::max(fpm::detail::abs(SqRhs::realVMin), fpm::detail::abs(SqRhs::realVMax)));
        static constexpr double realVMax = std::min(Sq::realVMax, fpm::detail::signum(Sq::realVMax) *
                                                    std::max(fpm::detail::abs(SqRhs::realVMin), fpm::detail::abs(SqRhs::realVMax)));
        using base_t = fpm::detail::common_q_base_t<Sq::base_t, typename SqRhs::base_t, f, realVMin, realVMax>;
        static constexpr base_t op(typename Sq::base_t lv, typename SqRhs::base_t rv) noexcept {
            // divide lhs by rhs in intermediate type and correct scaling to obtain result
            // a%b <=> (a * 2^f) % (b * 2^f) = a%b * 2^f
            return static_cast<base_t>( s2s<interm_t<base_t>, Sq::f, f>(lv) % s2s<interm_t<base_t>, SqRhs::f, f>(rv) );
        }
    };

    /// Implements the equality operator between Sq and SqRhs.
    template< class SqRhs >
    struct Equal {
        using base_t = std::common_type_t<typename Sq::base_t, typename SqRhs::base_t>;
        static constexpr scaling_t f = std::max(Sq::f, SqRhs::f);
        static constexpr double realVMin = Sq::realVMin;
        static constexpr double realVMax = Sq::realVMax;
        static constexpr base_t op(typename Sq::base_t lv, typename SqRhs::base_t rv) noexcept {
            // two values are considered equal if the values, scaled to the higher resolution, are equivalent
            return s2s<base_t, Sq::f, f>(lv) == s2s<base_t, SqRhs::f, f>(rv);
        }
    };

    /// Implements the spaceship operator (three-way comparison) between Sq and SqRhs.
    template< class SqRhs >
    struct Spaceship {
        using base_t = std::common_type_t<typename Sq::base_t, typename SqRhs::base_t>;
        static constexpr scaling_t f = std::max(Sq::f, SqRhs::f);
        static constexpr double realVMin = Sq::realVMin;
        static constexpr double realVMax = Sq::realVMax;
        static constexpr std::strong_ordering op(typename Sq::base_t lv, typename SqRhs::base_t rv) noexcept {
            // the two values are ordered with the higher resolution
            return s2s<base_t, Sq::f, f>(lv) <=> s2s<base_t, SqRhs::f, f>(rv);
        }
    };

    /// Implements the left shift operator of Sq by the value of an integral constant.
    template< std::integral T, T ic >
    struct ShiftL {
        using base_t = typename Sq::base_t;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = v2s<double, -f>(static_cast<interm_t<base_t>>(Sq::vMin) << ic);
        static constexpr double realVMax = v2s<double, -f>(static_cast<interm_t<base_t>>(Sq::vMax) << ic);
        static constexpr base_t op(typename Sq::base_t lv, std::integral_constant<T, ic>) noexcept {
            return lv << ic;
        }
    };

    /// Implements the right shift operator of Sq by the value of an integral constant.
    template< std::integral T, T ic >
    struct ShiftR {
        using base_t = typename Sq::base_t;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = v2s<double, -f>(static_cast<interm_t<base_t>>(Sq::vMin) >> ic);
        static constexpr double realVMax = v2s<double, -f>(static_cast<interm_t<base_t>>(Sq::vMax) >> ic);
        static constexpr base_t op(typename Sq::base_t lv, std::integral_constant<T, ic>) noexcept {
            return lv >> ic;
        }
    };

    /// Implements the abs function.
    struct Abs {
        using base_t = std::make_unsigned_t<typename Sq::base_t>;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = (std::is_signed_v<typename Sq::base_t> && Sq::vMin < 0 && Sq::vMax > 0)
            ? 0.0  // use 0 as new minimum if signed input type has a range of negative and positive values
            : std::min(fpm::detail::abs(Sq::realVMin), fpm::detail::abs(Sq::realVMax));
        static constexpr double realVMax = std::max(fpm::detail::abs(Sq::realVMin), fpm::detail::abs(Sq::realVMax));
        static constexpr base_t op(typename Sq::base_t v) noexcept {
            if constexpr (std::is_unsigned_v<typename Sq::base_t>) { return v; }
            else { return std::abs(v); }
        }
    };

    /// Implements the square function.
    struct Square {
        using base_t = std::common_type_t<int32_t, typename Sq::base_t>;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = (std::is_signed_v<base_t> && Sq::vMin < 0 && Sq::vMax > 0)
            ? 0.0  // use 0 as new minimum if signed input type has a range of negative and positive values
            : std::min(Sq::realVMin*Sq::realVMin, Sq::realVMax*Sq::realVMax);
        static constexpr double realVMax = std::max(Sq::realVMin*Sq::realVMin, Sq::realVMax*Sq::realVMax);
        static constexpr base_t op(typename Sq::base_t v) noexcept {
            // x^2 <=> [ (x*2^f)*(x*2^f) / 2^f ] = x*x*2^f
            auto const vIntm = static_cast<interm_t<base_t>>(v);
            constexpr auto fPower = v2s<interm_t<base_t>, f>(1);
            return static_cast<base_t>( vIntm*vIntm / fPower );
        }
    };

    /// Implements the square root function.
    struct Sqrt {
        using base_t = typename Sq::base_t;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = fpm::detail::floor( fpm::detail::sqrt(Sq::realVMin) );
        static constexpr double realVMax = fpm::detail::ceil( fpm::detail::sqrt(Sq::realVMax) );
        static constexpr base_t op(typename Sq::base_t v) noexcept {
            if (v <= 0) { return 0; }  // negative value has imaginary root, real part is 0
            else {
                // take root of corrected number; result can be cast to base_t without truncation
                // sqrt(x) <=> [ ((x*2^f) * 2^f)^1/2 ] = x^1/2 * 2^f
                auto xIntm = static_cast<interm_t<base_t>>(v);
                constexpr auto fPower = v2s<interm_t<base_t>, f>(1);
                return static_cast<base_t>( fpm::detail::isqrt( xIntm * fPower ) );
            }
        }
    };

    /// Implements the reverse square root function.
    struct RSqrt {
        using base_t = typename Sq::base_t;
        static constexpr scaling_t f = Sq::f;
        static constexpr double thMax = fpm::detail::highestRealVMax<base_t, f>();
        static constexpr double realVMin = fpm::detail::floor( fpm::detail::rsqrt(Sq::realVMax) );
        static constexpr double realVMax = std::min( thMax, fpm::detail::ceil( fpm::detail::rsqrt(Sq::realVMin) ) );
        static constexpr auto op(typename Sq::base_t v) noexcept {
            // too small number results in theoretical maximum; this is the case if x*2^f <= (2^f / max^2)
            constexpr base_t limit = v2s<base_t, f>( 1. / thMax / thMax );
            return v < limit
                ? v2s<base_t, f>(thMax)
                // 1/sqrt(x) <=> [ 2^(2f) / ((x*2^f) * 2^f)^1/2 ] = 2^f / sqrt(x)
                : static_cast<base_t>( v2s<interm_t<base_t>, 2*f>(1) / static_cast<interm_t<base_t>>( Sqrt::op(v) ) );
        }
    };

    /// Implements the cube function.
    struct Cube {
        using base_t = std::common_type_t<int32_t, Sq::base_t>;
        static constexpr scaling_t f = Sq::f;
        static constexpr double l1 = Sq::realVMin*Sq::realVMin*Sq::realVMin;
        static constexpr double l2 = Sq::realVMin*Sq::realVMin*Sq::realVMax;
        static constexpr double l3 = Sq::realVMin*Sq::realVMax*Sq::realVMax;
        static constexpr double l4 = Sq::realVMax*Sq::realVMax*Sq::realVMax;
        static constexpr double realVMin = std::min(std::min(std::min(l1, l2), l3), l4);
        static constexpr double realVMax = std::max(std::max(std::max(l1, l2), l3), l4);
        static constexpr base_t op(typename Sq::base_t v) noexcept {
            // x^3 <=> [ (x*2^f)*(x*2^f) / 2^f * (x*2^f) / 2^f ] = [ square(x)*(x*2^f) / 2^f ] = x*x*x*2^f
            auto xIntm = static_cast<interm_t<base_t>>(v);
            auto xSqr = static_cast<interm_t<base_t>>( Square::op(v) );
            constexpr auto fPower = v2s<interm_t<base_t>, f>(1);
            return static_cast<base_t>( xSqr*xIntm / fPower );
        }
    };

    /// Implements the cube root function.
    struct Cbrt {
        using base_t = typename Sq::base_t;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = (Sq::realVMin < 0. ? 0. : fpm::detail::floor( fpm::detail::cbrt(Sq::realVMin) ));
        static constexpr double realVMax = (Sq::realVMax < 0. ? 0. : fpm::detail::ceil( fpm::detail::cbrt(Sq::realVMax) ));
        static constexpr base_t op(typename Sq::base_t v) noexcept {
            if (v <= 0) { return 0; }  // negative number is out of scope for hardware algorithm icbrt
            else {
                // cbrt(x) <=> [ ((x*2^f) * 2^f * 2^f)^1/3 ] = x^1/3 * 2^f
                auto xIntm = static_cast<interm_t<base_t>>(v);
                constexpr auto fPower = v2s<interm_t<base_t>, f>(1);
                return static_cast<base_t>( fpm::detail::icbrt( xIntm * fPower * fPower ) );
            }
        }
    };

    /// Implements the clamp functions with Sq limits. This version is capable of runtime conversions
    /// between different Sq types.
    template< SqType SqLo, SqType SqHi >
    struct Clamp {
        using base_t = typename Sq::base_t;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = SqLo::realVMin;
        static constexpr double realVMax = SqHi::realVMax;
        static constexpr base_t range(Sq const &v, SqLo const &lo, SqHi const &hi) noexcept {
            return (v < lo) ? s2s<base_t, SqLo::f, f>(lo.value) : (hi < v) ? s2s<base_t, SqHi::f, f>(hi.value) : v.value;
        };
        static constexpr base_t lower(Sq const &v, SqLo const &lo) noexcept {
            return (v < lo) ? s2s<base_t, SqLo::f, f>(lo.value) : v.value;
        };
        static constexpr base_t upper(Sq const &v, SqHi const &hi) noexcept {
            return (hi < v) ? s2s<base_t, SqHi::f, f>(hi.value) : v.value;
        };
    };

    /// Implements the clamp functions with real limits (double).
    template< double realLo, double realHi >
    struct CTClamp {
        using base_t = typename Sq::base_t;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = realLo;
        static constexpr double realVMax = realHi;
        static constexpr base_t lo = v2s<base_t, f>(realVMin);
        static constexpr base_t hi = v2s<base_t, f>(realVMax);
        static constexpr base_t range(typename Sq::base_t v) noexcept {
            return (v < lo) ? lo : (hi < v) ? hi : v;
        };
        static constexpr base_t lower(typename Sq::base_t v) noexcept {
            return (v < lo) ? lo : v;
        };
        static constexpr base_t upper(typename Sq::base_t v) noexcept {
            return (hi < v) ? hi : v;
        };
    };

    /// Implements the min function.
    template< SqType Sq2 >
    requires fpm::detail::Similar<Sq, Sq2>
    struct Min {
        using base_t = typename Sq::base_t;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = std::min(Sq::realVMin, Sq2::realVMin);
        static constexpr double realVMax = std::min(Sq::realVMax, Sq2::realVMax);
        static constexpr base_t min(typename Sq::base_t v1, typename Sq2::base_t v2) noexcept {
            return (v1 > v2) ? v2 : v1;
        }
    };

    /// Implements the max function.
    template< SqType Sq2 >
    requires fpm::detail::Similar<Sq, Sq2>
    struct Max {
        using base_t = typename Sq::base_t;
        static constexpr scaling_t f = Sq::f;
        static constexpr double realVMin = std::max(Sq::realVMin, Sq2::realVMin);
        static constexpr double realVMax = std::max(Sq::realVMax, Sq2::realVMax);
        static constexpr base_t max(typename Sq::base_t v1, typename Sq2::base_t v2) noexcept {
            return (v1 < v2) ? v2 : v1;
        }
    };

public:
    /// Explicit static and safe cast to a different Sq type with a different base type.
    /// Only possible if the value can be cast safely without any potential overflow, i.e. if the
    /// target value range is equal to or larger than the value range of this class.
    /// \note If a cast does not work it's most probably due to unfulfilled requirements.
    template< /* deduced: */ SqType SqC >
    requires fpm::detail::CastableWithoutChecks<Sq, SqC>
    explicit constexpr
    operator SqC() const noexcept { return SqC( Cast<SqC>::from(this->value) ); }

    /// Explicit static cast to another Sq type with a different base type.
    /// Uses static_cast internally. Exists for consistency reasons.
    template< SqType SqC > requires fpm::detail::CastableWithoutChecks<Sq, SqC>
    friend constexpr
    SqC static_sq_cast(Sq from) noexcept { return static_cast<SqC>(from); }

    /// Explicit safe cast to another Sq type with a different base type.
    /// Uses static_cast internally. Exists for consistency reasons.
    template< SqType SqC >
    requires fpm::detail::CastableWithoutChecks<Sq, SqC>
    friend constexpr
    SqC safe_sq_cast(Sq from) noexcept { return static_cast<SqC>(from); }

    /// Unary plus operator. Integral promotion does not make any sense, so this just creates a copy.
    /// \returns a copy of the value with the same type.
    constexpr
    Sq operator +() const noexcept { return Sq( this->value ); }

    /// Unary minus operator. Negates the type and its value.
    /// \note A signed type can be negated if the corresponding INT_MIN value is not in the value
    /// range of the type. An unsigned type is promoted to a new type with a sized base integer that
    /// has either the same size if possible, or twice the size (e.g. u16 -> i32).
    /// \note For a chain of n subtractions, the propagated error is approximately n * 2^(-f) plus the
    /// representation error of the initial value.
    /// \warning a +0.0 in the limits will be negated to -0.0 and vice versa. Note that two Sq types
    ///          which differ only by the sign of the 0 in a limit are not equal by design!
    /// \returns the negated value, wrapped into a new Sq type with negated limits.
    constexpr
    auto operator -() const noexcept
    requires fpm::detail::Absolutizable<base_t, vMin> {
        return Sq<typename UnaryM::base_t, UnaryM::f, UnaryM::realVMin, UnaryM::realVMax>( UnaryM::op(this->value) );
    }

    /// Adds two Sq values.
    /// \returns the sum, wrapped into a new Sq type with the larger scaling (higher precision) and
    /// the value ranges added together. For the base type of the result a common type is determined
    /// that can hold the lowest and highest value of the resulting value range.
    // Note: Passing lhs by value helps optimize chained a+b+c.
    template< /* deduced: */ SqType SqRhs >
    requires fpm::detail::ValidImplType< Add<SqRhs> >
    friend constexpr
    auto operator +(Sq const lhs, SqRhs const &rhs) noexcept {
        using AddT = Add<SqRhs>;
        return Sq<typename AddT::base_t, AddT::f, AddT::realVMin, AddT::realVMax>( AddT::op(lhs.value, rhs.value) );
    }

    /// Subtracts the rhs value from the lhs value.
    /// \returns the difference, wrapped into a new Sq type with the larger scaling (higher precision)
    /// and the value ranges subtracted. For the base type of the result a common type is determined
    /// that can hold the lowest and highest value of the resulting value range.
    /// \note For a chain of n additions, the propagated error is approximately n * 2^(-f) plus the
    /// representation error of the initial value.
    // Note: Passing lhs by value helps optimize chained a-b-c.
    template< /* deduced: */ SqType SqRhs >
    requires fpm::detail::ValidImplType< Sub<SqRhs> >
    friend constexpr
    auto operator -(Sq const lhs, SqRhs const &rhs) noexcept {
        using SubT = Sub<SqRhs>;
        return Sq<typename SubT::base_t, SubT::f, SubT::realVMin, SubT::realVMax>( SubT::op(lhs.value, rhs.value) );
    }

    /// Multiplies the lhs value with the rhs value.
    /// \returns the product, wrapped into a new Sq type with the larger scaling (higher precision)
    /// and the value ranges multiplied. For the base type of the result a common type is determined
    /// that can hold the lowest and highest value of the resulting value range.
    /// \note The error propagation is complicated. When a number x is multiplied with itself n times,
    /// the real error is (n+1)*x^n * 2^(-f). For example, for a chain x*x*x (n=2) the real error is
    /// 3*x^2 * 2^(-f). Higher terms O( 2^(-m*f) ), m > 1 also occur for chains, but normally these
    /// are very close to 0 (when f is large enough) and can be ignored.
    // Note: Passing lhs by value helps optimize chained a*b*c.
    template< /* deduced: */ SqType SqRhs >
    requires fpm::detail::ValidImplType< Mult<SqRhs> >
    friend constexpr
    auto operator *(Sq const lhs, SqRhs const &rhs) noexcept {
        using MultT = Mult<SqRhs>;
        return Sq<typename MultT::base_t, MultT::f, MultT::realVMin, MultT::realVMax>( MultT::op(lhs.value, rhs.value) );
    }

    /// Multiplies the lhs Sq value with the rhs integral constant.
    /// \returns the product, wrapped in a new Sq type with the common base type and a value range
    /// scaled by the same integral constant.
    // Note: Passing lhs by value helps optimize chained a*b*c.
    template< /* deduced: */ std::integral T, T ic >
    requires fpm::detail::ValidImplType< MultIc<T,ic> >
    friend constexpr
    auto operator *(Sq const lhs, std::integral_constant<T, ic> iv) noexcept {
        using MultT = MultIc<T,ic>;
        return Sq<typename MultT::base_t, MultIc<T,ic>::f, MultT::realVMin, MultT::realVMax>( MultT::opR(lhs.value, iv) );
    }

    /// Multiplies the lhs integral constant with the rhs Sq value.
    /// \returns the product, wrapped in a new Sq type with the common base type and a value range
    /// scaled by the same integral constant.
    template< /* deduced: */ std::integral T, T ic >
    requires fpm::detail::ValidImplType< MultIc<T,ic> >
    friend constexpr
    auto operator *(std::integral_constant<T, ic> iv, Sq const &rhs) noexcept {
        using MultT = MultIc<T,ic>;
        return Sq<typename MultT::base_t, MultIc<T,ic>::f, MultT::realVMin, MultT::realVMax>( MultT::opL(iv, rhs.value) );
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
    // Note: Passing lhs by value helps optimize chained a/b/c.
    template< /* deduced: */ SqType SqRhs >
    requires ( fpm::detail::CanBeUsedAsDivisor<SqRhs> && fpm::detail::ValidImplType< Div<SqRhs> > )
    friend constexpr
    auto operator /(Sq const lhs, SqRhs const &rhs) noexcept {
        using DivT = Div<SqRhs>;
        return Sq<typename DivT::base_t, DivT::f, DivT::realVMin, DivT::realVMax>( DivT::op(lhs.value, rhs.value) );
    }

    /// Divides the lhs Sq value by the rhs integral constant.
    /// \returns the quotient, wrapped in a new Sq type with the common base type and a value range
    /// divided by the same integral constant.
    /// \warning Arithmetic underflow can happen if the result is smaller than the target resolution.
    /// \warning To ensure that compile-time overflow checks are not required, the rhs constant must
    ///          not be 0.
    // Note: Passing lhs by value helps optimize chained a/b/c.
    template< /* deduced: */ std::integral T, T ic >
    requires ( ic != 0 && fpm::detail::ValidImplType< DivIcR<T,ic> > )
    friend constexpr
    auto operator /(Sq const lhs, std::integral_constant<T, ic> iv) noexcept {
        using DivT = DivIcR<T,ic>;
        return Sq<typename DivT::base_t, DivT::f, DivT::realVMin, DivT::realVMax>( DivT::op(lhs.value, iv) );
    }

    /// Divides the lhs integral constant by the rhs Sq value.
    /// \returns the quotient, wrapped in a new Sq type with the common base type and a value range
    /// divided in the same way.
    /// \warning Arithmetic underflow can happen if the result is smaller than the target resolution.
    /// \warning To ensure that compile-time overflow checks are not required, the rhs type must not
    ///          have values between -1 and +1 in its value range.
    template< /* deduced: */ std::integral T, T ic >
    requires ( fpm::detail::CanBeUsedAsDivisor<Sq> && fpm::detail::ValidImplType< DivIcL<T,ic> >
               && v2s<typename DivIcL<T,ic>::interm_t, 2*f>(ic) <= std::numeric_limits<typename DivIcL<T,ic>::interm_t>::max() )
    friend constexpr
    auto operator /(std::integral_constant<T, ic> iv, Sq const &rhs) noexcept {
        using DivT = DivIcL<T,ic>;
        return Sq<typename DivT::base_t, DivT::f, DivT::realVMin, DivT::realVMax>( DivT::op(iv, rhs.value) );
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
    // Note: Passing lhs by value helps optimize chained a%b%c.
    template< /* deduced: */ SqType SqRhs >
    requires ( fpm::detail::CanBeUsedAsModulusDivisor<SqRhs> && fpm::detail::ValidImplType< Mod<SqRhs> > )
    friend constexpr
    auto operator %(Sq const lhs, SqRhs const &rhs) noexcept {
        using ModT = Mod<SqRhs>;
        return Sq<typename ModT::base_t, ModT::f, ModT::realVMin, ModT::realVMax>( ModT::op(lhs.value, rhs.value) );
    }

    /// Primary operator to compare for equality. The inequality operator (!=) is synthesized from
    /// this operator. The input values are scaled to the higher resolution of the two input types
    /// before being compared.
    /// \note Comparison is possible if both types have the same signedness, or if the size of the
    /// lhs type is larger than the size of the rhs type if the signedness is different.
    /// The common type is used for the comparison in these cases.
    /// \warning If two real values ​​are compared that are closer together than the higher resolution,
    ///          the result may be true instead of false.
    template< /* deduced: */ SqType SqRhs >
    requires ( fpm::detail::Comparable<base_t, typename SqRhs::base_t> && fpm::detail::ValidImplType< Equal<SqRhs> > )
    constexpr
    bool operator ==(SqRhs const &rhs) const noexcept { return Equal<SqRhs>::op(this->value, rhs.value); }

    /// Convenient three-way ordering operator. The secondary relational operators <, <=, > and >=
    /// are synthesized from this operator. The input values are scaled to the higher resolution
    /// before being ordered.
    /// \note Ordering is possible if both types have the same signedness, or if the size of the
    /// lhs type is larger than the size of the rhs type if the signedness is different.
    /// The common type is used for the ordering in these cases.
    /// \warning If two real values ​​are ordered that are closer together than the resolution, the
    ///          result may be false instead of true (i.e. the values may be considered equal).
    template< /* deduced: */ SqType SqRhs >
    requires ( fpm::detail::Comparable<base_t, typename SqRhs::base_t> && fpm::detail::ValidImplType< Equal<SqRhs> > )
    constexpr
    std::strong_ordering operator <=>(SqRhs const &rhs) const noexcept {
        return Spaceship<SqRhs>::op(this->value, rhs.value);
    }

    /// Left-Shifts lhs by the number of bits given by the rhs integral constant.
    /// \note Shifting is possible if the shifted limits do not exceed the base type's value range.
    /// \warning Arithmetic underflow can happen if the result is smaller than the target resolution.
    /// \returns the shifted value, wrapped into a new Sq type with shifted limits.
    template< /* deduced: */ std::integral T, T ic >
    requires ( std::is_unsigned_v<T> && fpm::detail::ValidImplType< ShiftL<T,ic> > )
    constexpr
    auto operator <<(std::integral_constant<T, ic> iv) const noexcept {
        using ShftLT = ShiftL<T,ic>;
        return Sq<typename ShftLT::base_t, ShftLT::f, ShftLT::realVMin, ShftLT::realVMax>( ShftLT::op(this->value, iv) );
    }

    /// Right-Shifts lhs by the number of bits given by the rhs integral constant.
    /// \note Shifting is possible if the shifted limits do not exceed the base type's value range.
    /// \returns the shifted value, wrapped into a new Sq type with shifted limits.
    template< /* deduced: */ std::integral T, T ic >
    requires ( std::is_unsigned_v<T> && fpm::detail::ValidImplType< ShiftR<T,ic> > )
    constexpr
    auto operator >>(std::integral_constant<T, ic> iv) const noexcept {
        using ShftRL = ShiftR<T,ic>;
        return Sq<typename ShftRL::base_t, ShftRL::f, ShftRL::realVMin, ShftRL::realVMax>( ShftRL::op(this->value, iv) );
    }

    /// Takes the absolute value of the given Sq value.
    /// \note The absolute value of a signed type can be taken if the corresponding INT_MIN value is
    /// not in the value range of the Sq type.
    /// \returns the absolute value, wrapped into a new unsigned Sq type with a modified range.
    template< SqType SqT = Sq >
    requires ( fpm::detail::Absolutizable<base_t, vMin> && fpm::detail::ValidImplType< Abs > )
    friend constexpr
    auto abs(Sq const &of) noexcept {
        return Sq<typename Abs::base_t, Abs::f, Abs::realVMin, Abs::realVMax>( Abs::op(of.value) );
    }

    /// \returns the squared value of the given number x, wrapped into a new Sq type with at least
    /// 32 bits base type and squared limits. The base type of the resulting value is the common type
    /// of int32 and the base type of the given number.
    /// \note The error propagation is similar to that of the multiplication operator: When a number
    /// x is multiplied with itself n times, the maximum real error is (n+1)*x^n * 2^(-f).
    /// For the square function (n=1) this gives 2x * 2^(-f) at most.
    friend constexpr
    auto square(Sq const &x) noexcept
    requires fpm::detail::ValidImplType< Square > {
        return Sq<typename Square::base_t, Square::f, Square::realVMin, Square::realVMax>( Square::op(x.value) );
    }

    /// \returns the computed square root of the given number x, wrapped into a new Sq type with the
    /// square roots of the limits.
    /// \note A binary search algorithm is used to calculate the square root in logarithmic time.
    friend constexpr
    auto sqrt(Sq const &x) noexcept
    requires ( fpm::detail::CanBePassedToSqrt< Sq > && fpm::detail::ValidImplType< Sqrt > ) {
        return Sq<typename Sqrt::base_t, Sqrt::f, Sqrt::realVMin, Sqrt::realVMax>( Sqrt::op(x.value) );
    }

    /// \returns the computed reciprocal square root of the given number x, wrapped into a new Sq type
    /// with the reciprocal square root of the limits. The maximum limit of the resulting type does not
    /// exceed the value range of its base type.
    /// \warning This uses a division. May be expensive!
    /// \note The maximum real error evaluates to 2^(-f) * ( 1/2*(1/x)^(3/2) - O( 2^(-f)*(1/x)^(5/2) ) ).
    /// Unless x is very small ( x < 1 / 2^((2f+2)/3) ), it is usually enough to estimate the real
    /// error with the resolution of the given number.
    friend constexpr
    auto rsqrt(Sq const &x) noexcept
    requires ( fpm::detail::CanBePassedToRSqrt< Sq > && fpm::detail::ValidImplType< RSqrt > ) {
        return Sq<typename RSqrt::base_t, RSqrt::f, RSqrt::realVMin, RSqrt::realVMax>( RSqrt::op(x.value) );
    }

    /// \returns the cube of the given number x, wrapped into a new Sq type with at least 32 bits
    /// base type and cubed limits. The base type of the resulting value is the common type of int32
    /// and the base type of the given number.
    /// \note The error propagation is similar to that of the multiplication operator: When a number
    /// x is multiplied with itself n times, the maximum real error is (n+1)*x^n * 2^(-f).
    /// For the cube function (n=2) this gives 3x^2 * 2^(-f) at most.
    friend constexpr
    auto cube(Sq const &x) noexcept
    requires fpm::detail::ValidImplType< Cube > {
        return Sq<typename Cube::base_t, Cube::f, Cube::realVMin, Cube::realVMax>( Cube::op(x.value) );
    }

    /// \returns the computed cube root of the given number x, wrapped into a new Sq type with the
    /// cube roots of the limits.
    /// \note A hardware algorithm is used to calculate the cube root of the number, the cube root
    /// of the limits is approximated via binary search.
    friend constexpr
    auto cbrt(Sq const &x) noexcept
    requires ( fpm::detail::CanBePassedToCbrt< Sq > && fpm::detail::ValidImplType< Cbrt > ) {
        return Sq<typename Cbrt::base_t, Cbrt::f, Cbrt::realVMin, Cbrt::realVMax>( Cbrt::op(x.value) );
    }

    /// If v compares less than lo, lo is returned; otherwise if hi compares less than v, hi is
    /// returned; otherwise v is returned.
    /// \returns clamped v, wrapped into a new Sq type with the lower limit of SqLo and the upper
    /// limit of SqHi.
    template< /* deduced: */ SqType SqLo, SqType SqHi >
    requires fpm::detail::Clampable<Sq, SqLo, SqHi>
    friend constexpr
    auto clamp(Sq const &v, SqLo const &lo, SqHi const &hi) noexcept {
        using ClmpT = Clamp<SqLo, SqHi>;
        return Sq<typename ClmpT::base_t, ClmpT::f, ClmpT::realVMin, ClmpT::realVMax>( ClmpT::range(v, lo, hi) );
    }

    /// If v compares less than lo, lo is returned; otherwise v is returned.
    /// \returns clamped v, wrapped into a new Sq type with the lower limit of SqLo.
    template< /* deduced: */ SqType SqLo >
    requires fpm::detail::ImplicitlyConvertible<SqLo, Sq>
    friend constexpr
    auto clampLower(Sq const &v, SqLo const &lo) noexcept {
        using ClmpT = Clamp<SqLo, Sq>;
        return Sq<typename ClmpT::base_t, ClmpT::f, ClmpT::realVMin, ClmpT::realVMax>( ClmpT::lower(v, lo) );
    }

    /// If hi compares less than v, hi is returned; otherwise v is returned.
    /// \returns clamped v, wrapped into a new Sq type with the upper limit of SqHi.
    template< /* deduced: */ SqType SqHi >
    requires fpm::detail::ImplicitlyConvertible<SqHi, Sq>
    friend constexpr
    auto clampUpper(Sq const &v, SqHi const &hi) noexcept {
        using ClmpT = Clamp<Sq, SqHi>;
        return Sq<typename ClmpT::base_t, ClmpT::f, ClmpT::realVMin, ClmpT::realVMax>( ClmpT::upper(v, hi) );
    }

    /// Version of clamp() for limits known at compile-time: if v compares less than lo, lo is returned;
    /// otherwise if hi compares less than v, hi is returned; otherwise v is returned.
    /// \returns clamped v, wrapped into a new Sq type with the value range from the template parameters.
    template< double realLo, double realHi >
    requires fpm::detail::ValidImplType< CTClamp<realLo, realHi> >
    friend constexpr
    auto clamp(Sq const &v) noexcept {
        using ClmpT = CTClamp<realLo, realHi>;
        return Sq<typename ClmpT::base_t, ClmpT::f, ClmpT::realVMin, ClmpT::realVMax>( ClmpT::range(v.value) );
    }

    /// Version of clampLower() for lower limit known at compile-time: if v compares less than lo,
    /// lo is returned; otherwise v is returned.
    /// \returns clamped v, wrapped into a new Sq type with the template parameter as lower limit.
    template< double realLo >
    requires fpm::detail::ValidImplType< CTClamp<realLo, realVMax> >
    friend constexpr
    auto clampLower(Sq const &v) noexcept {
        using ClmpT = CTClamp<realLo, realVMax>;
        return Sq<typename ClmpT::base_t, ClmpT::f, ClmpT::realVMin, ClmpT::realVMax>( ClmpT::lower(v.value) );
    }

    /// Version of clampUpper() for upper limit known at compile-time: if hi compares less than v,
    /// hi is returned; otherwise v is returned.
    /// \returns clamped v, wrapped into a new Sq type with the template parameter as upper limit.
    template< double realHi >
    requires fpm::detail::ValidImplType< CTClamp<realVMin, realHi> >
    friend constexpr
    auto clampUpper(Sq const &v) noexcept {
        using ClmpT = CTClamp<realVMin, realHi>;
        return Sq<typename ClmpT::base_t, ClmpT::f, ClmpT::realVMin, ClmpT::realVMax>( ClmpT::upper(v.value) );
    }

    /// \returns the minimum value of the two given values, wrapped into a new Sq type with the minimum
    /// of the limits. If both values are equivalent, the first value is returned.
    template< /* deduced: */ SqType Sq2 >
    requires ( fpm::detail::Similar<Sq, Sq2> && fpm::detail::ValidImplType< Min<Sq2> > )
    friend constexpr
    auto min(Sq const &v, Sq2 const &v2) noexcept {
        using MinT = Min<Sq2>;
        return Sq<typename MinT::base_t, MinT::f, MinT::realVMin, MinT::realVMax>( MinT::min(v.value, v2.value) );
    }

    /// \returns the maximum value of the two given values, wrapped into a new Sq type with the maximum
    /// of the limits. If both values are equivalent, the first value is returned.
    template< /* deduced: */ SqType Sq2 >
    requires ( fpm::detail::Similar<Sq, Sq2> && fpm::detail::ValidImplType< Max<Sq2> > )
    friend constexpr
    auto max(Sq const &v, Sq2 const &v2) noexcept {
        using MaxT = Max<Sq2>;
        return Sq<typename MaxT::base_t, MaxT::f, MaxT::realVMin, MaxT::realVMax>( MaxT::max(v.value, v2.value) );
    }

private:
    // delete undesired special members
    Sq() = delete;  // default constructor
    Sq& operator =(Sq const &) = delete;  // copy-assignment
    Sq& operator =(Sq&&) = delete;  // move-assignment

    /// Explicit, possibly compile-time constructor from scaled integer value.
    explicit constexpr
    Sq(base_t value) noexcept : value(value) {}

    // friend all Sq types so that private members of similar types can be accessed for construction
    template< std::integral BaseTSq, scaling_t fSq, double realVMinSq, double realVMaxSq >
    requires fpm::detail::SqRequirements<BaseTSq, fSq, realVMinSq, realVMaxSq>
    friend class Sq;

    // friend Q type so that it can access the private members of a Sq type to construct it
    template< std::integral BaseTQ, scaling_t fQ, double realVMinQ, double realVMaxQ, Overflow ovfQ >
    requires fpm::detail::QRequirements<BaseTQ, fQ, realVMinQ, realVMaxQ, ovfQ>
    friend class q::Q;

    //
    // memory
    //

    /// scaled integer value that represents a fixed-point value; stored in memory
    base_t const value;
};

/// Converts a literal number into the corresponding best-fit sq type.
/// Best-fit means that the literal number represents both limits and the value.
template< SqType Sq, char ...charArray >
consteval auto fromLiteral() {
    constexpr double value = fpm::detail::doubleFromLiteral<charArray...>();
    return Sq::template clamp_t<value, value>::template fromReal<value>();
}
/// Associates an Sq type with a literal.
#define FPM_SQ_BIND_LITERAL(_sq, _literal) \
    template< char ...chars > consteval auto operator "" ## _ ## _literal () { return fpm::sq::fromLiteral<_sq, chars...>(); }

/**\}*/
}

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
        return v2s<T, -SqT::f>( numeric_limits<typename SqT::base_t>::min() );
    }

    /// \returns the maximum real value that can be represented by the Sq type.
    /// \note In contrast to Sq::realVMax, this does not return the maximum value specified by the
    /// user, but the absolute maximum that can be represented by the underlying Sq type with respect
    /// to its base type and scaling. This can be significantly larger that the actual user maximum.
    template< typename T = double >
    static constexpr T max() noexcept {
        return v2s<T, -SqT::f>( numeric_limits<typename SqT::base_t>::max() );
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
