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
#include <numeric>
#include <type_traits>
#include <utility>


/** Fixed-Point Math Namespace. */
namespace fpm {
/** \defgroup grp_fpm Fixed Point Math
 * Fixed point library with overflow protection.
 * \{ */

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

    /// In case of an overflow, the value will be clamped to the closest limit at runtime.
    /// \note This is the type that should be used in released software when overflow checks cannot
    /// be avoided in the first place.
    clamp = 2u,

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
        std::integral SrcValueT = ValueT >  ///< type of the value before scaling/casting operation; required if different
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
        else if constexpr (Overflow::clamp == ovfBx) {
            // determine check type
            constexpr auto checkType = (std::is_signed_v<SrcValueT> && std::is_unsigned_v<ValueT>)
                ? CHECKTYPE_SIGNED_TO_UNSIGNED
                : (std::is_unsigned_v<SrcValueT> && std::is_signed_v<ValueT>)
                    ? CHECKTYPE_UNSIGNED_TO_SIGNED
                    : CHECKTYPE_SIGN_UNCHANGED;

            // if the value was cast from a signed to an unsigned type and is in the upper half of the
            // unsigned value range, the value was negative before; clamp it to the lower limit
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
            // positive before; clamp it to the upper limit
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

    /** \returns +1 if the given value is positive, -1 if it is negative and 0 if the value is 0. */
    template <typename T>
    constexpr int signum(T const x) {
        if constexpr (std::is_signed_v<T>) { return (T(0) < x) - (x < T(0)); }
        else { return T(0) < x; }
    }

    /** Calculates the given integer power of the given number.
     * Inspired by: https://prosepoetrycode.potterpcs.net/2015/07/a-simple-constexpr-power-function-c/ */
    consteval double dpowi(double num, int pow) {
        if (details::abs(pow) > std::numeric_limits<double>::max_exponent10) return 0.;
        if (pow == 0) return 1.;
        return (pow > 0) ? num * dpowi(num, pow-1) : dpowi(num, pow+1) / num;
    }

    /// Converts a given character array from a template literal operator into a double value.
    template< char ...charArray >
    consteval double doubleFromLiteral() {
        constexpr std::size_t length = sizeof...(charArray);
        constexpr char chars[length]{ charArray... };
        static_assert(length > 0u && length <= std::numeric_limits<double>::digits10
            && std::all_of(chars, chars + length, [](char c) { return isdigit(c) || c == '.' || c == 'e' || c == 'E' || c == '-'; }),
            "Argument to literal must be a positive integer or double");
        double number = 0., fScale = 1.;
        int exp = 0, eSign = 0;
        for (size_t i = 0u; i < length; ++i) {
            if (chars[i] == '.') { fScale = 0.1; continue; }
            else if (chars[i] == 'e' || chars[i] == 'E') {
                if (chars[i + 1] == '-') { eSign = -1; ++i; }
                else { eSign = 1; }
                continue;
            }
            int d = chars[i] - '0';
            if (eSign != 0) { exp = exp*10 + eSign*d; }
            else if (fScale > 0.5) { number = number*10 + (double)d; }
            else { number += (double)d*fScale; fScale /= 10; }
        }
        return number * dpowi(10., exp);
    }

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
}  // end of details


/** Scale-To-Scale scaling function.
 * Used to scale a given, already scaled (integer) value to a different scaling factor and target type
 * using multiplication/division.
 * \note Arithmetic multiplication/division is used here because these operations are symmetric for
 * positive and negative values with respect to rounding (e.g. +-514 / 2^4 is +-32).
 * Besides, floating-point types are also possible this way.
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

/** Scale-To-Scale function used in the implementations of the (s)q types.
 * Proxy for the s2sx function pre-selected by the user.
 * \note FPM_USE_SH can be predefined before this header is included into a source file.
 * \note constexpr implies inline. */
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

/** Value-To-Scale function used in the implementations of the (s)q types.
 * Proxy for the v2sx function pre-selected by the user.
 * \note FPM_USE_SH can be predefined before this header is included into a source file.
 * \note constexpr implies inline. */
#if !defined FPM_USE_SH
template< typename TargetT, scaling_t to, /* deduced: */ typename ValueT >
constexpr TargetT v2s(ValueT value) noexcept { return v2smd<TargetT, to>(value); }
#else
template< std::integral TargetT, scaling_t to, /* deduced: */ std::integral ValueT >
constexpr TargetT v2s(ValueT value) noexcept { return v2sh<TargetT, to>(value); }
#endif


// Continue with internal implementations.
namespace details {

