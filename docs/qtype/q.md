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

## Type Examples

Here are some examples of how `Q` types can be defined:

- `fpm::Q<int8_t, 4, -10., 10.>` represents a `Q8.4` fixed-point type, covering a real value range from -10 to +10. The default overflow behavior is set to `Ovf::error`, meaning any operation leading to overflow will not compile.
- `fpm::Q<uint32_t, 12, 0., 1000., Ovf::clamp>` defines a `UQ32.12` fixed-point type with a real value range from 0 to 1000. Overflow behavior is set to `Ovf::clamp`, meaning values that exceed the range are adjusted to the nearest boundary.
- `fpm::Q<in16_t, 8>` defines a `Q16.8` fixed-point type that uses `Ovf::error` as the default overflow behavior and the largest possible **symmetric** value range. This symmetric range excludes `INT16_MIN` as scaled value to avoid issues such as overflow when taking the absolute value.

### Type Aliases

The library provides alias templates as syntax sugar for common `Q` types, looking similarly to standard integer types, in the namespace `fpm::types`. This simplifies the usage and readability of the types in your code. For instance:

- **`i32q16< realMin, realMax, Ovf::clamp >`** is a `Q<int32_t, 16, realMin, realMax, Ovf::clamp>`
- **`u16q8< realMin, realMax >`** is a `Q<uint16_t, 8, realMin, realMax>`
- **`i32qm7< realMin, realMax >`** is a `Q<int32_t, -7, realMin, realMax`
- **`i16q6<>`** is a `Q<int16_t, 6>`, with the largest possible symmetric value range and `Ovf::error`

### Literals for Q Type Aliases

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

#### User Literals

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
