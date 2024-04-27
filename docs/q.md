# Q-Type

> *Just like the enigmatic **Q** from Star Trek, the `Q` type in the Fixed-Point Math Library possesses its own set of powerful capabilities, albeit more grounded in the realm of arithmetic precision than in galactic mischief.* -- ChatGPT

The primary purpose of `Q` is to efficiently implement a fixed-point type that mimics real, comma-separated numbers within a specific real value range, wrapping around a scaled integer value that can be stored and modified at runtime, ensuring dynamic and precise arithmetic operations. Additionally, `Q` provides robust overflow protection and facilitates seamless conversions to and from other `Q` types.

## Template

`Q` is declared by the following template:

```cpp
namespace fpm {

template< std::integral base_t,
          scaling_t f,
          double realMin,
          double realMax,
          Overflow ovfBx >
class Q {
    public:
    // ...
    private:
        base_t value;  ///< actual integer value stored in runtime memory
};

}
```

- **base_t**: Refers to the underlying integral type that holds the scaled value. Within the `Q` class, the scaled value, which represents the fixed-point number, is declared as a private member. This value is the only element that is stored in runtime memory for the `Q` instance.
- **f**: The number of fractional bits. This parameter, defined as `fpm::scaling_t`, determines the scaling factor, affecting how much of the integer's precision is used for fractional parts.
- **realMin** and **realMax**: These specify the minimum and maximum values of the real value range that `Q` can represent, effectively setting the bounds for compile-time computation. They are optional; if not explicitly set, the default is the largest feasible value range. For symmetric ranges in signed types, the default excludes the minimum value of the base type to prevent potential overflows when taking the absolute value, thereby ensuring safe operations within predictable limits. For a `Q` type with a signed base type, if the value range includes only negative values up to and including zero, it is highly recommended to use `-0.0` (note the explicit minus sign) as the upper limit to clearly define the endpoint of the negative value range. This distinction is recommended because, although the runtime scaled integer does not differentiate between `-0` and `+0`, the `Q` type does at compile-time. This can make a difference in how the compiler interprets and handles the boundary conditions and overflow checks.
- **ovfBx**: Dictates the overflow behavior for operations that exceed the designated value range. The default overflow behavior is `Ovf::error`, which means the code will not compile if a potential overflow scenario is detected. This is particularly common when a conversion from a wider to a narrower real value range is attempted, safeguarding against inadvertent data loss or corruption.

These parameters are integral to the structure of each `Q` type and are set as static constexpr members, allowing them to be accessed directly for various computations or validations.

Additionally, a `Q` type provides the following static constexpr members to aid in precise and effective data manipulation:

- **scaledMin**: This member stores the minimum value that can be represented in the scaled format, calculated based on the *realMin* and the scaling factor.
- **scaledMax**: This member stores the maximum value that can be represented in the scaled format, calculated based on the *realMax* and the scaling factor.
- **resolution**: This represents the real resolution (double) of the `Q` type, defined as \(2^{-f}\). It indicates the smallest difference between two representable real values within the type, providing clarity on the granularity and precision at which values can be manipulated or interpreted.

## Overflow Behaviors

The `Q` type provides a robust handling of overflow scenarios through different behaviors that can be specified during the type definition. Understanding and selecting the appropriate overflow behavior is critical to ensure that your application handles edge cases in a predictable and controlled manner. The library currently implements four types of overflow behaviors (from strictest to most lenient):

- **Error (`Ovf::error`)**: This behavior causes a compiler error if overflow is possible. It is the strictest behavior and the **default setting**, ensuring that potential overflow scenarios are addressed during development rather than at runtime.

- **Assert (`Ovf::assert`)**: This overflow behavior triggers a runtime assertion and calls the application-defined `ovfAssertTrap()` function if overflow occurs. This is useful for debugging and development phases where catching errors immediately is crucial.

- **Clamp (`Ovf::clamp`)**: With this behavior, values that would normally overflow are clamped at runtime to the maximum or minimum value within the range defined for the `Q` type. This prevents overflow while still allowing the application to continue running.

- **Unchecked (`Ovf::unchecked`), Allowed (`Ovf::allowed`)**: This setting disables overflow checking entirely. It allows the value to wrap around according to the standard behavior of the underlying data type. "Unchecked" and "Allowed" refer to the same behavior, however, the choice of term may fit better semantically depending on the context or usage within specific parts of your application.

