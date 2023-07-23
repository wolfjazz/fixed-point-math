/** \file
 * Default includes, declarations and definitions of the fpm library.
 */

#ifndef FPM_FPM_HPP_
#define FPM_FPM_HPP_

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>


/** \defgroup grp_fpm Fixed Point Math
 * Fixed point library with overflow protection.
 * \{ */

/** Fixed-Point Math Namespace. */
namespace fpm {

/** Overflow behavior.
 * \note Prioritized order: highest priority at top (forbidden), lowest at bottom (no_check). */
enum class Overflow : uint8_t {
    /// Default. Code must not compile if an overflow check is required.
    /// \note: It is best practice to keep this default behavior when possible, because this way the
    /// compiler will complain if an overflow check is required at some point, and the developer can
    /// then decide which overflow behavior fits best, or change the implementation if an overflow
    /// check is not desired at all.
    forbidden = 0u,

    /// If an overflow check is needed, it will be included. In case of an overflow the assert
    /// function will be called at runtime.
    /// \note This can be used e.g. in a debug build on types where 'forbidden' is not possible by design.
    assert = 1u,

    /// In case of an overflow, the value will be saturated to the closest limit.
    /// \note This is the type that should be used in released software when overflow checks cannot
    /// be avoided in the first place.
    saturate = 2u,

    /// Do not perform any overflow checks (i.e. overflow is explicitly allowed).
    /// \warning The value can overflow in this case!
    noCheck = 3u,
    allowed = noCheck,
};

/// Overflow type alias.
using Ovf = Overflow;

/** Checks if overflow behavior a is stricter than overflow behavior b. */
template< Overflow a, Overflow b >
struct is_ovf_stricter { static constexpr bool value = a < b; };
template< Overflow a, Overflow b >
static constexpr bool is_ovf_stricter_v = is_ovf_stricter<a, b>::value;


/// Scaling factor type.
using scaling_t = int8_t;

/// Intermediate type used for compile-time and runtime calculations with (s)q values.
template< std::integral BaseT >
using interm_t = typename std::conditional_t<std::is_signed_v<BaseT>, int64_t, uint64_t>;

/// Maximal supported size of a (s)q-type's base type.
/// TODO: Support uint64_t.
constexpr size_t MAX_BASETYPE_SIZE = sizeof(uint32_t);

/// Maximum possible value of f to support correct scaling of floating-point types with double precision.
/// Corresponds to the effective size of double's mantissa (significant double precision).
/// \note Absolute integers between 2^-53 and 2^53 can be exactly represented in double.
constexpr scaling_t MAX_F = 53;


/// Internal implementations.
namespace details {

    // Some standard functions are redefined here for use in concepts, otherwise VSCode would squiggle
    // the functions although the functions from std compile. :(
    inline namespace {
        /** Returns the absolute value of the given input value. */
        template< typename ValueT >
        consteval ValueT abs(ValueT const input) noexcept {
            return input >= 0 ? input : -input;
        }
    }

    /** Overflow check function.
     * \note Works for signed and unsigned value type. */
    template<
        Overflow ovfBx,   ///< overflow behavior
        std::integral ValueT,  ///< type of the value to check (after a scaling/casting operation)
        std::integral SrcValueT = ValueT  ///< type of the value before scaling/casting operation; required if different
    >
    constexpr void checkOverflow(ValueT &value, ValueT const min, ValueT const max) noexcept {
        // Overflow check types.
        enum checktype : uint8_t {
            CHECKTYPE_SIGN_UNCHANGED = 0u,  ///< default overflow check
            CHECKTYPE_SIGNED_TO_UNSIGNED = 1u,  ///< sign has changed from signed to unsigned
            CHECKTYPE_UNSIGNED_TO_SIGNED = 2u,  ///< sign has changed from unsigned to signed
        };

        if constexpr (Overflow::assert == ovfBx) {
            if (value < min || value > max) {
                assert(false);  // value is out of range
            }
        }
        else if constexpr (Overflow::saturate == ovfBx) {
            // determine check type
            constexpr auto checkType = (std::is_signed_v<SrcValueT> && std::is_unsigned_v<ValueT>)
                ? CHECKTYPE_SIGNED_TO_UNSIGNED
                : (std::is_unsigned_v<SrcValueT> && std::is_signed_v<ValueT>)
                    ? CHECKTYPE_UNSIGNED_TO_SIGNED
                    : CHECKTYPE_SIGN_UNCHANGED;

            // if the value was cast from a signed to an unsigned type and is in the upper half of the
            // unsigned value range, the value was negative before; saturate it to the lower limit
            if constexpr (CHECKTYPE_SIGNED_TO_UNSIGNED == checkType) {
                constexpr ValueT signedMax = static_cast<ValueT>(std::numeric_limits<std::make_signed_t<ValueT>>::max());
                if (value < min || value > signedMax) {
                    value = min;
                }
                else if (value > max) {
                    value = max;
                }
                else { /* okay */ }
            }
            // if the value was cast from an unsigned to a signed type and is negative, the value was
            // positive before; saturate it to the upper limit
            else if constexpr (CHECKTYPE_UNSIGNED_TO_SIGNED == checkType) {
                if (value < 0 || value > max) {
                    value = max;
                }
                else if (value < min) {
                    value = min;
                }
                else { /* okay */ }
            }
            else /* checktype sign unchanged */ {
                if (value < min) {
                    value = min;
                }
                else if (value > max) {
                    value = max;
                }
                else { /* okay */ }
            }
        }
        else { /* Overflow::allowed, Overflow::noCheck: no checks performed */ }
    }

