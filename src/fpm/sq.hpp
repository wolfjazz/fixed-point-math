/** \file
 * Declaration of the sq class template.
 */

#ifndef _FPM_SQ_HPP_
#define _FPM_SQ_HPP_

#include "fpm.hpp"
#include <limits>


namespace fpm {

template<
    typename BASE_T,  ///< type of the scaled integer stored in memory
    scaling_t F,      ///< number of fraction bits (precision 2^-F)
    double V_MIN,     ///< minimum real value represented by this type
    double V_MAX      ///< maximum real value represented by this type
>
class sq final
{
    static_assert(std::is_integral_v<BASE_T>, "base type must be integral");

public:
    static constexpr BASE_T MIN = v2s<BASE_T, F>(V_MIN);  ///< minimum value of integer value range
    static constexpr BASE_T MAX = v2s<BASE_T, F>(V_MAX);  ///< maximum value of integer value range
    static constexpr double RESOLUTION = v2s<double, -F>(1.);  ///< real resolution of this type

    /// Explicit compile-time-only named "constructor" from a floating-point value.
    /// \note Performs compile-time overflow checks and does not compile if value is out of (user-) range.
    template< double VALUE >
    static consteval sq from_real() {
        constexpr BASE_T scaledValue = v2s<BASE_T, F>(VALUE);  // does not compile if scaled value does not fit BASE_T

        // do not compile if initial value does not fit user-defined value range
        static_assert(scaledValue >= MIN && scaledValue <= MAX, "value is out of user range");

        return sq(scaledValue);
    }

    /// Explicit, possibly compile-time constructor from scaled integer value.
    explicit constexpr sq(BASE_T value) noexcept : value(value)
    {}

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
    TARGET_T unwrap() const noexcept {
        return s2s<TARGET_T, F, 0>(value);
    }

private:
    // delete default (runtime) constructor
    sq() = delete;

    /// scaled integer value that represents a fixed-point value; stored in memory
    BASE_T const value;
};

}


#endif
// EOF
