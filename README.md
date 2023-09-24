# Fixed-Point Math

Yep, another fixed-point library. The idea is to have something that can be used in embedded C++ systems that compiles down to more or less plain integer math operations, however **with compile-time and well placed (low-cost) runtime checks that make sure that the integers do not exceed a specified value range**.

## Motivation

In embedded systems programmed with C/C++, it's quite common to avoid the built-in floating-point types `float` and `double`. They have a high negative impact on code size and execution time, which are both precious goods in micro-controllers. An established way around the use of floating-point values is to use integers scaled with some *fixed* power of two factor so that decimal places can be expressed as well.  
For example, if a 32-bit unsigned integer is scaled with the factor 2<sup>16</sup>, there are 16 bits before the comma to express integers (0-65535) and 16 remaining bits after the comma to express fractions (with a precision of 2<sup>-16</sup>). This is an example of the very common UQ16.16 `fixed-point` type in ARM's **Q notation** that can represent numbers from *0* to *2<sup>16</sup> - 2<sup>-16</sup> = 65535.99998474*. It's called fixed-point because the position of the decimal separator, the point, is fixed. This separator can be fixed at any desired position: UQ0.32, UQ5.27 and UQ31.1, for example, represent fixed-point types with different separator positions but the same 32-bit unsigned integer base type.  
Negative numbers are usually expressed as signed integers in two's complement representation. For instance, the Q14.2 fixed-point type has a signed 16-bit integer base type with 1 sign bit, 13 integer bits and 2 fraction bits to represent values between *-2<sup>13</sup> = -8192.00* and *+2<sup>13</sup>-2<sup>-2</sup> = 8191.75*.

### Problems and Expectations

What sounds reasonable so far is actually one of the most underestimated sources for bugs:

- Mathematical operations like multiplications and especially more complex formulas tend to exceed the value range of the underlying base type quite fast. When no overflow check is performed and the values are not clamped, the integers will quietly overflow and the calculations will return completely wrong values with unpredictable/undefined consequences.
- Calculations with scaled values are quite complicated when done manually because scaling corrections are needed for many operators. As a result complex formulas are often hard to read and correcting factors can easily be forgotten since they are not intuitive.

In the end, one just wants to perform calculations within a predefined value range and a given precision, without having to worry about things like overflow too much. So let's define a list of expectations what a fixed-point library with overflow protection should provide:

