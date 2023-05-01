/** \file
 * Default includes and declarations.
 */

#ifndef _FPM_FPM_HPP_
#define _FPM_FPM_HPP_

#include <cassert>
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <utility>


/** Fixed-Point Math Namespace. */
namespace fpm {

/** Overflow actions.
 * \note Prioritized order: highest priority at top, lowest at bottom. */
enum class overflow : uint8_t {
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
    NO_CHECK = 3u,
    ALLOWED = NO_CHECK,
};

/** \returns whether overflow action a is stricter than overflow action b. */
consteval bool is_ovf_stricter(overflow a, overflow b) noexcept {
    return a < b;
}


/// Scaling factor type.
using scaling_t = int8_t;

/// Intermediate type used for compile-time and runtime calculations with (s)q values.
template< typename BASE_T >
using interm_t = std::conditional_t<std::is_signed_v<BASE_T>, int64_t, uint64_t>;

/// Maximal size of q type base type supported.
constexpr size_t MAX_BASETYPE_SIZE = sizeof(uint32_t);

/// Maximum possible value of F to support correct scaling of floating-point types with double precision.
constexpr scaling_t MAX_F = 30;


/// Internal implementations.
namespace _i {

    /** Returns the absolute value of the given input value.
     * \note Shadows cmath::abs() as it is not declared constexpr, therefore VSCode would squiggle it here :( */
    template< typename VALUE_T, typename _TARGET_T = typename std::make_unsigned<VALUE_T>::type >
    requires ( std::is_signed_v<VALUE_T> && std::is_integral_v<VALUE_T> )
    consteval _TARGET_T abs(VALUE_T const input) noexcept {
        return static_cast<_TARGET_T>( input >= 0 ? input : -input );
    }

    /** Overflow check function.
     * \note Works for signed and unsigned value type. */
    template< overflow OVF_ACTION, typename VALUE_T >
    constexpr void check_overflow(VALUE_T &value, VALUE_T const MIN, VALUE_T const MAX) noexcept {
        if constexpr (overflow::ASSERT == OVF_ACTION) {
            if ( !(value >= MIN && value <= MAX)) {
                assert(false);  // value is out of range
            }
        }
        else if constexpr (overflow::SATURATE == OVF_ACTION) {
            if ( !(value >= MIN)) {
                value = MIN;
            }
            else if ( !(value <= MAX)) {
                value = MAX;
            }
            else { /* okay */ }
        }
        else { /* overflow::ALLOWED, overflow::NO_CHECK: no checks performed */ }
    }

