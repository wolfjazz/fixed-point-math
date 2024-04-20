# Fixed-Point Math

This library introduces a fixed-point mathematical system designed specifically for embedded C++ systems. It simplifies integer math operations and incorporates compile-time and carefully optimized runtime checks to ensure that integers do not exceed predefined value ranges.

## Motivation

In embedded systems programmed with C/C++, it is typical to avoid built-in floating-point types like `float` and `double` due to their significant impact on code size and execution time—critical factors in microcontrollers. A common solution is to use integers scaled by a fixed power of two, allowing representation of fractional values. For instance, a 32-bit unsigned integer scaled by the factor $2^{16}$ splits the integer into 16 bits for the whole numbers (0-65535) and 16 bits for the fractional part, offering a precision of $2^{-16}$
. This `UQ16.16` fixed-point type, prevalent in ARM's Q notation, can represent values from 0 to 65535.99998474. Negative values are typically handled using signed integers in two's complement form, like the `Q14.2` type, which spans from -8192.00 to 8191.75.

### Problems and Expectations

While the concept of fixed-point math is straightforward, it often becomes a source of bugs:
- Mathematical operations, especially multiplications and complex formulas, can quickly exceed the value range of the base type, leading to silent overflows and unpredictable results if no overflow checks are in place.
- Manual calculations with scaled values are cumbersome and error-prone as scaling corrections are easily overlooked due to their non-intuitive nature.

The goal is to enable calculations within a specified value range and precision without constant concerns about overflow. Here are the features expected from a well-designed fixed-point library which tackles the aforementioned problems:

- Predefined fixed-point types based on both signed and unsigned integer types (8, 16, 32, and possibly 64 bits).
- User-defined precision and value range set at compile-time.
- Specification of value range using float values at compile-time for intuitive setup.
- Capability to adjust precision and/or value range later in the code, still at compile-time.
- Prohibit implicit construction from floating-point types to prevent ambiguity.
- Support for implicit construction from static integers in formulas (current limitations prevent constexpr function arguments; literals are used instead).
- Explicit construction from both scaled integers and real floating-point values at compile-time.
- Explicit construction from integer-based variables with scaled values at runtime.
- Disallow runtime construction from floating-point variables to maintain integrity in resource-constrained environments.
- Implement compile-time overflow checks where feasible, with runtime checks as necessary.
- Various overflow handling strategies (e.g., error, assert, clamp, or allow unchecked overflow).
- Implicit conversions between types of the same base type only if it results in higher precision; explicit conversion for down-scaling to lower precision.
- Conversion between different base types only through explicit casts (static cast, safe cast, force cast).
- Straightforward, easy-to-debug formulas without obscuring scaling corrections.
- Implementation of standard mathematical operations and specialized functions such as square, sqrt, cube and cbrt using adapted formulas to maintain integer-only calculations.
- Advanced mathematical operators and functions, like reciprocal square roots, implemented to accommodate the fixed-point calculation model.

### Additional Considerations

The library aims to extend further, potentially including functions like trigonometric and logarithmic calculations adapted for fixed-point types.  
It is crucial to emphasize that operations on Q and Sq types, which are fundamentally compile-time constructs built around basic integral types stored in memory at runtime, are not all thread-safe.

## Getting Started

To begin using the Fixed-Point Math Library in your project, start by including the library header file in your source code:

```cpp
#include <fpm.hpp>
```

Next, make use of the library's definitions and types by adding the following namespace directives to your source files:

```cpp
using namespace fpm::types;
using Ovf = fpm::Ovf;  // optional
```

This setup will allow you to easily access and utilize the various fixed-point types provided by the library in your applications. For clarity, the initial examples in this document may include these lines of prefix explicitly. Moving forward, these lines will not be shown in any further examples within this document, unless they are part of what is shown.

### Utilities

One of the foundational declarations is the definition of `scaling_t` as an alias for `int`. The decision to make `scaling_t` signed is strategic because the framework needs to support both positive and negative fractional values:

- **Positive Scaling Values**: These are used to represent fractional parts, where positive scaling factors enhance the precision of the fractional component by specifying how many bits are dedicated to values below the integer point.
- **Negative Scaling Values**: Negative scaling is used to represent larger numbers than those typically representable with the given bits of the used integral base type. By scaling the real number by $2^f$, where $f$ is negative, large numbers can be represented at the cost of reducing the precision of the integral part of the number by $2^{|f|}$, effectively balancing the range and precision according to specific needs.