Choosing the right overflow behavior depends on your application’s requirements for safety, debugging, and performance. It may even vary between different builds if corresponding declarations are used.

## Construction

The `Q` class can be instantiated using various integral types and scaling factors to fit specific application requirements. Here are some examples of how `Q` types can be defined:

- `fpm::Q<int8_t, 4, -10., 10.>` represents a `Q8.4` fixed-point type, covering a real value range from -10 to +10. The default overflow behavior is set to `Ovf::error`, meaning any operation leading to overflow will not compile.
- `fpm::Q<uint32_t, 12, 0., 1000., Ovf::clamp>` defines a `UQ32.12` fixed-point type with a real value range from 0 to 1000. Overflow behavior is set to `Ovf::clamp`, meaning values that exceed the range are adjusted to the nearest boundary.
- `fpm::Q<in16_t, 8>` defines a `Q16.8` fixed-point type that uses `Ovf::error` as the default overflow behavior and the largest possible **symmetric** value range. This symmetric range excludes `INT16_MIN` as scaled value to avoid issues such as overflow when taking the absolute value.

`Q` cannot be constructed directly from floating-point values to maintain type safety and prevent inadvertent data loss or misinterpretations at runtime. Instead, `Q` provides static constructor methods to create instances from real values, ensuring conversions are explicit and controlled:

- To construct a `Q` instance from a real constexpr value, you can use the static `fromReal< real >()` constructor function, which converts a floating-point number to the corresponding fixed-point representation. For example:
  ```cpp
  using fpm::Q;

  // Construct from real value
  auto a = Q<uint16_t, 8, 100., 200.>::fromReal< 155. >();
  ```
  The `auto` keyword is very handy here because it spares us from needing to specify the type twice, as the compiler deduces the type at compile time.
- Additionally, Q supports construction from scaled constexpr integer values through the static `fromScaled< scaled >()` constructor function, which takes an integer already scaled to the fixed-point format:
  ```cpp
  // Construct from scaled value
  auto b = Q<uint32_t, 8, 1000., 2000.>::fromScaled< 307200 >();
  auto c = Q<int32_t, 18, -10., 15.>::fromScaled< fpm::scaled<18>(12.) >();
  ```

- For creating instances of `Q` types with scaled integral runtime values, the constructor function  
`construct<ovfBxOvrd>( scaled )` is essential. It conducts a runtime overflow check as desired, adhering to the overflow behavior of the `Q` type, or, if specified, the behavior override provided via the template parameter `ovfBxOvrd`. This function ensures that values are instantiated safely, respecting the defined overflow strategies to prevent data corruption or unexpected behavior. It's important to note that this method can only be used with `scaled` integer values, as the runtime construction from double values is explicitly prohibited by the library's design to maintain type safety (no floats at runtime).
  ```cpp
  // Construct with runtime scaled value, clamped to target value range
  int32_t scaled = 123456;  // scaled value, q16; real=1.88379
  auto qVal = Q<int32_t, 16, -100., 100.>::construct<Ovf::clamp>(scaled);

  // Overflow override not necessary if target type inherently clamps
  auto qVal2 = Q<int32_t, 16, -100., 100., Ovf::clamp>::construct(scaled);
  ```

These construction methods provide precise control over how values are initialized in the `Q` type, ensuring adherence to the defined numerical ranges and overflow behaviors. Values are defined within the `< >` brackets at compile-time, and within the `( )` at runtime. It's important not to forget the `()` at the end of the static functions `Q<...>::fromReal<.>()` and `Q<...>::fromScaled<.>()`, as functions require parentheses to invoke them.

### Type Aliases

The library provides alias templates as syntax sugar for common `Q` types, looking similarly to standard integer types, in the namespace `fpm::types`. This simplifies the usage and readability of the types in your code. For instance:

- `i32q16< realMin, realMax, Ovf::clamp >` is a `Q<int32_t, 16, realMin, realMax, Ovf::clamp>`
- `u16q8< realMin, realMax >` is a `Q<uint16_t, 8, realMin, realMax>`
- `i32qm7< realMin, realMax >` is a `Q<int32_t, -7, realMin, realMax`
- `i16q6<>` is a `Q<int16_t, 6>`, with the largest possible symmetric value range and `Ovf::error`

