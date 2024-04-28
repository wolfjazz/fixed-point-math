# Sq-Type

The `Sq` type, standing for "Static Q," is a specialized variant of the `Q` type designed to handle constant values at runtime. Unlike the `Q` type, which accommodates dynamically changing values, the `Sq` type maintains a fixed value throughout its lifetime, making it particularly useful in scenarios where stability and predictability are paramount.

One of the principal advantages of the `Sq` type is the elimination of runtime overflow checks during computations. Because `Sq` types hold static values, the compiler can perform all necessary checks at compile time, ensuring that any formulas involving `Sq` types are verified for validity within the predefined value ranges. This static analysis significantly enhances performance and reliability, as it prevents runtime errors related to overflow and underflow.

`Sq` types are equipped with a range of mathematical functions and arithmetic operators, allowing them to efficiently handle a variety of calculations and operations that are integral to their use cases. This feature enables `Sq` types to function almost autonomously within their defined scope, facilitating complex mathematical operations without the need for runtime value adjustments.

However, while `Sq` types offer robust safety during calculations, they require careful management during type conversion. Overflow checks become necessary when converting between `Q` and `Sq` types, specifically at the points of conversion both to and from `Sq`. These checks are critical to ensure that the static values adhered to by `Sq` types are compatible with the potentially dynamic values of `Q` types.

In terms of scope and lifecycle, `Sq` types are inherently limited to the block of code in which they are defined. They cannot naturally persist or move beyond this local scope without conversion to a `Q` type. This characteristic makes `Sq` types ideally suited for temporary computations and intermediate steps within complex algorithms where values do not need to be retained post-computation.

Conversely, `Q` types are designed to store and manage runtime values that may need to be maintained or modified over time and across different scopes. The flexibility of `Q` types makes them indispensable for applications that require the storage and manipulation of values beyond a fixed context, providing a bridge from the localized, static environment of `Sq` types to broader, dynamic applications.

In summary, while both `Q` and `Sq` types are integral to the Fixed-Point Math Library, each serves distinct roles: `Sq` types for static, localized computations within a confined scope, and `Q` types for dynamic, persistent data management across wider operational contexts.

## Template

Similar to the `Q` type, the `Sq` type is defined using a template that specifies several critical parameters, enabling it to accurately represent fixed-point numbers within a specific real value range. The template for declaring an `Sq` type is outlined below:

```cpp
namespace fpm {

template< std::integral base_t,
          scaling_t f,
          double realMin,
          double realMax >
class Sq {
public:
    // ...
private:
    base_t const value;  ///< Constant value, ensuring the static nature of Sq
};

}
```

The parameters for the `Sq` type template are as follows:

- **base_t**: The base data type, which must be an integral type. This defines the type of the scaled integer that `Sq` uses to store its fixed-point number.
- **f**: The number of fractional bits. This parameter, defined as `fpm::scaling_t`, determines the scaling factor, affecting how much of the integer's precision is used for fractional parts.
- **realMin** and **realMax**: These parameters specify the minimum and maximum real values that the `Sq` type can represent. They are optional; if not explicitly set, the default is the largest feasible value range. For symmetric ranges in signed types, the default excludes the minimum value of the base type to prevent potential overflows when taking the absolute value, thereby ensuring safe operations within predictable limits. For signed base types where the range includes only negative values up to and including zero, it is highly recommended to use `-0.0` (note the explicit minus sign) as the upper limit to ensure clarity and prevent ambiguities at the zero boundary.

It's important to note that **the `Sq` type does not implement any overflow behavior**, reflecting its purpose for handling ranges of values that are constant and known at compile-time, thereby eliminating the need for runtime overflow checks.

Additionally, the `Sq` type provides the following static constexpr members:

- **scaledMin** and **scaledMax**: These members represent the minimum and maximum scaled integer values that can be derived from the specified `realMin` and `realMax`. These values ensure that all operations stay within the bounds defined by the fixed-point format of the `Sq` type.
- **resolution**: This member indicates the smallest increment in the real value range of the `Sq` type, calculated as \(2^{-f}\). It specifies the precision level of the `Sq`, providing clear information about the smallest change in value that the type can represent, essential for high-precision calculations.