This flexibility ensures that the framework can adapt to a variety of numerical ranges and precision requirements, making it highly versatile for different applications.

#### Helper Functions

The library includes a set of helper functions designed to facilitate the manipulation of values at compile-time, ensuring that they are correctly scaled for use with different Q types. These functions are critical for creating and converting scaled values accurately and efficiently.

- **`v2s` (Value to Scale)**: This function is used to convert a real floating-point value into a scaled integer value at compile time. It multiplies the given `ValueT` value by $2^{to}$, allowing for precise initialization of fixed-point variables from real numbers within the resolution of `TargetT`.
- **`s2s` (Scale to Scale)**: This function changes the scaling of a given scaled integer value. It multiplies the value by $2^{to-from}$, enabling the conversion of a value from one fixed-point scale to another without losing precision relative to the scaling factor.
- **`scaled`**: Converts a given real value (double) to its scaled representation, using the specified scaling factor `f`. This function is effectively an alias for `v2s<f, base_t>(real)`, streamlining the process of initializing `Q` types with precise scaled values derived from real numbers.
- **`real`**: Converts a scaled integral value back to its real (double) representation using the given scaling factor `f`. This function serves as an alias for `s2s<f, 0, double>(scaled)` (or `v2s<-f, double>(scaled)`), facilitating the conversion of scaled values into their real, human-readable forms, particularly useful for outputs and debugging.

The functions are declared as follows:

```cpp
namespace fpm {

template< scaling_t to, typename TargetT, typename ValueT >
constexpr TargetT v2s(ValueT value) noexcept { /* ... */ }

template< scaling_t from, scaling_t to, typename TargetT, typename ValueT >
constexpr TargetT s2s(ValueT value) noexcept { /* ... */ }

template< scaling_t f, std::integral TargetT = int >
constexpr TargetT scaled(double real) noexcept { /* ... */ }

template< scaling_t f, typename TargetT = double >
constexpr TargetT real(std::integral auto scaled) noexcept { /* ... */ }

}
```

Here are practical examples demonstrating how to use the helper functions provided by the library:

- Using `fpm::v2s` to convert a floating-point number to a scaled integer value:
  ```cpp
  // Converts 10.5 to a scaled integer using a scale factor of 2^3
  auto scaledValue = fpm::v2s<3, int8_t>(10.5);  // 84: int8_t
  ```

- Using `fpm::s2s` to change the scaling of an already scaled integer value:
  ```cpp
  // Converts a value scaled by 2^8 to a scaling of 2^4
  auto rescaledValue = fpm::s2s<8, 4, uint16_t>(256u);  // 16: uint16_t
  ```

- Using `fpm::scaled` to convert a real number to a scaled integer representation:
  ```cpp
  // Converts 25.5 to a scaled integer with a scaling factor of 2^10
  auto scaledValue = fpm::scaled<10, int16_t>(25.5);  // 26112: int16_t
  int scaledValueInt = fpm::scaled<10>(25.5);  // 26112: int (default type)
  ```

- Using `fpm::real` to convert a scaled integer value back to its real number representation:
  ```cpp
  // Converts the scaled value 26112 back to its real value, scaling f=10
  double realValue = fpm::real<10>(26112);  // 25.5: double (default type)
  auto realValueInt = fpm::real<10, int>(26112);  // 25: int
  ```

These examples illustrate the utility of `v2s`, `s2s`, `scaled`, and `real` in fixed-point calculations, enabling crucial scaling adjustments for maintaining precision in embedded systems programming. These functions simplify the conversion between real numbers and scaled representations, enhancing both accuracy and efficiency.

Furthermore, the library defines four compile-time-only static-assertion functions to ensure that `Q` types conform to expected specifications:

- `static_assert_base< std::integral T, class QSq >()`: This function static-asserts whether the `base_t` of `QSq` matches the specified type `T`.
- `static_assert_scale< scaling_t f, class QSq >()`: This function static-asserts that the scaling factor `f` of `QSq` is as specified.
- `static_assert_limits< double min, double max, class QSq >()`: This function static-asserts that the real value range of `QSq` falls within the specified `min` and `max` values.
- `static_assert_specs< std::integral T, scaling_t f, double min, double max, class QSq >()`: This comprehensive function static-asserts all critical properties - `base_t`, `f`, and value range - of `QSq`.

