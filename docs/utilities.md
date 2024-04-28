# Utilities

One of the foundational declarations is the definition of `scaling_t` as an alias for `int`. The decision to make `scaling_t` signed is strategic because the framework needs to support both positive and negative fractional values:

- **Positive Scaling Values**: These are used to represent fractional parts, where positive scaling factors enhance the precision of the fractional component by specifying how many bits are dedicated to values below the integer point.
- **Negative Scaling Values**: Negative scaling is used to represent larger numbers than those typically representable with the given bits of the used integral base type. By scaling the real number by \(2^f\), where \(f\) is negative, large numbers can be represented at the cost of reducing the precision of the integral part of the number by \(2^{|f|}\), effectively balancing the range and precision according to specific needs.

This flexibility ensures that the framework can adapt to a variety of numerical ranges and precision requirements, making it highly versatile for different applications.

## Helper Functions

The library includes a set of helper functions designed to facilitate the manipulation of values at compile-time, ensuring that they are correctly scaled for use with different Q types. These functions are critical for creating and converting scaled values accurately and efficiently.

- **v2s (Value to Scale)**: This function is used to convert a real floating-point value into a scaled integer value at compile time. It multiplies the given `ValueT` value by \(2^{to}\), allowing for precise initialization of fixed-point variables from real numbers within the resolution of `TargetT`.
- **s2s (Scale to Scale)**: This function changes the scaling of a given scaled integer value. It multiplies the value by \(2^{to-from}\), enabling the conversion of a value from one fixed-point scale to another without losing precision relative to the scaling factor.
- **scaled**: Converts a given real value (double) to its scaled representation, using the specified scaling factor `f`. This function is effectively an alias for `v2s<f, base_t>(real)`, streamlining the process of initializing `Q` types with precise scaled values derived from real numbers.
- **real**: Converts a scaled integral value back to its real (double) representation using the given scaling factor `f`. This function serves as an alias for `s2s<f, 0, double>(scaled)`, facilitating the conversion of scaled values into their real, human-readable forms, particularly useful for outputs and debugging.

The functions are declared as follows:

```cpp
namespace fpm {

template< scaling_t to, typename TargetT, typename ValueT >
consteval TargetT v2s(ValueT value) noexcept { /* ... */ }

template< scaling_t from, scaling_t to, typename TargetT, typename ValueT >
constexpr TargetT s2s(ValueT value) noexcept { /* ... */ }

template< scaling_t f, std::integral TargetT = int >
consteval TargetT scaled(double real) noexcept { /* ... */ }

template< scaling_t f, typename TargetT = double >
constexpr TargetT real(std::integral auto scaled) noexcept { /* ... */ }

}
```

Here are practical examples demonstrating how to use the helper functions provided by the library:

- Using `fpm::v2s` to convert a floating-point number to a scaled integer value at compile-time:
  ```cpp
  // Converts 10.5 to a scaled integer using a scale factor of 2^3
  auto scaledValue = fpm::v2s<3, int8_t>(10.5);  // 84: int8_t
  ```

- Using `fpm::s2s` to change the scaling of an already scaled integer value:
  ```cpp
  // Converts a value scaled by 2^8 to a scaling of 2^4
  auto rescaledValue = fpm::s2s<8, 4, uint16_t>(256u);  // 16: uint16_t
  ```

- Using `fpm::scaled` to convert a real number to a scaled integer representation at compile-time:
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

## Static Assertion

The library defines four compile-time-only static assertion functions to ensure that `Q` or `Sq` types conform to expected specifications:

- **static_assert_base**: This function statically asserts whether the `base_t` of a given `Q` or `Sq` type matches the specified base type `Base`.
  ```cpp
  // definition in fpm:
  template< std::integral Base, class QSq >
  consteval void static_assert_base() noexcept { /* ... */ }

  // use as follows:
  using someType = u8q4<1., 10.>;
  auto someVariable = someType::fromReal< 5.5 >();

  fpm::static_assert_base< uint8_t, someType >();
  fpm::static_assert_base< uint8_t, decltype(someVariable) >();
  ```
- **static_assert_scale**: This function statically asserts that the scaling factor `f` of a given `Q` or `Sq` type is as specified.
  ```cpp
  // definition in fpm:
  template< scaling_t f, class QSq >
  consteval void static_assert_scale() noexcept { /* ... */ }

  // use as follows:
  using someType = u8q4<1., 10.>;
  auto someVariable = someType::fromReal< 5.5 >();

  fpm::static_assert_scale< 4, someType >();
  fpm::static_assert_scale< 4, decltype(someVariable) >();
  ```
- **static_assert_limits**: This function statically asserts that the real value range of a given `Q` or `Sq` type falls within the specified `min` and `max` values.
  ```cpp
  // definition in fpm:
  template< double min, double max, class QSq >
  consteval void static_assert_limits() noexcept { /* ... */ }

  // use as follows:
  using someType = u8q4<1., 10.>;
  auto someVariable = someType::fromReal< 5.5 >();

  fpm::static_assert_limits< 1., 10., someType >();
  fpm::static_assert_limits< 1., 10., decltype(someVariable) >();
  ```
- **static_assert_specs**: This comprehensive function statically asserts all critical properties (`base_t`, `f`, `min` and `max`) of a given `Q` or `Sq` type.
  ```cpp
  // definition in fpm:
  template< std::integral Base, scaling_t f, double min, double max, class QSq >
  consteval void static_assert_specs() noexcept { /* ... */ }

  // use as follows:
  using someType = u8q4<1., 10.>;
  auto someVariable = someType::fromReal< 5.5 >();

  fpm::static_assert_specs< uint8_t, 4, 1., 10., someType >();
  fpm::static_assert_specs< uint8_t, 4, 1., 10., decltype(someVariable) >();
  ```

These assertions are especially useful for verifying the type of intermediate results after calculations with `Sq` types. If the core types of an application are changed, these assertions might trigger a compilation error, even if the formulas and calculations still compile. This ensures that certain type properties remain consistent throughout the development process, and any changes to the underlying type definitions are immediately flagged.
