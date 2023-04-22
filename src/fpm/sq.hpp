/** \file
 * Declaration of the sq class template.
 */

#ifndef _FPM_SQ_HPP_
#define _FPM_SQ_HPP_

#include "fpm.hpp"
#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>


namespace fpm {

/// Static (safe) q type. Implements mathematical operations and checks at compile-time whether these
/// operations can be performed for the specified value range without running into overflow issues.
/// \note sq values cannot be constructed directly; only indirect construction via q types is possible.
template<
    typename BASE_T,  ///< type of the scaled integer stored in memory
    scaling_t F,      ///< number of fraction bits (precision 2^-F)
    double REAL_V_MIN_,  ///< minimum real value represented by this type
    double REAL_V_MAX_ >  ///< maximum real value represented by this type
class sq final {
    static_assert(std::is_integral_v<BASE_T>, "base type must be integral");
    static_assert(sizeof(BASE_T) <= 4u, "base type larger than 32 bits is not supported");

    using interm_t = std::conditional_t<std::is_signed_v<BASE_T>, int64_t, uint64_t>;
    static_assert(std::in_range<BASE_T>(v2s<interm_t, F>(REAL_V_MIN_)), "scaled minimum value exceeds value range of base type");
    static_assert(std::in_range<BASE_T>(v2s<interm_t, F>(REAL_V_MAX_)), "scaled maximum value exceeds value range of base type");
    static_assert(REAL_V_MIN_ <= REAL_V_MAX_, "minimum value of value range must be less than or equal to maximum value");
    static_assert(std::is_signed_v<BASE_T> || REAL_V_MIN_ >= 0., "minimum value of value range must be larger than or equal to 0");

public:
    using base_t = BASE_T;
    static constexpr double REAL_V_MIN = REAL_V_MIN_;  ///< minimum real value
    static constexpr double REAL_V_MAX = REAL_V_MAX_;  ///< maximum real value
    static constexpr BASE_T V_MIN = v2s<BASE_T, F>(REAL_V_MIN_);  ///< minimum value of integer value range
    static constexpr BASE_T V_MAX = v2s<BASE_T, F>(REAL_V_MAX_);  ///< maximum value of integer value range
    static constexpr double RESOLUTION = v2s<double, -F>(1);  ///< real resolution of this type

    // friend related q type so that it can access the private members of this type to construct it
    template<typename BASE_T_FR, scaling_t F_FR, double, double, overflow>
    requires( std::is_same_v<BASE_T, BASE_T_FR> && F_FR == F )
    friend class q;

    /// Copy-Constructor from the same type.
    constexpr sq(sq const &) noexcept = default;

    /// Move-Constructor from the same type.
    constexpr sq(sq&&) noexcept = default;

    /// Named "Copy-Constructor" from another sq type value with the same base type and scaling.
    /// \note When an sq value is up-scaled to a larger resolution, the initial representation error
    /// will not change because the underlying integer value is just multiplied by some integral power
    /// of two factor. However, if the sq value is down-scaled to a smaller resolution, the resulting
    /// representation error may become larger since the underlying integer is divided and the result
    /// rounded towards zero to the next integer. The resulting representation error is at most the
    /// sum of the two resolutions before and after a down-scaling operation.
    template< double REAL_V_MIN_FROM, double REAL_V_MAX_FROM, scaling_t F_FROM >
    requires (
        // rescaling is only possible if the real value can be represented
        REAL_V_MIN <= REAL_V_MIN_FROM
        && REAL_V_MAX >= REAL_V_MAX_FROM
    )
    static constexpr sq from_sq(sq<BASE_T, F_FROM, REAL_V_MIN_FROM, REAL_V_MAX_FROM> const &from) noexcept {
        return sq( s2s<BASE_T, F_FROM, F>(from.reveal()) );
    }

    /// Destructor.
    constexpr ~sq()
    {}

    /// Reveals the integer value stored in the memory.
    BASE_T reveal() const noexcept {
        return value;
    }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning This conversion is expensive if the target type is a floating-point type.
    ///          If the target type is an integral type, there can be a significant loss of precision.
    ///          Use carefully!
    template< typename TARGET_T = double >
    TARGET_T to_real() const noexcept {
        return s2s<TARGET_T, F, 0>(value);
    }

private:
    // delete undesired special members
    sq() = delete;  // default constructor
    sq& operator=(sq const &) = delete;  // copy-assignment
    sq& operator=(sq&&) = delete;  // move-assignment

    /// Explicit, possibly compile-time constructor from scaled integer value.
    explicit constexpr sq(BASE_T value) noexcept : value(value)
    {}

    /// scaled integer value that represents a fixed-point value; stored in memory
    BASE_T const value;
};

}


#endif
// EOF