These assertions are especially useful for verifying the type of intermediate results after calculations with `Sq` types. If the core types of an application are changed, these assertions might trigger a compilation error, even if the formulas and calculations still compile. This ensures that certain type properties remain consistent throughout the development process, and any changes to the underlying type definitions are immediately flagged.

### Overflow Behaviors

The `fpm` library provides a robust handling of overflow scenarios through different behaviors that can be specified during the type definition of `Q`. Understanding and selecting the appropriate overflow behavior is critical to ensure that your application handles edge cases in a predictable and controlled manner. The library currently implements four types of overflow behaviors (from strictest to most lenient):

- **Error (`Ovf::error`)**: This behavior causes a compiler error if overflow is possible. It is the strictest behavior and the **default setting**, ensuring that potential overflow scenarios are addressed during development rather than at runtime.

- **Assert (`Ovf::assert`)**: This overflow behavior triggers a runtime assertion if overflow occurs, which is useful for debugging and development phases where catching errors immediately is crucial.

- **Clamp (`Ovf::clamp`)**: With this behavior, values that would normally overflow are clamped at runtime to the maximum or minimum value within the range defined for the `Q` type. This prevents overflow while still allowing the application to continue running.

- **Unchecked (`Ovf::unchecked`), Allowed (`Ovf::allowed`)**: This setting disables overflow checking entirely. It allows the value to wrap around according to the standard behavior of the underlying data type. "Unchecked" and "Allowed" refer to the same behavior, however, the choice of term may fit better semantically depending on the context or usage within specific parts of your application.

Choosing the right overflow behavior depends on your application’s requirements for safety, debugging, and performance.


## Q-Type

Just like the enigmatic **Q** from Star Trek, the `Q` type in the Fixed-Point Math Library possesses its own set of powerful capabilities, albeit more grounded in the realm of arithmetic precision than in galactic mischief. The primary purpose of our `Q` is to efficiently wrap around a scaled integer value that can be stored and modified at runtime, ensuring dynamic and precise arithmetic operations. Additionally, `Q` provides robust overflow protection and facilitates seamless conversions to and from other `Q` types.

### Implementation

`Q` is defined by the following template:

```cpp
namespace fpm {

template< std::integral base_t, fpm::scaling_t f, double realMin, double realMax,
          fpm::Overflow ovfBx = fpm::Ovf::error >
class Q {
    public:
    // ...

    private:
        base_t value;  ///< actual integer value stored in runtime memory
};

}
```

- `base_t`: Refers to the underlying integral type that holds the scaled value. Within the `Q` class, the scaled value, which represents the fixed-point number, is declared as a private member. This value is the only element that is stored in runtime memory for the `Q` instance.
- `realMin` and `realMax`: These specify the minimum and maximum values of the real value range that `Q` can represent, effectively setting the bounds for compile-time computation.
- `ovfBx`: Dictates the overflow behavior for operations that exceed the designated value range. The default overflow behavior is `Ovf::error`, which means the code will not compile if a potential overflow scenario is detected. This is particularly common when a conversion from a wider to a narrower real value range is attempted, safeguarding against inadvertent data loss or corruption.

These parameters are integral to the structure of each `Q` type and are set as static constexpr members, allowing them to be accessed directly for various computations or validations.

Additionally, a `Q` type provides the following static constexpr members to aid in precise and effective data manipulation:

- `scaledMin`: This member stores the minimum value that can be represented in the scaled format, calculated based on the *realMin* and the scaling factor.
- `scaledMax`: This member stores the maximum value that can be represented in the scaled format, calculated based on the *realMax* and the scaling factor.
- `resolution`: This represents the real resolution (double) of the `Q` type, defined as $2^{-f}$. It indicates the smallest difference between two representable real values within the type, providing clarity on the granularity and precision at which values can be manipulated or interpreted.

### Construction

The `Q` class can be instantiated using various integral types and scaling factors to fit specific application requirements. Here are some examples of how `Q` types can be defined:

- `fpm::Q<int8_t, 4, -10., 10.>` represents a `Q8.4` fixed-point type, covering a real value range from -10 to +10. The default overflow behavior is set to `Ovf::error`, meaning any operation leading to overflow will not compile.
- `fpm::Q<uint32_t, 12, 0., 1000., Ovf::clamp>` defines a `UQ32.12` fixed-point type with a real value range from 0 to 1000. Overflow behavior is set to `Ovf::clamp`, meaning values that exceed the range are adjusted to the nearest boundary.
- `fpm::Q<in16_t, 8>` defines a `Q16.8` fixed-point type that uses `Ovf::error` as the default overflow behavior and the largest possible **symmetric** value range. This symmetric range excludes `INT16_MIN` as scaled value to avoid issues such as overflow when taking the absolute value.

