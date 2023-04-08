/** \file
 * Declaration of the sq class template.
 */

#ifndef _FPM_SQ_HPP_
#define _FPM_SQ_HPP_

#include "fpm.hpp"
#include <limits>
#include <cmath>


namespace fpm {

template<
    typename BASE_T,  ///< type of the scaled integer stored in memory
    scaling_t F,      ///< number of fraction bits (precision 2^-F)
    double REAL_V_MIN_,  ///< minimum real value represented by this type
    double REAL_V_MAX_   ///< maximum real value represented by this type
>
class sq final
{
    static_assert(std::is_integral_v<BASE_T>, "base type must be integral");

public:
    static constexpr double REAL_V_MIN = REAL_V_MIN_;  ///< minimum real value
    static constexpr double REAL_V_MAX = REAL_V_MAX_;  ///< maximum real value
    static constexpr BASE_T V_MIN = v2s<BASE_T, F>(REAL_V_MIN_);  ///< minimum value of integer value range
    static constexpr BASE_T V_MAX = v2s<BASE_T, F>(REAL_V_MAX_);  ///< maximum value of integer value range
    static constexpr double RESOLUTION = v2s<double, -F>(1);  ///< real resolution of this type

    /// Named compile-time-only "constructor" from a floating-point value. This will use v2s to scale
    /// the given floating-point value at compile-time and then call the sq constructor with the
    /// scaled integer value.
    /// \warning Does NOT perform any overflow check with regard to the user-defined value range!
    template< double REAL_VALUE >
    static consteval sq from_real() {
        // does not compile if scaled value does not fit BASE_T
        constexpr BASE_T scaledValue = v2s<BASE_T, F>(REAL_VALUE);

        return sq(scaledValue);
    }

    /// Named "constructor" from a scaled integer value.
    /// \note Just calls the sq constructor with the given constexpr value. Exists for consistency reasons.
    template< BASE_T VALUE >
    static consteval sq from_scaled() {
        return sq(VALUE);
    }

    /// Explicit, possibly compile-time constructor from scaled integer value.
    explicit constexpr sq(BASE_T value) noexcept : value(value)
    {}

    /// Copy-Constructor from the same type.
    constexpr sq(sq const &) noexcept = default;

    /// Move-Constructor from the same type.
    constexpr sq(sq&&) noexcept = default;

    /// Destructor.
    constexpr ~sq()
    {}

    /// Copy-Constructor from a different q type with the same base type.
    template< double REAL_V_MIN_FROM, double REAL_V_MAX_FROM, scaling_t F_FROM >
    requires (
        F_FROM != F
        && REAL_V_MIN <= REAL_V_MIN_FROM
        && REAL_V_MAX >= REAL_V_MAX_FROM
    )
    sq(sq<BASE_T, F_FROM, REAL_V_MIN_FROM, REAL_V_MAX_FROM> const &from) noexcept
        : value( s2s<BASE_T, F_FROM, F>(from.reveal()) )
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

    /// scaled integer value that represents a fixed-point value; stored in memory
    BASE_T const value;
};

}


#endif
// EOF