Construction of `Q` instances using these aliases looks as follows:
```cpp
using namespace fpm::types;  // predefined type alias templates

// user-defined type aliases with specific value ranges
using i32qm7_t = i32qm7<4e10, 5e10, Ovf::clamp>;
using i16q6_t = i16q6<>;

auto a = u16q8<100., 200.>::fromReal< 155. >();
auto c = i32q18<1000., 1500.>::fromScaled< 314572800 >();
auto d = i32qm7_t::fromReal< 4.567e10 >();
auto e = i16q6_t::fromReal< 444. >();
```

In the examples provided in this document, these aliases will mostly be used for construction, as they offer a more concise and familiar notation.

### Some Syntax Sugar: Literals

C++ provides the flexibility to define user-defined literals, enhancing the expressiveness and clarity of code by allowing types to have their own suffixes. The `fpm` library supports this feature with a macro that simplifies the creation of these literals for specific `Q` types.

The macro `FPM_Q_BIND_LITERAL(_q, _literal)` enables the binding of a custom suffix to a `Q` type, making the use of fixed-point types more intuitive. For example:

```cpp
// Define speed as a range from 0 to 100 meters per second.
using speed_t = i32q8<0., 100.>;
// Bind the 'm_per_s' literal to the speed_t type.
FPM_Q_BIND_LITERAL(speed_t, m_per_s)  // m/s

// Define cruising speed using the user-defined literal.
auto cruisingSpeed = 55.0_m_per_s;
```

*Note: When using user-defined literals, a `_` must be placed between the number and the literal suffix, as shown in `55.0_m_per_s`. This ensures proper parsing and application of the literal in C++.*

This approach not only makes the code more readable but also reduces the risk of errors by enforcing type-specific operations directly through the literal notation.

While user-defined literals offer a significant advantage in code clarity and type safety, there is a limitation to their usage. Each literal can only be bound once per compilation unit to a specific `Q` type. This restriction ensures that literals are consistently associated with their respective types throughout a single file, preventing conflicts and ambiguities.

Nevertheless, it is often practical for certain literals to be used across the entire application. To accommodate this, such literals should be defined in a core header file. This centralized approach allows for the universal application of the literal across multiple files, ensuring consistent usage and behavior throughout the application.

#### Provided Literals in fpm Library

The `fpm` library automatically provides literals for all fixed-point types defined in the `fpm::types` namespace, ensuring a consistent and easy way to work with these types directly in your code. Each type has a corresponding literal suffix that mirrors its type declaration, enhancing code readability and precision.

An important characteristic of these literals is that the value range specified by the literal is only the value itself, which provides flexibility and avoids unnecessary limitation of calculations.

Here are examples of how these literals can be utilized:

```cpp
#include <fpm.hpp>
using namespace fpm::types;

i32q8<-1000., 1000.> distance = -150.0_i32q8;
u16q4<0., 2000.> pressure = 1023.0_u16q4;
u8q4<0., 15.> smallValue = 14.5_u8q4;
```

These literals, named after their respective types (such as `_i32q8` for `i32q8` and `_u16q4` for `u16q4`), make it clear what type of fixed-point precision is being applied, making the code self-documenting and easier to understand. This standardized approach ensures that developers can immediately recognize the data types and precision being used just by looking at the literal suffix.

## Value Access

Within the `Q` class, the actual scaled value that is stored in runtime memory can be accessed through specific member functions. These functions provide direct and controlled access to both the scaled representation and the real-value representation of the `Q` type:

- **Accessing the Scaled Value:**
  The scaled value can be accessed using the `scaled()` member function. This function returns the value in its internal scaled format, which is useful for low-level operations or when interfacing with systems that require the scaled integer directly.

  ```cpp
  auto scaledVal = qValue.scaled();  // Access the internal scaled value
  ```

- **Accessing the Real Value:**
To obtain the real value as a floating-point number, you can use the `real()` function. This method converts the value back to its double precision floating-point representation, primarily useful for debugging reasons. It's important to use this function cautiously due to potential floating-point inaccuracies and the computational overhead associated with floating-point operations.

  ```cpp
  double realVal = qValue.real();  // Get the real value as double
  ```