## Construction of `Sq`

The `Sq` type allows for compile-time construction from scaled integers and real floating-point values. Unlike the `Q` type, `Sq` has a constant value at runtime from a value range known at compile-time and does not implement any overflow behavior.

### Compile-Time Construction

#### From Scaled Integers

To construct an `Sq` instance from a scaled integer at compile-time, the constructor function `Sq<...>::fromScaled<.>()` is used. Here are some examples:

```cpp
// Constructing i8sq2 with a value range from -30.0 to 30.0, and value 42
auto sq1 = fpm::Sq<int8_t, 2, -30., 30.>::fromScaled< 42 >();

// Constructing u32sq16 with a value range from 0.0 to 1000.0, and value 65536
auto sq2 = fpm::Sq<uint32_t, 16, 0., 1000.>::fromScaled< 65536 >();

// Constructing i32sq8 with a full symmetric value range, and a value 256
auto sq3 = fpm::Sq<int32_t, 8>::fromScaled< 256 >();
```

The template parameters specify the base type, the number of fractional bits and optionally the real value range of the desired `Sq` type. The static `fromScaled` constructor function initializes the `Sq` instance with the provided scaled integer value. This value is stored at runtime if the `Sq` variable is used at runtime. When specifying the value, it should be enclosed within the angle brackets `< >`. Additionally, the parentheses `()` at the end of the constructor function invocation should not be forgotten, as they are needed to invoke the `fromScaled` function.

#### From Real Values

Similarly, `Sq` instances can be constructed from real floating-point values at compile-time using the `Sq<...>::fromReal<.>()` constructor function:

```cpp
// Constructs i16sq4 with a value range from -100. to 100., from real value 12.3
auto sqR1 = fpm::Sq<int16_t, 4, -100., 100.>::fromReal< 12.3 >();
```

### Runtime Construction Through Q

Unlike the `Q` type, `Sq` does not have a runtime `construct<>()` method. Direct construction of `Sq` instances from runtime variables is not possible; instead, construction through a `Q` variable via the `qVar.toSq< realMin, realMax, ovfBxOvrd >()` method is necessary to enforce proper overflow checks:

```cpp
// Constructing Q instance from a runtime scaled integer
int32_t someVariable = fpm::scaled<16, int32_t>()
auto qVal = fpm::Q<int32_t, 16, -10., 50.>::fromReal< 42.5 >();

// Constructing Sq instance from Q value; this Sq has similar properties than Q
auto sqFromQ = qVal.toSq();  // Sq<int32_t, 16, -10., 50.>, real value: 42.5

// toSq<>() can be used to clamp the Q value to a different Sq type
auto sqFromQ2 = qVal.toSq<0., 38., Ovf::clamp>();  // i32sq16<0.,38.>, v: 38|real

// if the target Sq type has a wider range, no clamping is needed
auto SqFromQ3 = qVal.toSq<-20., 50.>();  // i32sq16<-20.,50.>, real value: 42.5
```

Additionally, `Q` implements a unary `+` operator, which provides a shortcut for constructing a similar `Sq`-typed variable from a `Q` variable. This unary operator functions similarly to `qVar.toSq()`. Using `+qVar` is a shorthand that automatically converts a `Q` variable `qVar` to an `Sq` type that retains the same properties as `qVar`:

```cpp
auto qVal = fpm::Q<int32_t, 16, -10., 50.>::fromReal< -8.8 >();

// Conversion of a q value to a sq value with similar properties:
auto sqVal = +qVal;  // i32sq16<-10., 50.>, real value -8.8
```

Furthermore, when used in formulas, `Q` variables are implicitly converted to similar `Sq` variables. This implicit conversion feature is designed to streamline the syntax by reducing the clutter caused by explicit conversion overhead. For instance, in expressions involving multiple operations or different data types, `Q` variables can be seamlessly integrated without the need to manually convert each one, thus simplifying the code and enhancing its readability.