    /// Converts a given character array into a double.
    template< std::size_t size >
    requires ( size > 0u && size <= std::numeric_limits<double>::max_digits10 )
    consteval double charArrayToDouble(char const chars[size]) {
        double number = 0.;
        double fScale = 1.;
        for (size_t i = 0u; i < size; ++i) {
            if (chars[i] == '.') { fScale = 0.1; continue; }
            int d = chars[i] - '0';
            if (fScale > 0.5) { number = number*10 + (double)d; }
            else { number += (double)d*fScale; fScale /= 10; }
        }
        return number;
    }

    /// Helper that fits the smallest signed integral type that fits the given value.
    template< std::integral T, T value >
    struct fit_signed {
        using type = std::conditional_t<std::in_range<int8_t>(value), int8_t,
            std::conditional_t<std::in_range<uint8_t>(value) || std::in_range<int16_t>(value), int16_t,
            std::conditional_t<std::in_range<uint16_t>(value) || std::in_range<int32_t>(value), int32_t,
            std::conditional_t<std::in_range<uint32_t>(value) || std::in_range<int64_t>(value), int64_t, intmax_t>>>>;
    };
    /// Determines the smallest signed integral type that fits the given value.
    template< std::integral T, T value >
    using fit_signed_t = typename fit_signed<T, value>::type;

    /// Functions defined for testing purposes.
    namespace test {
        /** Returns the minimum distance between doubles (epsilon) for numbers of the magnitude
         * of the given value.
         * \warning Expensive when used in production code! */
        inline double floatpEpsilonFor(double value) noexcept {
            double epsilon = nextafter(value, std::numeric_limits<double>::infinity()) - value;
            return epsilon;
        }
    }

