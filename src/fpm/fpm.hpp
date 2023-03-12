/** \file
 * Default includes and declarations.
 */

#ifndef _FPM_FPM_HPP_
#define _FPM_FPM_HPP_

#include <limits>
#include <cstdint>


/** Fixed-Point Math Namespace. */
namespace fpm {

/** Overflow actions. */
enum overflow : uint8_t {
    /// Default. Code must not compile if an overflow check is required.
    /// \note: It is best practice to keep this default action when possible, because this way the
    /// compiler will complain if an overflow check is required at some point, and the developer can
    /// then decide which overflow action fits best, or change the implementation if an overflow check
    /// is not desired at all. -> It will be noticed when the compiler adds overflow checks!
    FORBIDDEN = 0u,

    /// If an overflow check is needed, it will be included. In case of an overflow the assert function
    /// will be called.
    /// \note This can be used e.g. for debug builds, on types where FORBIDDEN is not possible by design.
    ASSERT = 1u,

    /// In case of an overflow, the value will be saturated to the closest limitation value.
    /// \note This is the type that should be used in released software when an overflow checks cannot
    /// be avoided in the first place.
    SATURATE = 2u,
};


/** Scaling factor type. */
using scaling_t = int8_t;


/** Scale-To-Scale scaling function.
 * Used to scale a given integer value to a different scaling factor.
 * \note Shift operations are well defined for C++20 and above (signed integers are Two's Complement). */
template< typename TARGET_T, scaling_t FROM, scaling_t TO, typename VALUE_T >
constexpr TARGET_T s2s(VALUE_T value) noexcept {
    static_assert(std::is_integral<VALUE_T>::value && std::is_integral<TARGET_T>::value, "s2s only supports integers");

    if constexpr (FROM > TO) {
        // first downscale, then cast
        return static_cast<TARGET_T>(value >> (unsigned)(FROM - TO));
    }
    else if constexpr (TO > FROM) {
        // first cast, then upscale
        return static_cast<TARGET_T>(value) << (unsigned)(TO - FROM);
    }
    else /* FROM == TO */ {
        return static_cast<TARGET_T>(value);
    }
}


/** Value-To-Scale scaling function.
 * Used to scale a given compile-time floating-point value to a scaled runtime integer value.
 * \note consteval ensures that this evaluates to a compile-time constant expression. */
template< typename INT_T, scaling_t TO >
consteval INT_T v2s(double fpValue) noexcept {
    static_assert(std::is_integral<INT_T>::value, "v2s must return an integer");

    if constexpr (TO < 0) {
        return static_cast<INT_T>(fpValue / (1u << (unsigned)(-TO)));
    }
    else if constexpr (TO > 0) {
        return static_cast<INT_T>(fpValue * (1u << (unsigned)(TO)));
    }
    else /* TO == 0 */ {
        return static_cast<INT_T>(fpValue);
    }
}

}


#endif
// EOF
