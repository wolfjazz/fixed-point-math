/** \file
 * Declaration of the sq class template.
 */

#ifndef _FPM_SQ_HPP_
#define _FPM_SQ_HPP_

#include "fpm.hpp"
#include <cmath>
#include <limits>
#include <type_traits>


namespace fpm {

/// Static (safe) q type. Implements mathematical operations and checks at compile-time whether these
/// operations can be performed for the specified value range without running into overflow issues.
/// \note sq values cannot be constructed directly at runtime; only indirect construction via q types
///       is possible in this case.
template<
    typename BASE_T_,  ///< type of the scaled integer stored in memory
    scaling_t F_,      ///< number of fraction bits (precision 2^-F)
    double REAL_V_MIN_ = v2s<double, -F_>((double)std::numeric_limits<BASE_T_>::min()),   ///< minimum real value represented by this type
    double REAL_V_MAX_ = v2s<double, -F_>((double)std::numeric_limits<BASE_T_>::max()) >  ///< maximum real value represented by this type
requires (
       ValidBaseType<BASE_T_>
    && ValidScaling<F_>
    && RealLimitsInRangeOfBaseType<BASE_T_, F_, REAL_V_MIN_, REAL_V_MAX_>
)
class sq final {
public:
    using base_t = BASE_T_;  /// integral base type
    static constexpr scaling_t F = F_;  ///< number of fraction bits
    static constexpr double REAL_V_MIN = REAL_V_MIN_;  ///< minimum real value
    static constexpr double REAL_V_MAX = REAL_V_MAX_;  ///< maximum real value
    static constexpr base_t V_MIN = v2s<base_t, F>(REAL_V_MIN_);  ///< minimum value of integer value range
    static constexpr base_t V_MAX = v2s<base_t, F>(REAL_V_MAX_);  ///< maximum value of integer value range
    static constexpr double RESOLUTION = v2s<double, -F>(1);  ///< real resolution of this type

    // friend related q type so that it can access the private members of this type to construct it
    template<typename _BASE_T_FR, scaling_t _F_FR, double, double, overflow>
    requires ( std::is_same_v<base_t, _BASE_T_FR> && _F_FR == F )
    friend class q;

    /// Create a new sq type with the same base type and scaling but a different real value range.
    template< double NEW_REAL_V_MIN, double NEW_REAL_V_MAX >
    using delimit = sq< base_t, F, NEW_REAL_V_MIN, NEW_REAL_V_MAX >;

    /// Named compile-time-only "constructor" from a floating-point value. This will use v2s to scale
    /// the given floating-point value at compile-time and then call the sq constructor with the
    /// scaled integer value at runtime.
    /// \note: When a real value is wrapped into an sq value, there is an inherent rounding error due
    /// to the limited resolution. This error is called 'representation error' and it refers to the
    /// deviation from the initial real value when an sq value is unscaled to a real value again.
    /// Usually the representation error is in the order of the resolution of the sq type.
    /// \note Does not compile if the value is outside the value range.
    template< double REAL_VALUE >
    requires ( REAL_V_MIN <= REAL_VALUE && REAL_VALUE <= REAL_V_MAX )  // must not overflow
    static consteval sq from_real() {
        constexpr base_t scaledValue = v2s<base_t, F>(REAL_VALUE);
        return sq(scaledValue);
    }

    /// Named "constructor" from a scaled integer value.
    /// \note Does not compile if the value is outside the value range.
    template< base_t VALUE >
    requires ( V_MIN <= VALUE && VALUE <= V_MAX )  // must not overflow
    static consteval sq from_scaled() {
        return sq(VALUE);
    }

    /// Named "Copy-Constructor" from another sq type value with the same base type and scaling.
    /// \note When an sq value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the sq value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< double _REAL_V_MIN_FROM, double _REAL_V_MAX_FROM, scaling_t _F_FROM >
    requires (
        // rescaling is only possible if the real value can be represented (must not overflow)
        REAL_V_MIN <= _REAL_V_MIN_FROM
        && REAL_V_MAX >= _REAL_V_MAX_FROM
    )
    static constexpr sq from_sq(sq<base_t, _F_FROM, _REAL_V_MIN_FROM, _REAL_V_MAX_FROM> const &from) noexcept {
        return sq( s2s<base_t, _F_FROM, F>(from.reveal()) );
    }

    /// Copy-Constructor from the same type.
    constexpr sq(sq const &) noexcept = default;

    /// Move-Constructor from the same type.
    constexpr sq(sq&&) noexcept = default;

    /// Destructor.
    constexpr ~sq()
    {}

    /// Reveals the integer value stored in the memory.
    base_t reveal() const noexcept {
        return value;
    }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning This conversion is expensive if the target type is a floating-point type.
    ///          If the target type is an integral type, there can be a significant loss of precision.
    ///          Use carefully!
#   if defined FPM_USE_SH
    template< typename TARGET_T = int >
#   else
    template< typename TARGET_T = double >
#   endif
    TARGET_T to_real() const noexcept {
        return v2s<TARGET_T, -F>(value);
    }

private:
    // delete undesired special members
    sq() = delete;  // default constructor
    sq& operator=(sq const &) = delete;  // copy-assignment
    sq& operator=(sq&&) = delete;  // move-assignment

    /// Explicit, possibly compile-time constructor from scaled integer value.
    explicit constexpr sq(base_t value) noexcept : value(value)
    {}

    /// scaled integer value that represents a fixed-point value; stored in memory
    base_t const value;
};

}


#endif
// EOF
