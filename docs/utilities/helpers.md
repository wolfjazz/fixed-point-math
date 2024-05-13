# Helper Functions

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
