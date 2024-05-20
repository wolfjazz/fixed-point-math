# Fixed-Point Math Library

## Overview

This Fixed-Point Math Library is designed specifically for embedded systems that operate in C++ environments where the overhead of floating-point calculations is prohibitive. The library employs scaled integers to perform precise and efficient arithmetic operations, crucial in contexts where resources such as code size and execution time are limited.

## Features

- **Type Definitions**: Provides fixed-point types defined through template parameters specifying integer and fractional parts.
- **Precision and Range Definition**: Allows compile-time configuration of precision and value ranges to suit different application needs.
- **Overflow Protection**: Includes both compile-time assertions and runtime checks to prevent overflow, with strategies like clamping and assertions to handle exceptional cases.
- **Mathematical Operations**: Supports fundamental arithmetic operations (+, -, *, /, %) and complex functions (pow, sqrt, etc.), implemented using integer arithmetic to avoid floating-point computation.
- **Type Safety and Conversion**: Facilitates safe conversions between different fixed-point types and provides explicit casting methods to manage precision adjustments and prevent data loss.

## Usage

The library provides a robust framework for implementing fixed-point arithmetic in embedded applications. It allows developers to handle mathematical calculations efficiently without the need for floating-point units, reducing both the code footprint and execution time.

### Example

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

## Generate Documentation

Install `mkdocs` and run `mkdocs serve`. If successful, the generated documentation can then be accessed via `http://127.0.0.1:8000/`.

## Contributions
Contributions are welcome! Whether it's adding new features, improving existing ones, or fixing bugs, your input is valuable. Please submit your pull requests or open issues on GitLab to discuss potential changes or additions.

## License
This library is released under the Apache License 2.0, allowing it to be used freely in both open-source and proprietary software. However, any modifications made to the original code must be clearly documented, and all copies or substantial portions of the library must retain the original copyright notice, license, and any accompanying notices.
