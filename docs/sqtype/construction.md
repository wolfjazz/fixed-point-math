# Construction

The `Sq` type allows for compile-time construction from scaled integers and real floating-point values. Unlike the `Q` type, `Sq` has a constant value at runtime from a value range known at compile-time and does not implement any overflow behavior.

## Compile-Time Construction

### From Scaled Integers

To construct an `Sq` instance from a scaled integer at compile-time, the constructor function `Sq<...>::fromScaled<.>()` is used. Here are some examples:

```cpp
// Constructing i8sq2 with a value range from -30.0 to 30.0, and value 42
auto sq1 = fpm::Sq<int8_t, 2, -30., 30.>::fromScaled< 42 >();  // real: 10.5

// Constructing u32sq16 with a value range from 0.0 to 1000.0, and value 65536
auto sq2 = u32sq16<0., 1000.>::fromScaled< 65536 >();  // real: 1.0

// Constructing i32sq8 with a full symmetric value range, and a value 256
auto sq3 = fpm::Sq<int32_t, 8>::fromScaled< 128 >();  // real: 0.5
```

The template parameters specify the base type, the number of fractional bits and optionally the real value range of the desired `Sq` type. The static `fromScaled` constructor function initializes the `Sq` instance with the provided scaled integer value. This value is stored at runtime if the `Sq` variable is used at runtime. When specifying the value, it should be enclosed within the angle brackets `< >`. Additionally, the parentheses `()` at the end of the constructor function invocation should not be forgotten, as they are needed to invoke the `fromScaled` function.

### From Real Values

Similarly, `Sq` instances can be constructed from real floating-point values at compile-time using the `Sq<...>::fromReal<.>()` constructor function:

```cpp
// Constructs i16sq4 with a value range from -100. to 100., from real value 6.3
auto sqR1 = fpm::Sq<int16_t, 4, -100., 100.>::fromReal< 6.3 >();  // scaled: 100

// Constructs i8sq1 with a value range from -50. to 50. and a real value 35.67
auto sqR2 = i8sq1<-50., 50.>::fromReal< 35.67 >();  // scaled: 71
```

### From Literals

Literals can be utilized to construct `Sq` variables through explicit assignment, where an *rvalue* created from a literal can be assigned to an *lvalue* variable that has the same or a wider real value range. If the literal value is out of range, the expression does not compile. This feature increases readability while still ensuring type safety. Here are some examples demonstrating this approach:

```cpp
// Constructing Sq from literal with single value within the variable's range
u16sq7<0., 200.> sqLit1 = 89.32_u16sq7;

// Constructing Sq from literal without range specification in type alias,
// assuming full range
i32sq20<> sqLit2 = -444.56_i32sq20;

// Does not compile: valid literal, however out of variable range
// i8sq2<-20., 20.> sqLit3 = 30.5_i8sq2;
```

## Runtime Construction

Unlike the `Q` type, `Sq` does not have a runtime `construct<>()` method. Direct construction of `Sq` instances from runtime variables is not possible; instead, construction through a `Q` variable via the `qVar.toSq< realMin, realMax, ovfBxOvrd >()` method is necessary to enforce proper runtime overflow checks.  
Each `Q` type inherently provides a corresponding `Sq` type with similar properties, which is accessible via `Q<...>::Sq<>`. Instances of this `Sq` type are constructed using the aforementioned `toSq()` method:

```cpp
// Constructing Q instance from a runtime scaled integer
int32_t someVariable = fpm::scaled<16, int32_t>()
auto qV = i32q16<-10., 50.>::fromReal< 42.5 >();  // note: Q not Sq

// Constructing Sq instance from Q value; this Sq has similar properties than Q;
// Conversion is trivial, no overflow check needed
auto sqFromQ = qV.toSq();  // Sq<int32_t, 16, -10., 50.>, real value: 42.5

// toSq<>() can be used to clamp the Q value to a narrower Sq type
auto sqFromQ2 = qV.toSq<0., 38., Ovf::clamp>();  // i32sq16<0.,38.>, v: 38|real

// if the target Sq type has a wider range, no clamping is needed
auto SqFromQ3 = qV.toSq<-20., 50.>();  // i32sq16<-20.,50.>, real value: 42.5
```

Additionally, `Q` implements a unary `+` operator, which provides a shorthand for constructing a similar `Sq`-typed variable from a `Q` variable. This unary operator functions similarly to `qVar.toSq()`:

```cpp
auto qVar = fpm::Q<int32_t, 16, -10., 50.>::fromReal< -8.8 >();

// conversion of a Q variable to an Sq variable with similar properties:
auto sqVar = +qVar;  // i32sq16<-10., 50.>, real value -8.8
```

Furthermore, when used in formulas, `Q` variables are implicitly converted to similar `Sq` variables. This implicit conversion feature is designed to streamline the syntax by reducing the clutter caused by explicit conversion overhead. For instance, in expressions involving multiple operations or different data types, `Q` variables can be seamlessly integrated without the need to manually convert each one, thus simplifying the code and enhancing its readability.