`Q` cannot be constructed directly from floating-point values to maintain type safety and prevent inadvertent data loss or misinterpretations at runtime. Instead, `Q` provides static constructor methods to create instances from real values, ensuring conversions are explicit and controlled:

- To construct a `Q` instance from a real constexpr value, you can use the static `fromReal< real >()` constructor function, which converts a floating-point number to the corresponding fixed-point representation. For example:
  ```cpp
  // Construct from real value
  auto a = fpm::Q<uint16_t, 8, 100., 200.>::fromReal< 155. >();
  ```
  The `auto` keyword is very handy here because it spares us from needing to specify the type twice, as the compiler deduces the type at compile time.
- Additionally, Q supports construction from scaled constexpr integer values through the static `fromScaled< scaled >()` constructor function, which takes an integer already scaled to the fixed-point format:
  ```cpp
  // Construct from scaled value
  auto b = fpm::Q<uint32_t, 8, 1000., 2000.>::fromScaled< 307200 >();
  auto c = fpm::Q<int32_t, 18, 100., 150.>::fromScaled< fpm::scaled<18>(120.) >();
  ```

- For creating instances of `Q` types with scaled integral runtime values, the constructor function  
`construct<ovfBxOvrd>( scaled )` is essential. It conducts a runtime overflow check as desired, adhering to the overflow behavior of the `Q` type, or, if specified, the behavior override provided via the template parameter `ovfBxOvrd`. This function ensures that values are instantiated safely, respecting the defined overflow strategies to prevent data corruption or unexpected behavior. It's important to note that this method can only be used with `scaled` integer values, as the runtime construction from double values is explicitly prohibited by the library's design to maintain type safety (no floats at runtime).
  ```cpp
  // Construct with runtime scaled value, clamped to target value range
  int32_t rtScaled = 123456;  // scaled value, q16; real=1.88379
  auto qValue = fpm::Q<int32_t, 16, -100., 100.>::construct<Ovf::clamp>(rtScaled);

  // Overflow override not necessary if target type inherently clamps
  auto qValue2 = fpm::Q<int32_t, 16, -100., 100., Ovf::clamp>::construct(rtScaled);
  ```

These construction methods provide precise control over how values are initialized in the `Q` type, ensuring adherence to the defined numerical ranges and overflow behaviors. Values are defined within the `< >` brackets at compile-time, and within the `( )` at runtime. It's important not to forget the `()` at the end of the static functions `Q<...>::fromReal<.>()` and `Q<...>::fromScaled<.>()`, as functions require parentheses to invoke them.

#### Type Aliases

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

#### More Sugar: User-Defined Literals

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

##### Provided Literals in fpm Library

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

### Value Access

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

### Rescaling

In situations where the base type remains the same but the fractional part `f` differs between two `Q` types, rescaling is necessary. This process is referred to as "up-scaling" when `f` increases, and "down-scaling" when `f` decreases.

- **Up-Scaling**: Up-scaling is achieved by **multiplying** the source value by $2^{to-from}$, effectively increasing the number of fractional bits to fit the higher resolution of the target type. While this process is technically lossless because no data is discarded, it doesn't enhance the actual resolution or detail of the original data; the additional fractional bits are essentially filled with zeros.

- **Down-Scaling**: Down-scaling reduces the number of fractional bits by **dividing** the source value by $2^{from-to}$. This operation is inherently lossy as it involves discarding the least significant bits of the fractional part. Consequently, the precision of the original value is reduced to match the lower resolution of the target type, resulting in a loss of detail that reflects the decreased fractional granularity.

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

#### Examples

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

/* narrower value range: fromQ<>() is required unless target type checks anyway */
// narrower_t c1 = source;  // error (narrower range)
auto c2 = narrower_t::fromQ<Ovf::clamp>( source );  // ok
auto c3 = narrower_t::fromQ<Ovf::clamp>( ovfSrc );  // ok
i32q8<-400., 1500., Ovf::clamp> c4 = ovfSrc;        // ok (clamping target)
i32q8<-400., 1500., Ovf::unchecked> c5 = ovfSrc;    // ok (check discarded)
```

### Casting

In the `Q` type framework, casting between different `Q` types with varying base types necessitates explicit type conversion. The simplest form of casting involves using `static_cast< QTo >( qFrom )`, which is straightforward when scaling adjustments are feasible and the target value range is either the same or wider than the source's, and if the target type's overflow behavior is the same or less strict.

```cpp
auto sourceValue = i32q10<0., 1500.>::fromReal< 1024. >();

