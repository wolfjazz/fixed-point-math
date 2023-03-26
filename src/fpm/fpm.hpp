/** \file
 * Default includes and declarations.
 */

#ifndef _FPM_FPM_HPP_
#define _FPM_FPM_HPP_

#include <type_traits>
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
    /// \note This is the type that should be used in released software when overflow checks cannot
    /// be avoided in the first place.
    SATURATE = 2u,

    /// Do not perform any overflow checks (i.e. overflow is explicitly allowed).
    /// \warning The value can overflow in this case!
    ALLOWED = 3u,
};


/** Scaling factor type. */
using scaling_t = int8_t;


/** Scale-To-Scale scaling function.
 * Used to scale a given integer value to a different scaling factor and target type using multiplication/division.
 * \note Shift operations are well defined for C++20 and above (signed integers are Two's Complement),
 *       however, arithmetic right shift would always round down (e.g. -514 >> 4 is -33, but +514 >> 4 is +32).
 *       This kind of asymmetry might be unexpected, therefore arithmetic multiplication/division is used instead.
 *       Besides, floating-point target types are also possible this way.
 * \warning Floating-point target types are possible, however quite expensive at runtime!
 *          Use carefully! */
template< typename TARGET_T, scaling_t FROM, scaling_t TO, typename VALUE_T >
constexpr TARGET_T s2s(VALUE_T value) noexcept {
    static_assert(std::is_integral_v<VALUE_T>, "s2s only supports integer values");

    // use common type for calculation to avoid loss of precision
    using COMMON_T = typename std::common_type<VALUE_T, TARGET_T>::type;

    if constexpr (FROM > TO) {
        return static_cast<TARGET_T>( static_cast<COMMON_T>(value) / (1 << (unsigned)(FROM - TO)) );
    }
    else if constexpr (TO > FROM) {
        return static_cast<TARGET_T>( static_cast<COMMON_T>(value) * (1 << (unsigned)(TO - FROM)) );
    }
    else /* FROM == TO */ {
        return static_cast<TARGET_T>(value);
    }
}


/** Scale-To-Scale-Shift scaling function.
 * Used to scale a given integer value to a different scaling factor and target type via arithmetic
 * shift operations.
 * \note Shift operations are well defined for C++20 and above (signed integers are Two's Complement).
 * \note Floating-point target types are NOT possible, since shift operators are not defined for such types.
 *
 * \warning Be aware that arithmetic right shift always rounds down. Consequently, the scaled result
 *          is not symmetric for the same value with a different sign
 *          (e.g. -514 >> 4u is -33, but +514 >> 4u is +32). */
template< typename TARGET_T, scaling_t FROM, scaling_t TO, typename VALUE_T >
constexpr TARGET_T s2sh(VALUE_T value) noexcept {
    static_assert(std::is_integral_v<VALUE_T> && std::is_integral_v<TARGET_T>, "s2sh only supports integer types");

    // use common type for shift to avoid loss of precision
    using COMMON_T = typename std::common_type<VALUE_T, TARGET_T>::type;

    if constexpr (FROM > TO) {
        return static_cast<TARGET_T>( static_cast<COMMON_T>(value) >> (unsigned)(FROM - TO) );
    }
    else if constexpr (TO > FROM) {
        return static_cast<TARGET_T>( static_cast<COMMON_T>(value) << (unsigned)(TO - FROM) );
    }
    else /* FROM == TO */ {
        return static_cast<TARGET_T>(value);
    }
}


/** Value-To-Scale scaling function.
 * Used to scale a given compile-time floating-point value to a scaled runtime value of target type.
 * \note consteval ensures that this evaluates to a compile-time constant expression.
 * \warning Floating-point target types are possible, however quite expensive at runtime!
 *          Use carefully! */
template< typename TARGET_T, scaling_t TO >
consteval TARGET_T v2s(double fpValue) noexcept {
    if constexpr (TO < 0) {
        return static_cast<TARGET_T>(fpValue / (1 << (unsigned)(-TO)));
    }
    else if constexpr (TO > 0) {
        return static_cast<TARGET_T>(fpValue * (1 << (unsigned)(TO)));
    }
    else /* TO == 0 */ {
        return static_cast<TARGET_T>(fpValue);
    }
}

}


#endif
// EOF