    /** Determines the smallest type that can hold the the given real minimum and maximum values for
     * the given scaling f. The size of the resulting type will not be smaller than the smallest of
     * the input types. If one of the given base types is signed, the result will be signed too,
     * otherwise the result will be an unsigned type.
     * For example, if the input types are int8_t and uint8_t and the scaled value range is 50..550,
     * the resulting type will be int16_t. */
    template< std::integral T1, std::integral T2, scaling_t f, double realVMin, double realVMax >
    struct common_q_base {
    private:
        static constexpr bool isSigned = std::is_signed_v<T1> || std::is_signed_v<T2>;
        static constexpr size_t minSize = std::min(sizeof(T1), sizeof(T2));
        using interm_type = interm_t< typename std::conditional_t<isSigned, int, unsigned> >;
        static constexpr auto vMin = v2s<interm_type, f>(realVMin);
        static constexpr auto vMax = v2s<interm_type, f>(realVMax);
    public:
        using type =
            std::conditional_t< isSigned && sizeof( int8_t ) >= minSize && std::in_range< int8_t >(vMin) && std::in_range< int8_t >(vMax),  int8_t,
            std::conditional_t<!isSigned && sizeof(uint8_t ) >= minSize && std::in_range<uint8_t >(vMin) && std::in_range<uint8_t >(vMax), uint8_t,
            std::conditional_t< isSigned && sizeof( int16_t) >= minSize && std::in_range< int16_t>(vMin) && std::in_range< int16_t>(vMax),  int16_t,
            std::conditional_t<!isSigned && sizeof(uint16_t) >= minSize && std::in_range<uint16_t>(vMin) && std::in_range<uint16_t>(vMax), uint16_t,
            std::conditional_t< isSigned && sizeof( int32_t) >= minSize && std::in_range< int32_t>(vMin) && std::in_range< int32_t>(vMax),  int32_t,
            std::conditional_t<!isSigned && sizeof(uint32_t) >= minSize && std::in_range<uint32_t>(vMin) && std::in_range<uint32_t>(vMax), uint32_t, interm_type>>>>>>;
    };
    /// Alias for common_q_base<>::type.
    template< std::integral T1, std::integral T2, scaling_t f, double realVMin, double realVMax >
    using common_q_base_t = typename common_q_base<T1, T2, f, realVMin, realVMax>::type;

    /** returns the real minimum value for the given integral type and scaling that can safely be
     * used in operations like negation or taking the absolute value
     * (i.e. 0u for unsigned, INT_MIN + 1 for signed).
     * \note Internal. Use Q<>::realVMin in applications. */
    template< std::integral T, scaling_t f >
    consteval double lowestRealVMin() {
        return v2s<double, -f>( std::is_unsigned_v<T> ? static_cast<T>(0) : std::numeric_limits<T>::min() + 1 );
    }

    /** \returns the real maximum value for the given integral type and scaling.
     * \note Internal. Use Q<>::realVMax in applications. */
    template< std::integral T, scaling_t f >
    consteval double highestRealVMax() {
        return v2s<double, -f>( std::numeric_limits<T>::max() );
    }

    /** Concept of a Q-like type.
     * \warning Does not guarantee that T is actually of type Q. Only checks for the basic properties. */
    template< class T >
    concept QType = requires (T t) {
        { std::bool_constant<T::isQType>() } -> std::same_as<std::true_type>;
        typename T::base_t;
        std::is_integral_v<typename T::base_t>;
        std::is_same_v<std::remove_cv_t<typename T::base_t>, typename T::base_t>;
        std::is_same_v<scaling_t, decltype(T::f)>;
        std::is_same_v<double, decltype(T::realVMin)>;
        std::is_same_v<double, decltype(T::realVMax)>;
        std::is_same_v<typename T::base_t, decltype(T::vMin)>;
        std::is_same_v<typename T::base_t, decltype(T::vMax)>;
        std::is_same_v<double, decltype(T::resolution)>;
        std::is_same_v<Overflow, decltype(T::ovfBx)>;
        { t.reveal() } -> std::same_as<typename T::base_t>;
    };