- predefined fixed-point types based on signed and unsigned integer types (8, 16, 32 and potentially 64 bits)
- user-defined precision and value range (at compile-time)
- ability to specify the value range via floats (= unscaled values) at compile-time
- ability to change the precision and/or the value range later in code (at compile-time, via types)
- no implicit construction from floating-point types (safety, to avoid confusion)
- implicit construction (of Sq) from static integers when they appear in formulas (e.g. as factors)  -->  not possible yet (no constexpr function arguments yet); literals can be used instead
- explicit construction from static, scaled integers and real floating-point values at compile time
- explicit construction from integer-based variables with scaled integer values at runtime
- no runtime construction from floating-point variables (we don't want floats at runtime)
- compile-time overflow checks where possible, runtime-checks only when really needed
- different types of (runtime) overflow behaviors (overflow: forbidden, assert, clamp, allowed/no-check)
- implicit conversion between fixed-point types of same base type only to higher precision (no losses)
- explicit conversion to fixed-point type with same base type but different precision via up/downscale-copy
- conversion to different base types only via explicit casts (static_q_cast, safe_q_cast, force_q_cast)
- simple, easy-to-debug, on-point formulas without any obscuring scaling corrections
- implementation of the most-common mathematical operators that make sense (+, -, \*, /, %, ==, !=, <, >, <=, >=)
- multiplication and division with integral constants; a std::integral_constant can be constructed via literal `_ic`, e.g.: &emsp; `2_ic * x / 3_ic`
- at the moment, shift operations are only possible using std::integral_constant to shift a value, e.g. `x << 2_ic`
- abs, clamp, clampLower, clampUpper, min, max (constexpr impl. for both compile-time and runtime)
- some sophisticated operators like pow, sqr, sqrt -> integral powers and roots
  - pow: &ensp; x<sup>y</sup> = [ (x\*2<sup>f</sup>)<sup>y</sup> \* 2<sup>f - f\*y</sup> ]<sub>f</sub> &emsp; x: real, y: int, f: int &emsp; <-- std::pow requires double!
    - square: &ensp; x<sup>2</sup> = [ (x\*2<sup>f</sup>)\*(x\*2<sup>f</sup>) / 2<sup>f</sup> ]<sub>f</sub> &emsp; <-- this works!
    - cube: &ensp; x<sup>3</sup> = [ (x\*2<sup>f</sup>)\*(x\*2<sup>f</sup>)/2<sup>f</sup>\*(x\*2<sup>f</sup>)/2<sup>f</sup> ]<sub>f</sub> &emsp; <-- this works!
  - root: &ensp; x<sup>1/y</sup> = [ (x\*2<sup>f</sup>)<sup>1/y</sup> \* 2<sup>f - f/y</sup> ]<sub>f</sub> &emsp; <-- no generic root function in std library!
    - sqrt: &ensp; x<sup>1/2</sup> = [ (x\*2<sup>f</sup>\*2<sup>f</sup>)<sup>1/2</sup> ]<sub>f</sub> &emsp; <-- std::sqrt requires double! binary search calculation is used instead
    - cbrt: &ensp; x<sup>1/3</sup> = [ (x\*2<sup>f</sup>\*2<sup>f</sup>\*2<sup>f</sup>)<sup>1/3</sup> ]<sub>f</sub> &emsp; <-- std::cbrt requires double! hardware algorithm is used instead

### Further Ideas

- Function to static-assert Sq value range at compile-time (e.g. after a complicated calculation)

## This Library

Provides different fixed-point types which fulfill the expectations from above (more or less).

### Some Ideas (will be replaced with proper sections in this document when ready)

````C++
/*
 * normal fixed-point type in Q notation with value clamping or assertion at runtime
 */

// base-type, number of fraction bits f, minimum value, maximum value, overflow protection;
// note: only scaled integer value (mem-value) is stored in memory; rest (e.g. value range) is
//       compile-time-only!
fpm::q::Q<type, f, v_min, v_max, ovf>;

// predefined types
using i32q<...> = fpm::q::Q<int32_t, ...>;
using u32q<...> = fpm::q::Q<uint32_t, ...>;
using i16q<...> = fpm::q::Q<int16_t, ...>;
using u16q<...> = fpm::q::Q<uint16_t, ...>;
// ...

// user-defined types
// note: overflow behaviors are examples here; best practice is to use the default, forbidden, and
// to change the overflow behavior explicitly when needed/desired (so that a dev has control when
// the compiler should add overflow checks; code does not compile if a check is needed
// -> this way a dev can add a check explicitly, or fix the bug if the check should not be needed)
using u32q16<...> = u32q<16, ..., fpm::Overflow::clamp>;  // res. 2^-16; overflow: clamping
using i32q16<...> = i32q<16, ..., fpm::Overflow::assert>;  // res. 2^-16; overflow: assertion
// res. 2^-20; overflow at runtime forbidden -> code does not compile if check would be needed
using u32q20<...> = u32q<20, ...>;  // Overflow::forbidden is default
using i16q2<...> = i16q<2, ..., fpm::Overflow::clamp>;  // res. 2^-2; overflow: clamping


/* declaration and initialization */
u32q16<> a0::fromReal<99.9>;  // direct initialization; default value range is full possible range
auto a = u32q16<>::fromReal<45678.123>;  // construction
auto b = u32q16<45.0, 98.2>::fromReal<66.>;  // constr.; value range 45.0-98.2 (2949120-6435635);
// value range specified via scaled integer is not useful because if the value of n is changed
// all ranges need to be adapted when scaled values are used; this is not needed for real values
// and lets be honest - this is not intuitive either.
//auto c = u32q16<1966080, 3932160>::fromReal<45.1>;

// copy: construct from another Q value with same base-type; value range and overflow behavior can
// be changed this way; note that copy will perform a range check at runtime when the lhs range is
// smaller than the rhs range
auto d1 = u32q16<>::fromQ<Overflow::assert>(b);
auto d2 = u32q16<40000.0, 50000.0>::fromQ(a);  // limitation of value range; will perform range
                                               // check at runtime
// upscale-copy: mem-value increased by 2^4 and checked at runtime; value range implicitly reduced
auto e = u32q20<>::fromQ(a);
auto e2 = u32q20<>::fromQ( u32q16<>::fromReal<1.1> );  // construct temporary q16 and upscale-move
                                                       // to q20 lvalue
// downscale-copy: mem-value decreased at runtime without checks; value range implicitly extended
auto f = u32q16<>::fromQ(e);


/* assignment operator */
// assigns value of a to f; performs runtime checks when lhs range is smaller than rhs range;
// note: when overflow checks are not allowed for lhs type, this operation will not compile!
f = a;


/* casting */
// to be used when you would use casting for standard types as well
// - explicit cast from signed to unsigned or vice versa (e.g. u32 to i32);
// - explicit cast to different base type size (e.g. i32 to i16)

// whether or not runtime checks are performed, depends on the type of the cast:
auto cast1 = static_cast<i16q2<>>(b);  // Performs checks if needed (decided at compile-time).
auto cast1b = static_q_cast<i16q2<>, Overflow::clamp>(b);  // Same as static_cast but with overflow
                                                           // override.
auto cast2 = safe_q_cast<i16q2<>, Overflow::assert>(a);  // Safe cast will always perform checks.
                                                         // Overflow:noCheck is not permitted.
// Forced cast doesn't perform any scaling or overflow checks! Value is simply reused.
// Can overflow! (E.g. useful if an overflow is required as part of an algorithm.)
auto cast3 = force_q_cast<i16q2<40., 100.>>(b);

// copy constructors can be used to cast when the base type is the same but the precision different


/* operations */
// Q values do not implement operators! When operators are used, they are implicitly converted to
// static Q values (see next section) within the same range. The user can, if needed, change the
// value range by explicitly converting the value.


/* static Q-type for static formulas that have to be used to guarantee at compile time that a
 * calculation works for a range of input values. Runtime checks are only needed when a Q value is
 * transformed into an Sq value (and vice versa) when the value range gets smaller.
 * Sq-only formulas are guaranteed to be safe at runtime, because for each operator the value range
 * is modified and checked against overflow at compile-time. Runtime checks are not included as long
 * as only Sq values are used in the formula. This guarantees that the formula compiles into an
 * efficient calculation.
 * Note: By design, Sq values cannot be changed. For each operator a new Sq value is constructed.
 *  => Q values are dynamic and can be changed, but they do not implement mathematical operations.
 *     For formulas Sq values are needed; they are static so that formulas can be guaranteed to be
 *     secure for a given range of input values. Results are usually transformed back into Q values.
 *  => Q values store dynamic values, Sq values are used for calculations. */
fpm::sq::Sq<type, f, v_min, v_max>;

// predefined types
using i32sq<...> = fpm::sq::Sq<int32_t, ...>;
using u32sq<...> = fpm::sq::Sq<uint32_t, ...>;
using i16sq<...> = fpm::sq::Sq<int16_t, ...>;
using u16sq<...> = fpm::sq::Sq<uint16_t, ...>;

// user-defined types
// from above: for Q-types, corresponding Sq types are created; can be accessed via Q<>::Sq<>;
//             default value range of these Sq types is that of the related Q types;
//             can be changed to a different (usually smaller) range of values!


// ...
// some given Q values, e.g. speed [mm/s] and acceleration [mm/s^2]
using speed_t = i32q16<-100., 100.>;
using accel_t = i32q16<-10., 10.>;
using pos_t = i32q16<-10000., 10000.>;
auto speed = speed_t::fromReal<50.>;
auto accel = accel_t::fromReal<5.>;
auto pos = pos_t::fromReal<1000.>;
// ...

// Sq-calculation, performed safely via Sq values. As mentioned above, if only Sq values are used
// in a formula, this whole calculation will not include any overflow checks at runtime, because the
// compiler is doing these checks for the value ranges at compile-time and the code does not compile
// when the calculated value range does not fit into the user-defined, expected value range.
// Note: Conversion from Q to Sq space can be implicit, conversion from Sq to Q space can only be
//       implicit if no overflow check is necessary, otherwise it has to be done explicitly!
//
// Note: About conversions between Q and Sq: the Q type knows the corresponding Sq type, but the Sq
//       type does NOT know the corresponding Q type. Therefore conversions have to be performed
//       from Q perspective (i.e. Q can be constructed from Sq and it can be converted to Sq, but
//       Sq cannot be converted to Q or be constructed from Q).
//
// not needed: construct Sq value from corresponding Q value;  -> is done implicitly for operators;
//             default value range of Sq type is that of the Q type
//speed_t::Sq<> v0 = speed.toSq<>();  // conversion Q -> Sq
//accel_t::Sq<> a = accel.toSq<>();
//
// explicit change of value range: Sq value for pos0 with a smaller value range; performs overflow
// checks!
auto s0 = pos.toSq< -5e3, 5e3, Overflow::clamp >();
//
// also given: current time [s]
auto time = u16sq8<0., 10.>::fromReal<4.>;
//
// calculation; for a range of input values; expect an Sq value within a given range (can and should
// be calculated with the real decimal range values given when the types are defined; for example,
// 10*10*10/2 + 100*10 + 10000 = 11500 is the upper limit of the output range);
// no checks are performed; there are only operations, implicit precision and scaling corrections
// one would need to perform manually when doing these calculations with scaled integers;
// -> output value is always of Sq type! Can be converted explicitly to a Q value eventually.
// s = 1/2*a*t^2 + v0*t + s0
pos_t::Sq<-6500., 6500.> s = accel*time*time / 2_ic + speed*time + s0;
//
// alternative: explicit conversion in situ:
pos_t::Sq<-6500., 6500.> s = accel*time*time / 2_ic + speed*time + pos.toSq<-5e3, 5e3, Overflow::clamp>();
//                                                            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
// next calculation step; note that a new variable needs to be defined because s cannot be changed
// since it is of Sq type.
pos_t::Sq<-6500., 7000.> s2 = s + pos_t::Sq<0., 500.>::fromReal<250.>; 
//    add some constant value from a range ^^^^^^^^^^^^^^^^^^^^^^^^^^
//
// now update position in Q scaling;
// performs no check when value of s is assigned to pos because of smaller value range of s2
// => calculations via Sq, storage at runtime via Q;
// explicit conversion Sq -> Q (via named constr. of Q); implicit conversion is not possible here
// because value range of Sq type is larger than that of Q type
pos = pos_t::fromSq< ovf_override >(s2);

// explicit conversion of constant numbers in formulas via literals
// -> numbers are converted explicitly via literal operator to either sq type, or integral constant;
//    the latter is only supported by multiplications/divisions (because for these operators both
//    the real value and the scaled value are scaled by the same factor)
pos_t::Sq<> s3 = 2_ic * s / 3_ic;  // 2 and 3 are converted to integral constants via literal _ic
pos_t::Sq<> s4 = s * 4.2_i32q16;   // converts 4.2 to i32q16<4.2,4.2>::fromReal<4.2>
pos_t::Sq<> s5 = s / 3.14159_i32q16;

// >> some thoughts about implicit conversion of numbers in formulas:
// - ideally, numbers are converted implicitly at compile-time to the (resulting) type on the lhs,
//   or to the type on the rhs if a formula starts with a number; this is ambiguous to some extend,
//   because the user might wonder whether the underlying integer or the real value is modified
//   for each operator (by design it should be the latter)
// - NOT SUPPORTED YET; as of 2023, C++ does not have constexpr function parameters

/* user-defined literals */
// Literals open up lots of opportunities. For example, the user could define a literal operator
// for the pos_t from above:
template< char ...chars >
consteval auto operator""_mm() { return pos_t::fromLiteral<chars...>(); }
//
// Now it is possible to create a pos_t variable from a number that uses the literal _mm:
pos_t position = 500.5_mm;  // i32q16<-10000., 10000.>::fromReal<500.5>
//
// Such literals can be used in any formula of Q variables.
pos_t::Sq<> position2 = position + 100.1_mm;


/* mathematical operators */
// using:
u32sq16<4.e4, 5.e4> aa = a;
u32sq20<4.e4, 5.e4> ee = e;
//
/* addition */
u32sq16<> g = aa + b;
u32sq16<0., 45700.> h = aa + b;  // runtime check error: out of range (beyond upper limit)
u32sq20<> i = aa + ee;  // addition performed in q20 (higher precision of e) and stored as q20 (i)
u32sq16<> j = aa + ee;  // addition performed in q20 (higher precision of e) and stored as q16 (j)
//
// remember: no range check performed when R1 * R2 (ranges Ri, * is an operator) cannot go ooR
auto x = u32q16<40., 80.>::fromReal<50.>;
auto y = u32q16<10., 20.>::fromReal<15.>;
u32sq16<> sz = x + y;  // no range check performed here; implicit conversion of x and y to Sq type!
u32q16<> z = sz;  // implicit conversion of Sq result back to Q-value (same value range)
//
//
/* subtraction */
/* multiplication */
/* division */
// similar to addition

// clamp value
posClamped = fpm::q::clamp(position, -100_mm, 100_mm);  // this
posClamped2 = fpm::q::clamp<-100., 100.>(position);  // this re-limits type and clamps value
// note: ADL is used when clamp() is unqualified: clamp(position, min, max)

// ...
````

---

TODO: provide full readme based on the following template:

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
