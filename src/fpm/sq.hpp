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
    double V_MAX,     ///< maximum real value represented by this type
    overflow OVF_ACTION = overflow::FORBIDDEN  ///< overflow action when overflow check is positive
>
class sq final
{
    static_assert(std::is_integral<BASE_T>::value, "base type must be integral");

public:
    static constexpr BASE_T T_MIN = std::numeric_limits<BASE_T>::min();  ///< minimum possible value of base type
    static constexpr BASE_T T_MAX = std::numeric_limits<BASE_T>::max();  ///< maximum possible value of base type
    static constexpr BASE_T MIN = v2s<BASE_T, F>(V_MIN);  ///< minimum value of integer value range
    static constexpr BASE_T MAX = v2s<BASE_T, F>(V_MAX);  ///< maximum value of integer value range

    /// Explicit compile-time-only constructor from floating-point value.
    /// \note Due to consteval it is guaranteed that this will not overflow (would not compile otherwise).
    explicit consteval sq(double value) noexcept : sq( v2s<BASE_T, F>(value) )
    {}

    /// Destructor.
    constexpr ~sq()
    {}

    /// Reveals the integer value stored in the memory.
    BASE_T Reveal() const {
        return value;
    }

    /// Unwraps to the real value. May be used for debugging purposes.
    /// \warning Unscaled, real value will be of specified target integer type (no floats at runtime).
    ///          There can be a significant loss of precision.
    template< typename TARGET_T >
    TARGET_T Unwrap() const noexcept {
        return s2s<TARGET_T, F, 0>(value);
    }

private:
    // delete default (runtime) constructor
    sq() = delete;

    /// Explicit, possibly compile-time-only constructor from integer value.
    explicit constexpr sq(BASE_T value) noexcept : value(value)
    {
        // // do not compile if constexpr value is out of range
        // if consteval {
        //     // static_assert on function argument is not working; use a trick to make this not compile
        //     value /= (value >= MIN && value <= MAX);  // constexpr value is out of value range if this fails
        // }
        // else {
        //     if constexpr ((value >= MIN && value <= MAX) && overflow::FORBIDDEN == OVF_ACTION) {
        //         throw();  // make this not compile when it should not
        //     }
        //     else if constexpr ((value >= MIN && value <= MAX) && overflow::ASSERT == OVF_ACTION) {
        //         assert(false);
        //     }
        //     else if constexpr ((value >= MIN && value <= MAX) && overflow::SATURATE == OVF_ACTION) {
        //         // TODO: saturate value properly
        //     }
        //     else { /* okay */ }
        // }
    }

    /// scaled integer value that represents a fixed-point value; stored in memory
    BASE_T value;
};

}


#endif
// EOF
