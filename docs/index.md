# Fixed-Point Math

This C++20 library introduces a fixed-point mathematical system designed specifically for embedded C++ systems. It simplifies integer math operations and incorporates compile-time and carefully optimized runtime checks to ensure that integers do not exceed predefined value ranges.

## Motivation

In embedded systems programmed with C/C++, it is typical to avoid built-in floating-point types like `float` and `double` due to their significant impact on code size and execution time—critical factors in microcontrollers. A common solution is to use integers scaled by a fixed power of two, allowing representation of fractional values. For instance, a 32-bit unsigned integer scaled by the factor \(2^{16}\) splits the integer into 16 bits for the whole numbers (0-65535) and 16 bits for the fractional part, offering a precision of \(2^{-16}\)
. This `UQ16.16` fixed-point type, prevalent in ARM's Q notation, can represent values from `0.0` to `65535.99998474`. Negative values are typically handled using signed integers in two's complement form, like the `Q14.2` type, which spans from `-8192.00` to `8191.75`.

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

## Example

Here's a simple example of using the library:

```cpp
#include <fpm.hpp>
using namespace fpm::types;

int main() {
    using i32q12_t = i32q12<-100.,  200.>;  // int32_t, q12
    using i32q14_t = i32q14<-100.,  200.>;  // int32_t, q14
    using u32q11_t = u32q11<   0., 1111.>;  // uint32_t, q11

    // Type resolutions
    // i32q12_t::res: 0.000244141, i32q14_t::res: 6.10352e-05,
    // u32q11_t::res: 0.000488281
    std::cout << "i32q12_t::res: " << i32q12_t::resolution << ", "
              << "i32q14_t::res: " << i32q14_t::resolution << ", "
              << "u32q11_t::res: " << u32q11_t::resolution << std::endl;

    auto a = i32q12_t::fromReal<105.45>();  // scaled to 431923
    i32q14_t b = 123.456_i32q14;  // via literal; scaled to 2022703
    i32q14_t a2 = a;  // copy-upscale from q12 to q14; scaled to 1727692

    /* do some math: math results have static-Q (Sq) types */

    // addition
    auto c = a + b;  // i32sq14<-200.,400.>; scaled: 3750395, real: 228.906

    // clamp value to narrower type
    auto d = i32q12_t::fromSq<Ovf::clamp>(c);  // scaled: 819200, real: 200.0

    // cast to unsigned type
    auto s = static_q_cast<u32q11_t, Ovf::unchecked>(d);  // sc: 409600, re: 200

    // multiplication
    auto p = a2 * d;  // i32sq14<-20000.,40000.>; scaled: 345538400, real: 21090

    // Sum: 200, Product: 21090
    std::cout << "Sum: " << s.real() << ", Product: " << p.real() << std::endl;
    // Sum[u32q11]: 409600, Product[i32sq14]: 345538400
    std::cout << "Sum[u32q11]: " << s.scaled() << ", "
              << "Product[i32sq14]: " << p.scaled() << std::endl;

    return 0;
}
```

For a more complex example of a computation, please refer to [Practical Example](arithmetics/practical.md).