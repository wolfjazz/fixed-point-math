# Fixed-Point Math

This library introduces a fixed-point mathematical system designed specifically for embedded C++ systems. It simplifies integer math operations and incorporates compile-time and carefully optimized runtime checks to ensure that integers do not exceed predefined value ranges.

## Motivation

In embedded systems programmed with C/C++, it is typical to avoid built-in floating-point types like `float` and `double` due to their significant impact on code size and execution time—critical factors in microcontrollers. A common solution is to use integers scaled by a fixed power of two, allowing representation of fractional values. For instance, a 32-bit unsigned integer scaled by the factor 2^16 splits the integer into 16 bits for the whole numbers (0-65535) and 16 bits for the fractional part, offering a precision of 2^-16. This `UQ16.16` fixed-point type, prevalent in ARM's Q notation, can represent values from 0 to 65535.99998474. Negative values are typically handled using signed integers in two's complement form, like the `Q14.2` type, which spans from -8192.00 to 8191.75.

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
- Implicit conversions between types of the same base type only if it results in higher precision; explicit conversion for downscaling or changing precision.
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

### Helper functions

The library includes a set of helper functions designed to facilitate the manipulation of values at compile-time, ensuring that they are correctly scaled for use with different Q types. These functions are critical for creating and converting scaled values accurately and efficiently.

- **Value to Scale (`v2s`)**: This function is used to convert a real floating-point value into a scaled integer value at compile time. It multiplies the given `ValueT` value by 2^to, allowing for precise initialization of fixed-point variables from real numbers within the resolution of `TargetT`.
- **Scale to Scale (`s2s`)**: This function changes the scaling of a given scaled integer value. It multiplies the value by 2^(to-from), enabling the conversion of a value from one fixed-point scale to another without losing precision relative to the scaling factor.

```cpp
namespace fpm {

template<typename TargetT, fpm::scaling_t to, typename ValueT>
constexpr
TargetT v2s(ValueT value) noexcept { /* ... */ }

template<typename TargetT, fpm::scaling_t from, fpm::scaling_t to, typename ValueT>
constexpr
TargetT s2s(ValueT value) noexcept { /* ... */ }

}
```

Here are practical examples demonstrating how to use the helper functions `v2s` and `s2s` provided by the library:

- Using `v2s` to convert a floating-point number to a scaled integer value:
  ```cpp
  // Converts 10.5 to a scaled integer using a scale factor of 2^3
  auto scaledValue = fpm::v2s<int8_t, 3>(10.5);  // 84: int8_t
  ```

- Using `s2s` to change the scaling of an already scaled integer value:
  ```cpp
  // Converts a value scaled by 2^8 to a scaling of 2^4
  auto rescaledValue = fpm::s2s<uint16_t, 8, 4>(256u);  // 16: uint16_t
  ```

These examples illustrate the utility of `v2s` and `s2s` in fixed-point calculations, allowing for compile-time and runtime scaling adjustments that are crucial for maintaining precision in embedded systems programming.

### Overflow Behaviors

The `fpm` library provides a robust handling of overflow scenarios through different behaviors that can be specified during the type definition of `Q`. Understanding and selecting the appropriate overflow behavior is critical to ensure that your application handles edge cases in a predictable and controlled manner. The library currently implements four types of overflow behaviors:

- **Error (`Ovf::error`)**: This behavior causes a compiler error if overflow is possible. It is the **default setting**, ensuring that potential overflow scenarios are addressed during development rather than at runtime.

- **Assert (`Ovf::assert`)**: This overflow behavior triggers a runtime assertion if overflow occurs, which is useful for debugging and development phases where catching errors immediately is crucial.

- **Clamp (`Ovf::clamp`)**: With this behavior, values that would normally overflow are clamped at runtime to the maximum or minimum value within the range defined for the `Q` type. This prevents overflow while still allowing the application to continue running.

- **Unchecked (`Ovf::unchecked`), Allowed (`Ovf::allowed`)**: This setting disables overflow checking entirely. It allows the value to wrap around according to the standard behavior of the underlying data type. "Unchecked" and "Allowed" refer to the same behavior, however, the choice of term may fit better semantically depending on the context or usage within specific parts of your application.

Choosing the right overflow behavior depends on your application’s requirements for safety, debugging, and performance.


## Q-Type

Just like the enigmatic Q from Star Trek, the `Q` type in the Fixed-Point Math Library possesses its own set of powerful capabilities, albeit more grounded in the realm of arithmetic precision than in galactic mischief. The main purpose of our `Q` is to provide robust overflow protection and checks, along with facilitating conversions to and from other `Q` types. It stores a scaled integer value that can be stored and modified at runtime.

### Implementation

`Q` is defined by the following template:

```cpp
template< std::integral base_t, fpm::scaling_t f, double realMin, double realMax,
          fpm::Overflow ovfBx = fpm::Ovf::error >
class Q {
    public:
    // ...

    private:
        base_t value;  ///< actual integer value stored in runtime memory
};
```