    /** Concept of a Sq-like type.
     * \warning Does not guarantee that T is actually of type Sq. Only checks for the basic properties. */
    template< class T >
    concept SqType = requires (T t) {
        { std::bool_constant<T::isSqType>() } -> std::same_as<std::true_type>;
        typename T::base_t;
        std::is_integral_v<typename T::base_t>;
        std::is_same_v<std::remove_cv_t<typename T::base_t>, typename T::base_t>;
        std::is_same_v<scaling_t, decltype(T::f)>;
        std::is_same_v<double, decltype(T::realVMin)>;
        std::is_same_v<double, decltype(T::realVMax)>;
        std::is_same_v<typename T::base_t, decltype(T::vMin)>;
        std::is_same_v<typename T::base_t, decltype(T::vMax)>;
        std::is_same_v<double, decltype(T::resolution)>;
        { t.reveal() } -> std::same_as<typename T::base_t>;
    };

    /** Concept of a Q-like or a Sq-like type. */
    template< class T >
    concept SqOrQType = SqType<T> || QType<T>;

    /** Concept of a valid (s)q base-type. The base type has to be a reasonably sized integral. */
    template< typename BaseT >
    concept ValidBaseType = (
        std::is_integral_v<BaseT>
        && std::is_same_v<std::remove_cv_t<BaseT>, BaseT>
        && sizeof(BaseT) <= MAX_BASETYPE_SIZE
    );

    /** Concept of a valid (s)q scaling value.
     * Types can be scaled by maximal the number of bits in the base type minus one bit, limited by the
     * size of double's mantissa. */
    template< typename BaseT, scaling_t f >
    concept ValidScaling = (
        f <= std::numeric_limits<std::make_signed_t<BaseT>>::digits  // CHAR_BIT * sizeof(BaseT) - 1
        && f <= MAX_F
    );

    /** Concept of a valid difference between two integral types and two scaling factors to support
     * scaling without overflow or significant loss of precision.
     * Scaling is possible if:
     *  - target and source type are of equal size and the effective target value range is not shifted out, or
     *  - target and source type are of different size and the effective size difference is not shifted out, or
     *  - both types are equal and the number of shifted bits is at most the number of bits in each type
     * \note If this fails, none of the conditions listed above is fulfilled. Double-check! */
    template< typename from_t, scaling_t fFrom, typename to_t, scaling_t fTo >
    concept Scalable = (
        ( sizeof(from_t) == sizeof(to_t) && details::abs(fTo - fFrom) <= std::numeric_limits<to_t>::digits )
        || ( sizeof(from_t) != sizeof(to_t)
            && details::abs(fTo - fFrom) <= details::abs(std::numeric_limits<to_t>::digits - std::numeric_limits<from_t>::digits) )
        || ( std::is_same_v<from_t, to_t> && details::abs(fTo - fFrom) <= sizeof(to_t) * CHAR_BIT )
    );

    /** Concept of similar Q or Sq types, i.e. they have the same base type and scaling. */
    template< class T1, class T2 >
    concept Similar = (
        ((SqType<T1> && SqType<T2>) || (QType<T1> && QType<T2>))
        && std::is_same_v<typename T1::base_t, typename T2::base_t>
        && T1::f == T2::f
    );

    /** Concept of a valid scaled value that fits the specified base type. */
    template< typename BaseT, scaling_t f, double realValue >
    concept RealValueScaledFitsBaseType = ( std::in_range<BaseT>(v2s<interm_t<BaseT>, f>(realValue)) );

    /** Concept of a valid (s)q type value range that fits the specified base type.
     * \note If this fails, the specified real value limits exceed the value range of the selected
     * base type when scaled with the desired scaling factor. Double-check the sign and value of your
     * selected real limits! */
    template< typename BaseT, scaling_t f, double realVMin, double realVMax >
    concept RealLimitsInRangeOfBaseType = (
        RealValueScaledFitsBaseType<BaseT, f, realVMin>
        && RealValueScaledFitsBaseType<BaseT, f, realVMax>
        && realVMin <= realVMax
        && (std::is_signed_v<BaseT> || realVMin >= 0.)
    );

    /** Concept of a base type that can overflow when allowed. Typically used in the context of casting.
     * \note In C++23, signed int overflow (i.e. the value does not fit in the type) is still undefined.
     * \note If this fails, a signed base type is used in the context of a potential overflow. This is
     *       not allowed. Use an unsigned target base type! */
    template< typename BaseT, Overflow ovfBx >
    concept BaseTypeCanOverflowWhenAllowed = ( std::is_unsigned_v<BaseT> && ovfBx == Overflow::allowed );

    /** Concept: Runtime overflow check is allowed when needed.
     * \note If this fails, a runtime overflow check is needed but not allowed for the desired q type.
     *       Allow for type, or specify the overflow-override template argument (to be preferred)! */
    template< Overflow ovfBx, bool checkNeeded = true >
    concept RuntimeOverflowCheckAllowedWhenNeeded = ( !checkNeeded || Overflow::forbidden != ovfBx );