    /// Functions defined for testing purposes.
    namespace testing {
        /** Returns the minimum distance between doubles (epsilon) for numbers of the magnitude
         * of the given value.
         * \warning Expensive at runtime! */
        constexpr double fp_epsilon_for(double value) noexcept {
            double epsilon = nextafter(value, std::numeric_limits<double>::infinity()) - value;
            return epsilon;
        }
    }
}


/** Concept of a valid difference between two types and two scaling factors to support scaling
 * without overflow or significant loss of precision. */
template< typename from_t, scaling_t F_FROM, typename to_t, scaling_t F_TO >
concept ScalingIsPossible = (
    ( ( sizeof(from_t) == sizeof(to_t) && _i::abs(F_TO - F_FROM) <= std::numeric_limits<to_t>::digits )
      || _i::abs(F_TO - F_FROM) <= (std::numeric_limits<from_t>::digits - std::numeric_limits<to_t>::digits) )
);


/** Scale-To-Scale scaling function.
 * Used to scale a given, already scaled (integer) value to a different scaling factor and target type
 * using multiplication/division.
 * \note Arithmetic multiplication/division is used here because these operations are symmetric for
 *       positive and negative values with respect to rounding (e.g. +-514 / 2^4 is +-32).
 *       Besides, floating-point types are also possible this way.
 * \warning Floating-point types are possible, however quite expensive at runtime!
 *          Use carefully! */
template< typename TARGET_T, scaling_t FROM, scaling_t TO, typename VALUE_T >
constexpr TARGET_T s2smd(VALUE_T value) noexcept {
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
 * \note Floating-point target types are NOT possible, since shift operators are not defined for them.
 *
 * \warning Be aware that arithmetic right shift always rounds down. Consequently, the scaled result
 *          is not symmetric for the same value with a different sign
 *          (e.g. -514 >> 4u is -33 but +514 >> 4u is +32). */
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

/// Scale-To-Scale function used in the implementations of the (s)q types.
/// Proxy for the s2sx function pre-selected by the user.
/// \note FPM_USE_SH can be predefined before this header is included into a source file.
/// \note constexpr implies inline.
#if !defined FPM_USE_SH
template< typename TARGET_T, scaling_t FROM, scaling_t TO, typename VALUE_T >
constexpr TARGET_T s2s(VALUE_T value) noexcept { return s2smd<TARGET_T, FROM, TO>(value); }
#else
template< typename TARGET_T, scaling_t FROM, scaling_t TO, typename VALUE_T >
constexpr TARGET_T s2s(VALUE_T value) noexcept { return s2sh<TARGET_T, FROM, TO>(value); }
#endif


/** Value-To-Scale scaling function. Uses multiplication/division for the conversion.
 * Used to scale a given compile-time floating-point value to a scaled runtime value of target type.
 * \warning Floating-point target types are possible, however quite expensive at runtime!
 *          Use carefully! */
template< typename TARGET_T, scaling_t TO, typename VALUE_T >
constexpr TARGET_T v2smd(VALUE_T value) noexcept {
    // use common type for shift to avoid loss of precision
    using COMMON_T = typename std::common_type<VALUE_T, TARGET_T>::type;

    if constexpr (TO < 0) {
        return static_cast<TARGET_T>( static_cast<COMMON_T>(value) / (1 << (unsigned)(-TO)) );
    }
    else if constexpr (TO > 0) {
        return static_cast<TARGET_T>( static_cast<COMMON_T>(value) * (1 << (unsigned)TO) );
    }
    else /* TO == 0 */ {
        return static_cast<TARGET_T>(value);
    }
}

/** Value-To-Scale scaling function. Uses arithmetic shifts for the conversion.
 * \note Shift operations are well defined for C++20 and above (signed integers are Two's Complement).
 * \note Floating-point target types are NOT possible, since shift operators are not defined for them.
 *
 * \warning Be aware that arithmetic right shift always rounds down. Consequently, the scaled result
 *          is not symmetric for the same value with a different sign
 *          (e.g. -514 >> 4u is -33 but +514 >> 4u is +32). */
template< typename TARGET_T, scaling_t TO, typename VALUE_T >
constexpr TARGET_T v2sh(VALUE_T value) noexcept {
    static_assert(std::is_integral_v<VALUE_T> && std::is_integral_v<TARGET_T>, "v2sh only supports integer types");

    // use common type for calculation to avoid loss of precision
    using COMMON_T = typename std::common_type<VALUE_T, TARGET_T>::type;

    if constexpr (TO < 0) {
        return static_cast<TARGET_T>( static_cast<COMMON_T>(value) >> (unsigned)(-TO) );
    }
    else if constexpr (TO > 0) {
        return static_cast<TARGET_T>( static_cast<COMMON_T>(value) << (unsigned)TO );
    }
    else /* TO == 0 */ {
        return static_cast<TARGET_T>(value);
    }
}

/// Value-To-Scale function used in the implementations of the (s)q types.
/// Proxy for the v2sx function pre-selected by the user.
/// \note FPM_USE_SH can be predefined before this header is included into a source file.
/// \note constexpr implies inline.
#if !defined FPM_USE_SH
template< typename TARGET_T, scaling_t TO, typename VALUE_T >
constexpr TARGET_T v2s(VALUE_T value) noexcept { return v2smd<TARGET_T, TO>(value); }
#else
template< typename TARGET_T, scaling_t TO, typename VALUE_T >
constexpr TARGET_T v2s(VALUE_T value) noexcept { return v2sh<TARGET_T, TO>(value); }
#endif


/** Concept of a valid (s)q base-type. */
template< typename BASE_T >
concept ValidBaseType = (
       std::is_integral_v<BASE_T>
    && sizeof(BASE_T) <= MAX_BASETYPE_SIZE
);

/** Concept of a valid (s)q scaling value. */
template< scaling_t SCALING >
concept ValidScaling = (
    SCALING <= MAX_F
);

/** Concept of a valid (s)q type value range that fits the specified base type. */
template< typename BASE_T, scaling_t F, double REAL_V_MIN, double REAL_V_MAX >
concept RealLimitsInRangeOfBaseType = (
       std::in_range<BASE_T>(v2s<interm_t<BASE_T>, F>(REAL_V_MIN))
    && std::in_range<BASE_T>(v2s<interm_t<BASE_T>, F>(REAL_V_MAX))
    && REAL_V_MIN <= REAL_V_MAX
    && (std::is_signed_v<BASE_T> || REAL_V_MIN >= 0.)
);

/** Concept of a valid value that fits the specified base type. */
template< typename BASE_T, scaling_t F, double REAL_VALUE >
concept RealValueScaledFitsBaseType = ( std::in_range<BASE_T>(v2s<interm_t<BASE_T>, F>(REAL_VALUE)) );

/** Concept: Runtime overflow check required when needed.
 * \note If this fails, runtime overflow check is needed but not allowed for desired q type.
 *       Allow for type, or specify the action-override template argument (to be preferred). */
template< overflow OVF_ACTION, bool CHECK_NEEDED = true >
concept RuntimeCheckAllowedWhenNeeded = ( !CHECK_NEEDED || overflow::FORBIDDEN != OVF_ACTION );

}

#endif
// EOF