- `base_t`: Refers to the underlying integral type that holds the scaled value. Within the `Q` class, the scaled value, which represents the fixed-point number, is declared as a private member. This value is the only element that is stored in runtime memory for the `Q` instance.
- `realMin` and `realMax`: These specify the minimum and maximum values of the real value range that `Q` can represent, effectively setting the bounds for compile-time computation.
- `ovfBx`: Dictates the overflow behavior for operations that exceed the designated value range. The default overflow behavior is `Ovf::error`, which means the code will not compile if a potential overflow scenario is detected. This is particularly common when a conversion from a wider to a narrower real value range is attempted, safeguarding against inadvertent data loss or corruption.

### Construction

The `Q` class can be instantiated using various integral types and scaling factors to fit specific application requirements. Here are some examples of how `Q` types can be defined:

- `Q<int8_t, 4, -10., 10.>` represents a `Q8.4` fixed-point type, covering a real value range from -10 to +10. The default overflow behavior is set to `Ovf::error`, meaning any operation leading to overflow will not compile.
- `Q<uint32_t, 12, 0., 1000., Ovf::clamp>` defines a `UQ32.12` fixed-point type with a real value range from 0 to 1000. Overflow behavior is set to `Ovf::clamp`, meaning values that exceed the range are adjusted to the nearest boundary.
- `Q<in16_t, 8>` defines a `Q16.8` fixed-point type that uses `Ovf::error` as the default overflow behavior and the largest possible **symmetric** value range. This symmetric range excludes `INT16_MIN` as scaled value to avoid issues such as overflow when taking the absolute value.

`Q` cannot be constructed directly from floating-point values to maintain type safety and prevent inadvertent data loss or misinterpretations at runtime. Instead, `Q` provides static constructor methods to create instances from real values, ensuring conversions are explicit and controlled:

- To construct a `Q` instance from a real value, you can use the static `fromReal<.>()` constructor function, which converts a floating-point number to the corresponding fixed-point representation. For example:
  ```cpp
  // Construct from real value
  auto a = Q<uint16_t, 8, 100., 200.>::fromReal< 155. >();
  ```
  The `auto` keyword is very handy here because it spares us from needing to specify the type twice, as the compiler deduces the type at compile time.
- Additionally, Q supports construction from scaled integer values through the static `fromScaled<.>()` constructor function, which takes an integer already scaled to the fixed-point format:
  ```cpp
  // Construct from scaled value
  auto b = Q<uint32_t, 8, 1000., 2000.>::fromScaled< 307200 >();
  auto c = Q<int32_t, 18, 1000., 1500.>::fromScaled< fpm::v2s<int32_t, 18>(1200.) >();
  ```

These construction methods provide precise control over how values are initialized in the Q type, ensuring adherence to the defined numerical ranges and overflow behaviors. Note that the values are specified at compile-time between the `<>` characters. The `()` at the end should not be forgotten because `Q<...>::fromReal<.>()` and `Q<...>::fromScaled<.>()` are static functions.

#### Type Aliases

The library provides alias templates as syntax sugar for common Q types, looking similarly to standard integer types, in the namespace `fpm::types`. This simplifies the usage and readability of the types in your code. For instance:

- `i32q16<realMin, realMax, Ovf::clamp>` is a `Q<int32_t, 16, realMin, realMax, Ovf::clamp>`
- `u16q8<realMin, realMax>` is a `Q<uint16_t, 8, realMin, realMax>`
- `i32qm7<realMin, realMax>` is a `Q<int32_t, -7, realMin, realMax`
- `i16q6<>` is a `Q<int16_t, 6>`, with the largest possible symmetric value range and `Ovf::error`

Construction of `Q` instances using these aliases looks as follows:
```cpp
using namespace fpm::types;

// user-defined type aliases
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

However, it is often practical for certain literals to be used across the entire application. To accommodate this, such literals should be defined in a core header file. This centralized approach allows for the universal application of the literal across multiple files, ensuring consistent usage and behavior throughout the application.

##### Provided Literals in fpm Library

The `fpm` library automatically provides literals for all fixed-point types defined in the `fpm::types` namespace, ensuring a consistent and easy way to work with these types directly in your code. Each type has a corresponding literal suffix that mirrors its type declaration, enhancing code readability and precision.

An important characteristic of these literals is that the value range specified by the literal is only the value itself, which provides flexibility and avoids unnecessary limitation of calculations.

Here are examples of how these literals can be utilized:

```cpp
#include <fpm.hpp>
using namespace fpm::types;

i32q8<-1000., 1000.> distance = -150.0_i32q8;
u16q4<0., 2000.> pressure = 1023_u16q4;
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
  For accessing the real value in any integer format, use `real<typename>()`. This method converts the unscaled real value to the given integral type by truncating it according to C++ rules, discarding any fractional digits. This truncation must be handled with care to avoid unintended data loss, especially when the type has a large fraction part.

  ```cpp
  // Get the real value as an integer, truncating fractional digits
  int intRealVal = qValue.real<int>();
  ```

### Rescaling

TODO

### Casting

TODO