    /** Concept: Checks whether From can be converted implicitly to To. This is possible when the
     * base type is the same, scaling is possible and the value range of From is fully within the
     * value range of To, or if the target type is a Q type without overflow checks. */
    template< class From, class To >
    concept ImplicitlyConvertible = (
        SqOrQType<From> && SqOrQType<To>
        && std::is_same_v<typename From::base_t, typename To::base_t>
        && details::Scalable<typename From::base_t, From::f, typename To::base_t, To::f>
        && ((QType<To> && !is_ovf_stricter_v<To::ovfBx, Ovf::noCheck>)
            || (To::realVMin <= From::realVMin && From::realVMax <= To::realVMax))
    );

    /** Concept: Checks whether casting of From to To is possible without an overflow check. This is
     * possible if scaling is possible and the value range of the source type is fully within the
     * range of the target type. */
    template< class From, class To >
    concept CastableWithoutChecks = (
        SqOrQType<From> && SqOrQType<To>
        && details::Scalable<typename From::base_t, From::f, typename To::base_t, To::f>
        && To::realVMin <= From::realVMin && From::realVMax <= To::realVMax
    );

    /** Concept: Checks whether the absolute value can be taken for a value of the given base type.
     * This is possible if the base type is integral and unsigned, or signed but without INT_MIN in
     * its value range. */
    template< typename BaseT, BaseT vMin >
    concept Absolutizable = (
        std::is_integral_v<BaseT>
        && (std::is_unsigned_v<BaseT> || (std::is_signed_v<BaseT> && vMin != std::numeric_limits<BaseT>::min()))
    );

    /** Concept: Checks whether the given (S)Q-Type can be used as divisor in a division. This is
     * possible if the given type doesn't have parts of -1 > x < +1 in its range. This is not allowed
     * in order to prevent an uncontrollable explosion of the limits. */
    template< typename T >
    concept CanBeUsedAsDivisor = (
        T::realVMax <= -1. || +1. <= T::realVMin
    );

    /** Concept: Checks whether the given (S)Q-Type can be used as divisor in a remainder-division
     * (modulus). This is possible as long as the given type doesn't have parts of
     * -resolution > x < +resolution in its range to prevent a modulo zero. */
    template< typename T >
    concept CanBeUsedAsModulusDivisor = (
        T::realVMax <= -T::resolution || T::resolution <= T::realVMin
    );

    /** Concept: Checks whether the two given base types can be compared.
     * Comparison is possible if both types have the same signedness, or if the size of the lhs type
     * is larger than the size of the rhs type if the signedness is different. */
    template< typename LhsT, typename RhsT >
    concept Comparable = (
        std::is_integral_v<LhsT> && std::is_integral_v<RhsT>
        && (std::is_signed_v<LhsT> == std::is_signed_v<RhsT> || sizeof(LhsT) > sizeof(RhsT))
    );

    /** Concept: Checks whether a value of the given type SqV can be clamped to a value range between
     * a value of the given type SqLo and a value of the given type SqHi.
     * \note In order to avoid overflow, both types SqLo and SqHi have to be implicitly-convertible
     * to SqV and SqLo must have a lower minimum than SqHi, which in turn must have the larger maximum. */
    template< class SqV, class SqLo, class SqHi >
    concept Clampable = (
        SqType<SqV> && SqType<SqLo> && SqType<SqHi>
        && details::ImplicitlyConvertible<SqLo, SqV> && details::ImplicitlyConvertible<SqHi, SqV>
        && SqLo::realVMin <= SqHi::realVMin && SqLo::realVMax <= SqHi::realVMax
    );

    /** Concept that defines the requirements for a valid Q type. */
    template< typename BaseT, scaling_t f, double realVMin, double realVMax, Overflow ovfBx >
    concept QRequirements = (
        details::ValidBaseType<BaseT>
        && details::ValidScaling<BaseT, f>
        && details::RealLimitsInRangeOfBaseType<BaseT, f, realVMin, realVMax>
    );

    /** Concept that defines the requirements for a valid Sq type. */
    template< typename BaseT, scaling_t f, double realVMin, double realVMax >
    concept SqRequirements = (
        details::ValidBaseType<BaseT>
        && details::ValidScaling<BaseT, f>
        && details::RealLimitsInRangeOfBaseType<BaseT, f, realVMin, realVMax>
    );

}  // end of details


/**\}*/
}  // end of fpm
#endif
// EOF
