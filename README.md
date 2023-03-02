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
- ability to specify the value range via floats (= unscaled values)
- ability to change the precision and/or the value range later in code (only at compile-time)
- implementation of the most-common mathematical operators (+, -, *, /, %, pow, sqr, sqrt, <<, >>, ++, --, <, >) and the corresponding assignment-operator constructs (+=, -=, *=, /=, %=, <<=, >>=)
- simple, easy-to-debug, on-point formulas without any obscuring scaling corrections
- no implicit construction from any integer or floating-point types (safety, to avoid confusion)
- explicit construction from static integers at compile time
- explicit construction from integer-based variables at runtime
- explicit construction of static floats at compile time
- no runtime construction from floats (because we don't want floats at runtime)
- compile-time overflow checks where possible
- different types of runtime overflow checks (runtime::saturation, runtime::assertion, runtime::forbidden)
- implicit conversion between fixed-point types of same base type to higher precision
- explicit conversion to different base type or lower precision -> casting (static, safe, use-as)

## This Library

Provides different fixed-point types which fulfill the expectations from above (more or less).

### Some Ideas (will be replaced with proper sections in this document when ready)

````C++
/*
 * normal fixed-point type in Q notation with value saturation or assertion at runtime
 */

// base-type, number of fraction bits n, check type, minimum value, maximum value;
// note: only scaled integer value (mem-value) is stored in memory; rest (e.g. value range) is
//       compile-time-only!
fpm::q<type, n, check, v_min, v_max>;
// or use 'fixed' instead of 'q'?
// fpm::fixed<type, n, v_min, v_max>;  --> no

// predefined types
using q32<...> = fpm::q<int32_t, ...>;
using qu32<...> = fpm::q<uint32_t, ...>;
using q16<...> = fpm::q<int16_t, ...>;
using qu16<...> = fpm::q<uint16_t, ...>;
// ...

// user-defined types
using qu32n16<...> = qu32<16, fpm::check::SATURATE, ...>;  // res. 2^-16; overflow protection: saturation
using q32n16<...> = q32<16, fpm::check::ASSERT, ...>;  // res. 2^-16; overflow protection: assertion
// res. 2^-20; checks at runtime forbidden -> code does not compile if check would be needed
using qu32n20<...> = qu32<20, fpm::check::FORBIDDEN, ...>;
using q16n2<...> = q16<2, fpm::check::SATURATE, ...>;  // res. 2^-2; saturation


/* declaration and initialization */
qu32n16<> a0(99.9);  // direct initialization; default value range is full possible range
auto a = qu32n16<>(45678.123);  // construction
auto b = qu32n16<45.0, 98.2>(66.);  // construction; value range 45.0-98.2 (2949120-6435635);
// value range specified via scaled integer is not useful because if the value of n is changed
// all ranges need to be adapted when scaled values are used; this is not needed for real values
// and lets be honest - this is not intuitive either.
//auto c = qu32n16<1966080, 3932160>(45.1);

// copy: construct from another q value with same base-type; value range can be changed this way;
// note that copy will perform a range check at runtime when the lhs range is smaller than the rhs range
qu32n16<> d1 = b;
qu32n16<40000.0, 50000.0> d2 = a;  // limitation of value range; will perform range check at runtime
// upscale-copy: mem-value increased by 2^4 and checked at runtime; value range implicitly reduced
qu32n20<> e = a;
auto e2 = qu32n20<>(a);
qu32n20<> e3 = qu32n16<>(1.1);  // construct temporary q16 and upscale-move to q20 lvalue
// downscale-copy: mem-value decreased at runtime without checks; value range implicitly extended
qu32n16<> f = e;
auto f2 = qu32n16<>(e);


/* assignment operator */
f = a;  // assigns value of a to f; performs runtime checks when lhs range is smaller than rhs range


/* casting */
// explicit cast from signed to unsigned or vice versa (e.g. u32 to i32);
// explicit cast to different base type size (e.g. i32 to i16) and/or different precision;
// whether or not runtime checks are performed, depends on the type of the cast:
auto cast1 = static_cast<q16n2<>>(b);  // Performs checks if needed (decided at compile-time).
auto cast1b = static_q_cast<q16n2<>>(b);  // Same as static_cast; Provided for consistency.
auto cast2 = safe_q_cast<q16n2<>>(a);  // Safe cast will always perform checks.
// Use-as cast will never perform checks! Value is simply reused (reinterpreted). Can overflow!
// (E.g. useful if an overflow is required as part of an algorithm.)
auto cast3 = use_as_q_cast<q16n2<40., 100.>>(b);


/* addition */
qu32n16<> g = a + b;
qu32n16<0., 45700.> h = a + b;  // runtime check error: out of range (beyond upper limit)
qu32n20<> i = a + e;  // addition performed in q20 (higher precision of e) and stored as q20 (i)
qu32n16<> j = a + e;  // addition performed in q20 (higher precision of e) and stored as q16 (j)

// no range check performed when R1 * R2 (ranges Ri, * is an operator) cannot go out-of-range
auto x = qu32n16<40., 80.>(50.);
auto y = qu32n16<10., 20.>(15.);
qu32n16<> z = x + y;  // for example, no range check performed here because R1 + R2 cannot go out-of-range


/* subtraction */
/* multiplication */
/* division */
// similar to addition


/* static Q-type for static formulas that can be used to guarantee at compile time that a calculation
 * works for a range of input values. Runtime checks are only needed when a q value is transformed
 * into an sq value and vice versa. sq-only formulas are guaranteed to be safe at runtime, because
 * for each operator the value range is modified and checked against overflow at compile-time.
 * Runtime checks are not included as long as only sq values are used in the formula. This guarantees
 * that the formula compiles into an efficient calculation.
 * Note: By design, sq values cannot be changed. For each operator a new sq value is constructed. */
fpm::sq<type, n, v_min, v_max>;

// predefined types
using sq32<...> = fpm::sq<int32_t, ...>;
using squ32<...> = fpm::sq<uint32_t, ...>;
using sq16<...> = fpm::sq<int16_t, ...>;
using squ16<...> = fpm::sq<uint16_t, ...>;
// ...

// user-defined types
// from above: for q-types, corresponding sq types are created; can be accessed via q<>::sq<>;
//             default value range of these sq types is that of the related q types;
//             can be changed to a different (smaller) range of values!


// ...
// some given q values, e.g. speed [mm/s] and acceleration [mm/s^2]
using speed_t = q32n16<-100., 100.>;
using accel_t = q32n16<-10., 10.>;
using pos_t = q32n16<-10000., 10000.>;
auto speed0 = speed_t(50.);
auto accel = accel_t(5.);
auto pos0 = pos_t(1000.);
// ...

// sq-calculation, performed safely via sq values. As mentioned above, if only sq values are used
// in a formula, this whole calculation will not include any overflow checks at runtime, because the
// compiler is doing these checks for the value ranges at compile-time and the code does not compile
// when the calculated value range does not fit the user-defined, expected value range.
// Note: Transitions from q to sq space and vice versa are explicit by design (assignment does not work)!
speed_t::sq<> sV0 = speed0.as_sq();  // construct sq value from corresponding q value; checks needed?
accel_t::sq<> sAccel = accel.as_sq();  // note: default value range of sq type here is that of q type
pos_t::sq<-5000., 5000.> sS0 = pos0.as_sq();  // sq value for pos0 with a smaller value range; performs checks
auto sTime = squ16n8<0., 10.>(4.);  // current time [s]
// calculation; perform for a range of input values; expect a range of output values (can and should
// be calculated with the real decimal range values given when the types are defined;
// for example, 10*10*10/2 + 100*10 + 10000 = 11500 is the upper limit of the output range);
// no checks are performed here; only the given operations and the implicit precision and scaling corrections
// one would need to perform explicitly when doing these calculations manually with scaled integers;
// s = a/2*t^2 + v0*t + s0
pos_t::sq<-6500., 6500.> s = sAccel*sTime*sTime/2 + sV0*time + sS0;  // TODO: handle /2 how?
// another calculation step; note that a new variable needs to be defined because s cannot be changed
// since it is a sq type.
pos_t::sq<-6500., 7000.> s2 = s + pos_t::sq<0., 500.>(250.);  // add some constant value from a range
// now update position in q scaling;
// performs no check when value of s is assigned to pos0 this way because of smaller value range
pos = s2.as_q();

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