- **Accessing the Real Value as an Integer:**
  For accessing the real value in any integer format, use `real<typename>()`. This method converts the unscaled real value to the given integral type by truncating it according to C++ rules, discarding any fractional digits. This truncation must be handled with care to avoid unintended data loss, especially when the type has a large fractional part.

  ```cpp
  // Get the real value as an integer, truncating fractional digits
  int intRealVal = qValue.real<int>();
  ```

## Rescaling

In situations where the base type remains the same but the fractional part `f` differs between two `Q` types, rescaling is necessary. This process is referred to as "up-scaling" when `f` increases, and "down-scaling" when `f` decreases.

- **Up-Scaling**: Up-scaling is achieved by **multiplying** the source value by \(2^{to-from}\), effectively increasing the number of fractional bits to fit the higher resolution of the target type. While this process is technically lossless because no data is discarded, it doesn't enhance the actual resolution or detail of the original data; the additional fractional bits are essentially filled with zeros.

- **Down-Scaling**: Down-scaling reduces the number of fractional bits by **dividing** the source value by \(2^{from-to}\). This operation is inherently lossy as it involves discarding the least significant bits of the fractional part. Consequently, the precision of the original value is reduced to match the lower resolution of the target type, resulting in a loss of detail that reflects the decreased fractional granularity.

To rescale a `Q` type to another with the same base type but different `f`, you can use simple assignment, or, if necessary, the `QTo::fromQ<ovfBxOvrd>( qFrom )` constructor function, both of which handle the scaling adjustments internally:

```cpp
auto sourceValue = i32q10<-500., 1500.>::fromReal< 1024. >();

// Rescale from f=10 to f=8 and a wider value range via assignment
i32q8<-510., 1500.> targetValue1 = sourceValue;  // automatically rescaled

// Rescale from f=10 to f=8 and clamp to the narrower value range via fromQ<>()
auto targetValue2 = i32q8<-450., 1450.>::fromQ<Ovf::clamp>( sourceValue );
```

Overflow checks during scaling adjustments are unnecessary if the real value range of the target type is the same as or wider than that of the source type, and if the overflow behavior of the target type is the same or less strict than that of the source type. Under these conditions, values can be directly assigned.

However, if the target value range is narrower than the source's, or if the target type implements a stricter overflow behavior than the source type, you may need to use the `fromQ<ovfBx>(.)` function to specify a different overflow behavior. Typically, the value is clamped to fit within the target type's range, ensuring correct handling of potential overflow situations without causing compilation errors or runtime assertions. This adjustment does not permanently change the overflow behavior of the target type but applies the specified overflow handling only for the conversion to the target type.

```cpp
using source_t = i32q10<-500., 1500.>;   // i32q10, Ovf::error
using overflow_t = i32q10<-500., 1500., Ovf::allowed>;
using coarser_t = i32q8<-500., 1500.>;   // i32q8, same value range as source_t
using wider_t = i32q8<-600., 1600.>;     // i32q8, wider value range
using narrower_t = i32q8<-400., 1500.>;  // i32q8, narrower value range

auto source = source_t::fromReal< 1024.7 >();
auto ovfSrc = overflow_t::fromReal< -555.5 >();  // value out of range

/* same value range: direct assignment can be used if ovf behavior allows it */
coarser_t a1 = source;                        // ok
auto a2 = coarser_t::fromQ( source );         // also ok
// coarser_t a3 = ovfSrc;                     // error (stricter ovf behavior)
auto a4 = coarser_t::fromQ<Ovf::clamp>( ovfSrc );  // ok (runtime clamp needed)
i32q8<-500., 1500., Ovf::clamp> a5 = ovfSrc;  // ok (clamping target)

/* wider value range: direct assignment can be used if ovf behavior allows it */
wider_t b1 = source;                             // ok
auto b2 = wider_t::fromQ( source );              // also ok
// wider_t b3 = ovfSrc;                          // error (stricter ovf beh.)
auto b4 = wider_t::fromQ<Ovf::clamp>( ovfSrc );  // ok (runtime clamp needed)
i32q8<-600., 1600., Ovf::clamp> b5 = ovfSrc;     // ok (clamping target)

/* narrower range: fromQ<>() is required unless target type checks anyway */
// narrower_t c1 = source;  // error (narrower range)
auto c2 = narrower_t::fromQ<Ovf::clamp>( source );  // ok
auto c3 = narrower_t::fromQ<Ovf::clamp>( ovfSrc );  // ok
i32q8<-400., 1500., Ovf::clamp> c4 = ovfSrc;        // ok (clamping target)
i32q8<-400., 1500., Ovf::unchecked> c5 = ovfSrc;    // ok (check discarded)
```