// cast i32q10 to unsigned u32q12
auto cast1 = static_cast< u32q12<0., 1500.> >(sourceValue);

// cast i32q10 to i16q4, explicitly dropping any overflow check in target type
// (not recommended though, as target type will drop any overflow check this way)
auto cast2 = static_cast< i16q4<0., 1030., Ovf::unchecked> >(sourceValue);
```

However, if the target range is narrower and the target type incorporates overflow checks, or if the target type's overflow behavior is more restrictive than that of the source, `static_cast` may not be sufficient. In cases where overflow could occur, and unless the target type handles overflow with `Ovf::clamp` or `Ovf::assert` when constructed, a simple `static_cast` will not properly address potential overflow issues and thus not compile.

To manage potential overflow effectively, `Q` provides three specialized casting functions:

- `static_q_cast< QTo, ovfBxOvrd >( qFrom )`: This function applies the overflow handling as specified by the target type or uses the overridden overflow behavior if provided. It ensures that the conversion adheres to the designated overflow protocols, **performing runtime checks only when necessary** from a static point of view.

- `safe_q_cast< QTo, ovfBxOvrd >( qFrom )`: This function performs a runtime overflow check regardless of whether it is strictly necessary. It requires that the overflow behavior is set to a runtime check `Ovf::clamp` or `Ovf::assert`, and does not permit `Ovf::unchecked` and the compile-time check `Ovf::error`. This method is designed for scenarios where ensuring data integrity is critical, and overflow must be actively managed.

- `force_q_cast< QTo >( qFrom )`: This casting function performs no checks and simply takes the scaled value from the source type, constructing the specified `ToQ` type around it. This is essentially a forced value reinterpretation and should be used with caution, as it bypasses all overflow and range checks.

These casting functions provide robust tools for managing different fixed-point types, ensuring that conversions are both safe and efficient, depending on the operational requirements and data integrity needs.

```cpp
auto value = i32q10<0., 1500.>::fromReal< 1234. >();

// cast value to unsigned u32q12 (identical to static_cast)
auto cast1 = static_q_cast< u32q12<0., 1500.> >( value );  // 1234.

// cast value to i16q4 with narrower range, overflow check is dropped explicitly
// --> value is out of range!
auto cast2 = static_q_cast< i16q4<0., 1000.>, Ovf::unchecked >( value );  // 1234.

// static-cast cast2 (potentially out of range) to a wider range;
// note: no overflow checks are performed (wider range, identical ovf behavior)
auto cast3static = static_q_cast< i16q4<-100., 1100.> >( cast2 );  // 1234.

// safe-cast of cast2 is needed to really catch and clamp the out-of-range value
auto cast3safe = safe_q_cast< i16q4<-100., 1100.>, Ovf::clamp >( cast2 );  // 1100.

// force-cast cast2 (i16q4) into a u8q1
auto forced = force_q_cast< u8q1<0., 100.> >( cast2 );  // 16.
```

#### Range Clamping

The `Q` type framework includes a specialized feature known as "range clamping" to address the nuances of casting between differently signed types of different sizes. This feature ensures that conversions between signed and unsigned types adhere to intuitive expectations regarding value ranges:

- **From Signed to Unsigned**: When a `Q` type with a signed base type is cast to a `Q` type with an unsigned base type of the same or smaller size, any values in the negative range of the signed type are clamped to the lower limit of the unsigned type. This clamping is crucial to avoid underflow and to ensure values remain within the legitimate range of the unsigned type.
- **From Unsigned to Signed**: Similarly, when a `Q` type with an unsigned base type is cast to a `Q` type with a signed base type of the same or smaller size, values that fall in the upper half of the unsigned range are clamped to the upper limit of the signed type. This measure prevents potential overflow, ensuring that the values do not exceed the maximum positive value of the signed type.
These adjustments are driven by internal logic designed to mirror what would typically be expected from the real values during such type conversions. By incorporating this feature, the framework helps preserve data integrity and provides a logical, safe transition between signed and unsigned `Q` types, reflecting the expected behavior of real-world values.

TODO: examples