    /** Concept of a valid difference between two integral types and two scaling factors to support scaling
     * without overflow or significant loss of precision.
     * Scaling is possible if:
     *  - target and source type are of equal size and the effective target value range is not outshifted, or
     *  - target and source type are of different size and the effective size difference is not outshifted, or
     *  - both types are equal and the number of shifted bits is at most the number of bits in each type
     * \note If this fails, none of the conditions listed above is fulfilled. Double-check! */
    template< typename from_t, scaling_t fFrom, typename to_t, scaling_t fTo >
    concept ScalingIsPossible = (
        ( sizeof(from_t) == sizeof(to_t) && details::abs(fTo - fFrom) <= std::numeric_limits<to_t>::digits )
        || ( sizeof(from_t) != sizeof(to_t)
            && details::abs(fTo - fFrom) <= details::abs(std::numeric_limits<to_t>::digits - std::numeric_limits<from_t>::digits) )
        || ( std::is_same_v<from_t, to_t> && details::abs(fTo - fFrom) <= sizeof(to_t) * CHAR_BIT )
    );

}  // end of details


/** Scale-To-Scale scaling function.
 * Used to scale a given, already scaled (integer) value to a different scaling factor and target type
 * using multiplication/division.
 * \note Arithmetic multiplication/division is used here because these operations are symmetric for
 *       positive and negative values with respect to rounding (e.g. +-514 / 2^4 is +-32).
 *       Besides, floating-point types are also possible this way.
 * \warning Floating-point types are possible, however quite expensive at runtime!
 *          Use carefully! */
template< typename TargetT, scaling_t from, scaling_t to, /* deduced: */ typename ValueT >
constexpr TargetT s2smd(ValueT value) noexcept {
    // use common type for calculation to avoid loss of precision
    using CommonT = typename std::common_type<ValueT, TargetT>::type;
    using ScalingT = typename std::conditional_t<sizeof(CommonT) <= 4u,
        std::conditional_t<std::is_signed_v<CommonT>, int32_t, uint32_t>,
        std::conditional_t<std::is_signed_v<CommonT>, int64_t, uint64_t>>;

    if constexpr (from > to) {
        return static_cast<TargetT>( static_cast<CommonT>(value) / (static_cast<ScalingT>(1) << (unsigned)(from - to)) );
    }
    else if constexpr (to > from) {
        return static_cast<TargetT>( static_cast<CommonT>(value) * (static_cast<ScalingT>(1) << (unsigned)(to - from)) );
    }
    else /* from == to */ {
        return static_cast<TargetT>(value);
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
template< std::integral TargetT, scaling_t from, scaling_t to, /* deduced: */ std::integral ValueT >
constexpr TargetT s2sh(ValueT value) noexcept {
    // use common type for shift to avoid loss of precision
    using CommonT = typename std::common_type<ValueT, TargetT>::type;

    if constexpr (from > to) {
        return static_cast<TargetT>( static_cast<CommonT>(value) >> (unsigned)(from - to) );
    }
    else if constexpr (to > from) {
        return static_cast<TargetT>( static_cast<CommonT>(value) << (unsigned)(to - from) );
    }
    else /* from == to */ {
        return static_cast<TargetT>(value);
    }
}

/// Scale-To-Scale function used in the implementations of the (s)q types.
/// Proxy for the s2sx function pre-selected by the user.
/// \note FPM_USE_SH can be predefined before this header is included into a source file.
/// \note constexpr implies inline.
#if !defined FPM_USE_SH
template< typename TargetT, scaling_t from, scaling_t to, /* deduced: */ typename ValueT >
constexpr TargetT s2s(ValueT value) noexcept { return s2smd<TargetT, from, to>(value); }
#else
template< std::integral TargetT, scaling_t from, scaling_t to, /* deduced: */ std::integral ValueT >
constexpr TargetT s2s(ValueT value) noexcept { return s2sh<TargetT, from, to>(value); }
#endif


/** Value-To-Scale scaling function. Uses multiplication/division for the conversion.
 * Used to scale a given compile-time floating-point value to a scaled runtime value of target type.
 * \warning Floating-point target types are possible, however quite expensive at runtime!
 *          Use carefully! */
template< typename TargetT, scaling_t to, /* deduced: */ typename ValueT >
constexpr TargetT v2smd(ValueT value) noexcept {
    // use common type for shift to avoid loss of precision
    using CommonT = typename std::common_type<ValueT, TargetT>::type;
    using ScalingT = typename std::conditional_t<sizeof(CommonT) <= 4u,
        std::conditional_t<std::is_signed_v<CommonT>, int32_t, uint32_t>,
        std::conditional_t<std::is_signed_v<CommonT>, int64_t, uint64_t>>;

    if constexpr (to < 0) {
        return static_cast<TargetT>( static_cast<CommonT>(value) / (static_cast<ScalingT>(1) << (unsigned)(-to)) );
    }
    else if constexpr (to > 0) {
        return static_cast<TargetT>( static_cast<CommonT>(value) * (static_cast<ScalingT>(1) << (unsigned)to) );
    }
    else /* to == 0 */ {
        return static_cast<TargetT>(value);
    }
}

/** Value-To-Scale scaling function. Uses arithmetic shifts for the conversion.
 * \note Shift operations are well defined for C++20 and above (signed integers are Two's Complement).
 * \note Floating-point target types are NOT possible, since shift operators are not defined for them.
 *
 * \warning Be aware that arithmetic right shift always rounds down. Consequently, the scaled result
 *          is not symmetric for the same value with a different sign
 *          (e.g. -514 >> 4u is -33 but +514 >> 4u is +32). */
template< std::integral TargetT, scaling_t to, /* deduced: */ std::integral ValueT >
constexpr TargetT v2sh(ValueT value) noexcept {
    // use common type for calculation to avoid loss of precision
    using CommonT = typename std::common_type<ValueT, TargetT>::type;

    if constexpr (to < 0) {
        return static_cast<TargetT>( static_cast<CommonT>(value) >> (unsigned)(-to) );
    }
    else if constexpr (to > 0) {
        return static_cast<TargetT>( static_cast<CommonT>(value) << (unsigned)to );
    }
    else /* to == 0 */ {
        return static_cast<TargetT>(value);
    }
}

/// Value-To-Scale function used in the implementations of the (s)q types.
/// Proxy for the v2sx function pre-selected by the user.
/// \note FPM_USE_SH can be predefined before this header is included into a source file.
/// \note constexpr implies inline.
#if !defined FPM_USE_SH
template< typename TargetT, scaling_t to, /* deduced: */ typename ValueT >
constexpr TargetT v2s(ValueT value) noexcept { return v2smd<TargetT, to>(value); }
#else
template< std::integral TargetT, scaling_t to, /* deduced: */ std::integral ValueT >
constexpr TargetT v2s(ValueT value) noexcept { return v2sh<TargetT, to>(value); }
#endif


// Continue with internal implementations.
namespace details {

    /// \returns the real minimum value for the given integral type and scaling that can safely be
    /// used in operations like negation or taking the absolute value
    /// (i.e. 0u for unsigned, INT_MIN + 1 for signed).
    /// \note Internal. Use Q<>::realVMin in applications.
    template< std::integral T, scaling_t f >
    consteval double lowestRealVMin() {
        return v2s<double, -f>( std::is_unsigned_v<T> ? static_cast<T>(0) : std::numeric_limits<T>::min() + 1 );
    }

    /// \returns the real maximum value for the given integral type and scaling.
    /// \note Internal. Use Q<>::realVMax in applications.
    template< std::integral T, scaling_t f >
    consteval double highestRealVMax() {
        return v2s<double, -f>( std::numeric_limits<T>::max() );
    }

    /** Concept of a valid (s)q base-type.
     * \note If this fails, the selected base type is not integral, or too large.
             Use an integer with a proper size! */
    template< typename BaseT >
    concept ValidBaseType = (
        std::is_integral_v<BaseT>
        && sizeof(BaseT) <= MAX_BASETYPE_SIZE
    );

    /** Concept of a valid (s)q scaling value.
     * Types can be scaled by maximal the number of bits in the base type minus one bit, limited by the
     * size of double's mantissa.
     * \note If this fails, the selected scaling factor is too large. Use a smaller scaling factor! */
    template< typename BaseT, scaling_t f >
    concept ValidScaling = (
        f <= std::numeric_limits<std::make_signed_t<BaseT>>::digits  // CHAR_BIT * sizeof(BaseT) - 1
        && f <= MAX_F
    );

    /** Concept of a valid (s)q type value range that fits the specified base type.
     * \note If this fails, the specified real value limits exceed the value range of the selected
     *       base type when scaled with the desired scaling factor.
     *       Use a larger base type, a smaller scaling factor, or double-check the sign and value of
     *       your selected real limits! */
    template< typename BaseT, scaling_t f, double realVMin, double realVMax >
    concept RealLimitsInRangeOfBaseType = (
        std::in_range<BaseT>(v2s<interm_t<BaseT>, f>(realVMin))
        && std::in_range<BaseT>(v2s<interm_t<BaseT>, f>(realVMax))
        && realVMin <= realVMax
        && (std::is_signed_v<BaseT> || realVMin >= 0.)
    );

    /** Concept of a type that can overflow when allowed. Typically used in the context of casting.
     * \note In C++23, signed int overflow (i.e. the value does not fit in the type) is still undefined.
     * \note If this fails, a signed base type is used in the context of a potential overflow. This is
     *       not allowed. Use an unsigned target base type. */
    template< typename BaseT, Overflow ovfBx >
    concept CanBaseTypeOverflow = ( std::is_unsigned_v<BaseT> && ovfBx == Overflow::allowed );

    /** Concept of a valid value that fits the specified base type.
     * \note If this fails, the scaled integer value exceeds the value range of the specified base type.
     *       Use a larger base type or a smaller real value! */
    template< typename BaseT, scaling_t f, double realValue >
    concept RealValueScaledFitsBaseType = ( std::in_range<BaseT>(v2s<interm_t<BaseT>, f>(realValue)) );

    /** Concept: Compile-time-only check is possible.
     * \note If this fails, a runtime overflow check is probably needed but not allowed in the context
     *       where this concept is required. Use a different overflow check! */
    template< Overflow ovfBx >
    concept CompileTimeOnlyOverflowCheckPossible = ( Overflow::assert != ovfBx );

    /** Concept: Runtime overflow check required when needed.
     * \note If this fails, a runtime overflow check is needed but not allowed for the desired q type.
     *       Allow for type, or specify the overflow-override template argument (to be preferred)! */
    template< Overflow ovfBx, bool checkNeeded = true >
    concept RuntimeOverflowCheckAllowedWhenNeeded = ( !checkNeeded || Overflow::forbidden != ovfBx );

    /** Concept: Checks whether the absolute value can be taken for a value of the given base type.
     * \note If this fails, the given base type is either not integral, or it is a signed integer and
     *       INT_MIN is part of the value range. Exclude INT_MIN from the value range! */
    template< typename BaseT, BaseT vMin >
    concept CanAbsolutize = (
        std::is_integral_v<BaseT>
        && (std::is_unsigned_v<BaseT> || (std::is_signed_v<BaseT> && vMin != std::numeric_limits<BaseT>::min()))
    );

}  // end of details


}  // end of fpm
/**\}*/

#endif
// EOF