## Casting

In the `Q` type framework, casting between different `Q` types with varying base types necessitates explicit type conversion. The simplest form of casting involves using `static_cast< QTo >( qFrom )`, which is straightforward when scaling adjustments are feasible and the target value range is either the same or wider than the source's, and if the target type's overflow behavior is the same or less strict.

```cpp
auto sourceValue = i32q10<0., 1500.>::fromReal< 1024. >();

// cast i32q10 to unsigned u32q12
auto cast1 = static_cast< u32q12<0., 1500.> >(sourceValue);

// cast i32q10 to i16q4, explicitly dropping any overflow check in target type
// (not recommended though, target type will drop any overflow check this way)
auto cast2 = static_cast< i16q4<0., 1030., Ovf::unchecked> >(sourceValue);
```

However, if the target range is narrower and the target type incorporates overflow checks, or if the target type's overflow behavior is more restrictive than that of the source, `static_cast` may not be sufficient. In cases where overflow could occur, and unless the target type handles overflow with `Ovf::clamp` or `Ovf::assert` when constructed, a simple `static_cast` will not properly address potential overflow issues and thus not compile.

To manage potential overflow effectively, `Q` provides three specialized casting functions:

- `static_q_cast< QTo, ovfBxOvrd >( qFrom )`: This function applies the overflow handling as specified by the target type or uses the overridden overflow behavior if provided. It ensures that the conversion adheres to the designated overflow protocols, **performing runtime checks only when necessary** from a static point of view.

- `safe_q_cast< QTo, ovfBxOvrd >( qFrom )`: This function **performs a runtime overflow check regardless of whether it is strictly necessary**. It requires that the overflow behavior is set to a runtime check `Ovf::clamp` or `Ovf::assert`, and does not permit `Ovf::unchecked` and the compile-time check `Ovf::error`. This method is designed for scenarios where ensuring data integrity is critical, and overflow must be actively managed.

- `force_q_cast< QTo >( qFrom )`: This casting function **performs no checks and simply reinterprets the scaled source value**, constructing the specified `ToQ` type around it, potentially truncating bit information. This is essentially a forced value reinterpretation and should be used with caution, as it bypasses all overflow and range checks as well as rescaling. This method is suitable for scenarios where the developer is certain of the data integrity and the applicability of the conversion.

These casting functions provide robust tools for managing different fixed-point types, ensuring that conversions are both safe and efficient, depending on the operational requirements and data integrity needs.

```cpp
auto value = i32q10<0., 1500.>::fromReal< 1234. >();

// cast value to unsigned u32q12 (identical to static_cast)
auto cast1 = static_q_cast< u32q12<0., 1500.> >( value );  // 1234.

// cast value to i16q4 with narrower range, overflow check is dropped explicitly
// --> value intentionally is out of range
auto cast2 = static_q_cast< i16q4<0., 1000.>, Ovf::allowed >( value );  // 1234.

// static-cast cast2 (potentially out of range) to a wider range
// note: no overflow checks are performed (wider range, identical ovf behavior)
auto c3static = static_q_cast< i16q4<-100., 1100.> >( cast2 );  // 1234.

// safe-cast of cast2 is needed to really catch and clamp the out-of-range value
auto c3safe = safe_q_cast< i16q4<-100., 1100.>, Ovf::clamp >( cast2 );  // 1100.

// force-cast cast2 (i16q4) into a u8q1
// --> u8q1<0., 100.>::fromScaled< cast2.scaled() % 256 >() = 32|scaled, 16|real
auto forced = force_q_cast< u8q1<0., 100.> >( cast2 );  // 16.
```

### Range Clamping

The `Q` type framework includes a specialized feature known as "range clamping" to address the nuances of casting between differently signed types of different sizes. This feature ensures that conversions between signed and unsigned types adhere to intuitive expectations regarding value ranges:

- **From Signed to Unsigned**: When a `Q` type with a signed base type is cast to a `Q` type with an unsigned base type of the same or smaller size, any values in the negative range of the signed type are clamped to the lower limit of the unsigned type. This clamping is crucial to avoid underflow and to ensure values remain within the legitimate range of the unsigned type.
- **From Unsigned to Signed**: Similarly, when a `Q` type with an unsigned base type is cast to a `Q` type with a signed base type of the same or smaller size, values that fall in the upper half of the unsigned range are clamped to the upper limit of the signed type. This measure prevents potential overflow, ensuring that the values do not exceed the maximum positive value of the signed type.
- **From Larger Signed Type to Smaller Signed Type**: When casting from a larger signed `Q` type to a smaller signed type, clamping adjusts any values that exceed the storage capabilities of the smaller type to the nearest boundary. This is crucial for preventing data overflow or underflow, helping maintain the integrity of the data.
- **From Larger Unsigned Type to Smaller Unsigned Type**: In cases where a larger unsigned `Q` type is cast to a smaller unsigned type, the values that exceed the maximum representable value of the smaller type are clamped to this maximum. This adaptation is essential for preserving data correctness by preventing overflow.

```cpp
auto iN = i32q10<-1300., 1300.>::fromReal< -1234. >();
auto iP = i32q10<-1300., 1300.>::fromReal< 1234. >();
auto uP = u16q8<0., 255.>::fromReal< 234. >();

// cast negative signed i32q10 to unsigned u32q5
auto iN2u = static_q_cast< u32q5<0., 3000.>, Ovf::clamp >( iN );  // 0.

// cast positive signed i32q10 to smaller unsigned u16q8
auto iP2u = static_q_cast< u16q8<0., 255.>, Ovf::clamp >( iP );  // 255.

// cast negative signed i32q10 to signed i16q2 with narrower value range
auto iN2i = static_q_cast< i16q2<-1000., -0.>, Ovf::clamp >( iN );  // -1000.

// cast positive signed i32q10 to signed i8q1
auto iP2i = static_q_cast< i8q1<-60., 60.>, Ovf::clamp >( iP );  // 60.

// cast unsigned u16q8 to signed i8q1
auto u2i = static_q_cast< i8q1<-60., 60.>, Ovf::clamp >( uP );  // 60.

// cast unsigned u16q8 to unsigned u8q2 with narrower value range
auto u2u = static_q_cast< u8q2<0., 30.>, Ovf::clamp >( uP );  // 30.
```

These adjustments are driven by internal logic designed to mirror what would typically be expected from the real values during such type conversions. By incorporating this feature, the framework helps preserve data integrity and provides a logical, safe transition between signed and unsigned `Q` types, reflecting the expected behavior of real-world values.

#### How Does Range Clamping Work?

The process of range clamping in the `Q` type framework is designed to handle conversions between types of different sizes and signs meticulously to avoid data loss and ensure integrity. Here's a detailed breakdown of how this process works:

1. **Rescaling the Base Value**: Initially, the base value of the source `Q` type is rescaled in a `scale_t`, which has twice the size and retains the same sign as the `base_t` of the source `Q` type. This step is crucial as it preserves the sign bit during the scaling process, particularly important when the source type is signed and the operation might involve a reduction in size (down-cast).

2. **Casting to a Transitional Type**: The scaled value is then cast to a transitional `cast_t`, which maintains the size of `scale_t` but adopts the sign of the target `Q` type. This intermediary step ensures that the value can be adjusted in a way that conforms to the sign characteristics of the target type, facilitating a smoother transition and accurate clamping in subsequent steps.

3. **Clamping and Final Adjustment**: In the final stage, the value now represented in `cast_t` allows for proper clamping. For instance, if the original value was negative and the target type is unsigned, the value falls within the upper half of the value range of `cast_t`. This positioning is key as it indicates that the value requires clamping to the lower limit of the unsigned target type to be clamped intuitively. Conversely, if the original value was positive and fits into the value range of the target type, the cast value will fit and not be clamped. The larger intermediate type (`cast_t`) provides the necessary headroom to accommodate these adjustments without overflow or underflow, ensuring that the final value is accurate and within the expected range.
