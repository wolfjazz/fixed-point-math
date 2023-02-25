# Fixed-Point Math

Yep, another fixed-point library. The idea is to have something that can be used in embedded C++ systems that compiles down to more or less plain integer math operations, however **with compile-time and well placed (low-cost) runtime checks that make sure that the integers do not exceed a specified value range**.

## Motivation

In embedded systems programmed with C/C++ it's quite common to avoid the built-in floating-point types `float` and `double`, as they have a high negative impact on code size and execution time, which are both precious goods in micro-controllers. An established way around the use of floating-point values is to use integers scaled with some *fixed* power of two factor so that decimal places can be expressed as well.  
For example, if a 32-bit unsigned integer is scaled with the factor 2<sup>16</sup>, there are 16 bits before the comma to express integers (0-65535) and 16 remaining bits after the comma to express fractions (with a precision of 2<sup>-16</sup>). This is an example of the very common UQ16.16 `fixed-point` type in ARM's **Q notation** that can represent numbers from *0* to *2<sup>16</sup> - 2<sup>-16</sup> = 65535.99998474*. It is called fixed-point type because the comma is fixed. Of course, the comma can be at any desired place for different fixed-point types: UQ0.32, UQ5.27 and UQ31.1 also describe fixed-point types with an unsigned 32-bit integer as base type.  
Negative numbers are usually expressed as a signed integer in two's complement representation. For instance, Q14.2 has a signed 16-bit integer base type with 1 sign bit, 13 integer bits and 2 fraction bits to represent values between *-2<sup>13</sup> = -8192.00* and *+2<sup>13</sup>-2<sup>-2</sup> = 8191.75*.

### Problems and Expectations

What sounds reasonable so far is actually one of the most underestimated sources for bugs:
- Mathematical operations like multiplications and especially more complex formulas tend to exceed the value range of the underlying base type quite fast. When no overflow check is performed and the values are not saturated, the integers will quietly overflow and the calculations will return completely wrong values with unpredictable consequences.
- Calculations with scaled values are quite complicated when done manually because scaling corrections are needed for many operators. As a result complex formulas are often hard to read and correcting factors can easily be forgotten.

In the end, we just want to perform calculations with a predefined value range and precision without having to worry about things like overflow too much. So let's define a list of expectations what a fixed-point library should provide:

- predefined fixed-point types based on signed and unsigned integer types (8, 16, 32 and 64 bits)
- user-defined precision and value range (at compile-time)
- ability to specify the value range via floats (= unscaled values) or scaled integers
- ability to change the precision and/or the value range later in code (only at compile-time)
- implementation of the most-common mathematical operators (+, -, *, /, %, ^, sqr, sqrt, <<, >>)
- simple, easy-to-debug, on-point formulas without any obscuring scaling corrections
- implicit and explicit conversion between fixed-point types (implicit: to higher precision)
- no implicit construction from any integer or floating-point types (safety, to avoid confusion)
- explicit construction from static integers at compile time
- explicit construction from integer-based variables at runtime
- explicit construction of static floats at compile time
- no runtime construction from floats (because we don't want floats at runtime)
- different types of overflow checks (compile-time, runtime::saturation, runtime::assertion)

## This Library

Provides different fixed-point types which fulfill the expectations from above (more or less).

### Some Tests (will be replaced with proper sections in this document)

````C++
/* normal fixed-point type in Q notation with value saturation or assertion at runtime */
fpm::q<type, n, v_min, v_max>;
// or use 'fixed' instead of 'q'?
fpm::fixed<type, n, v_min, v_max>;

// predefined
using q32<...> = fpm::q<int32_t, ...>;
using qu32<...> = fpm::q<uint32_t, ...>;
using q16<...> = fpm::q<int16_t, ...>;
using qu16<...> = fpm::q<uint16_t, ...>;
// ...

// user-defined
using q32n16<...> = q32<16, ...>;  // resolution 2^-16
using qu32n20<...> = qu32<20, ...>;  // resolution 2^-20

// formulas



/* static Q-type for static calculations that can be used to guarantee at compile time that
 * a formula works for a range of input values */
fpm::sq<...>

// ...
````


## Badges
On some READMEs, you may see small images that convey metadata, such as whether or not all the tests are passing for the project. You can use Shields to add some to your README. Many services also have instructions for adding a badge.

## Visuals
Depending on what you are making, it can be a good idea to include screenshots or even a video (you'll frequently see GIFs rather than actual videos). Tools like ttygif can help, but check out Asciinema for a more sophisticated method.

## Installation
Within a particular ecosystem, there may be a common way of installing things, such as using Yarn, NuGet, or Homebrew. However, consider the possibility that whoever is reading your README is a novice and would like more guidance. Listing specific steps helps remove ambiguity and gets people to using your project as quickly as possible. If it only runs in a specific context like a particular programming language version or operating system or has dependencies that have to be installed manually, also add a Requirements subsection.

## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## Support
Tell people where they can go to for help. It can be any combination of an issue tracker, a chat room, an email address, etc.

## Roadmap
If you have ideas for releases in the future, it is a good idea to list them in the README.

## Contributing
State if you are open to contributions and what your requirements are for accepting them.

For people who want to make changes to your project, it's helpful to have some documentation on how to get started. Perhaps there is a script that they should run or some environment variables that they need to set. Make these steps explicit. These instructions could also be useful to your future self.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## License
For open source projects, say how it is licensed.

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.
