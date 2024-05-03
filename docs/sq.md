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

## Type Examples

To illustrate the versatility and utility of the `Sq` type, here are a few examples showing how different configurations can be applied to accommodate various numeric ranges and precisions:

- `fpm::Sq<int16_t, 8, -100., 100>` represents an `Sq` type that uses a 16-bit signed integer (`int16_t`). The number of fractional bits is 8 (`f=8`), allowing for a decimal precision that suits a real value range of -100 to +100. 

- `fpm::Sq<uint32_t, 12>` utilizes a 32-bit unsigned integer (`uint32_t`) with 12 fractional bits (`f=12`) and a full real value range, which is determined based on the capabilities of the unsigned integer and the specified number of fractional bits.

These examples demonstrate the flexibility of the `Sq` type in adapting to various needs by simply adjusting the base type, the number of fractional bits, and optionally (yet recommended), the value range.

### Aliases and Literals

To simplify the usage of common `Sq` types in the library, shorter type aliases are provided via the `fpm::types` namespace. These aliases allow for more concise and readable code, particularly when dealing with commonly used configurations. Additionally, literals are defined for each alias to facilitate straightforward and error-free value assignments directly in the code.

#### Common `Sq` Type Aliases

Here are examples of some typical type aliases for `Sq` types:

- **`i32sq16<min, max>`**: Alias for `fpm::Sq<int32_t, 16, min, max>`. This configuration uses a 32-bit signed integer with 16 fractional bits, suitable for a range defined by `min` and `max`.
- **`u16sq7<min, max>`**: Alias for `fpm::Sq<uint16_t, 7, min, max>`. This version employs a 16-bit unsigned integer with 7 fractional bits, accommodating the specified real value range.
- **`i8qm2<>`**: Alias for `fpm::Sq<int8_t, -2>`. This configuration uses an 8-bit integer with `-2` fractional bits, effectively scaling the range upwards to accommodate larger values. The full symmetric value range achievable with this setup is from `-508.` to `508.`, facilitated by the negative scaling which expands the representable value range at the cost of precision (\(2^2\)).

Such aliases exist for the most common `Sq` (and `Q`) types, covering all integral types up to 32 bits and scalings `f` ranging from \(\small\left[-bitnum/2, bitnum-1\right]\).

#### Literals for `Sq` Type Aliases

To enhance usability, literals corresponding to each type alias are provided, allowing for immediate and clear value initialization:

- `-45.4_i32sq16` results in an `i32sq16<-45.4, -45.4>` initialized with a real value of `-45.4`.
- `15.5_u16sq7` creates an `u16sq7<15.5, 15.5>` with the real value `15.5`.
- `256_i8sqm2` creates an `i8sqm2<256, 256>` with a real value of `256`.

It's crucial to include an underscore between the numeric value and the literal suffix, as in `15.5_u16sq7` and `-45.4_i32sq16`, to ensure correct parsing and association in the source code.

**Why is the Value Range Restricted to the Literal Value?**

When an `Sq` type (or `Q` type) is initialized from a literal, the value range is intentionally set to be exactly the same as the value. This approach ensures that when compile-time calculations are performed involving these `Sq` types' limits, the potential for value range expansions that could lead to overflow (which would not compile) is minimized. Essentially, by limiting the `Sq` type to the precise value given in the literal, the type becomes highly predictable and efficient in computations, particularly in environments where precise control over numerical limits and performance is critical. This strategy simplifies handling edge cases and ensures that calculations remain as streamlined and efficient as possible, especially when multiple `Sq` types interact within an expression or algorithm.

## Construction

The `Sq` type allows for compile-time construction from scaled integers and real floating-point values. Unlike the `Q` type, `Sq` has a constant value at runtime from a value range known at compile-time and does not implement any overflow behavior.

### Compile-Time Construction

#### From Scaled Integers

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

#### From Real Values

Similarly, `Sq` instances can be constructed from real floating-point values at compile-time using the `Sq<...>::fromReal<.>()` constructor function:

```cpp
// Constructs i16sq4 with a value range from -100. to 100., from real value 6.3
auto sqR1 = fpm::Sq<int16_t, 4, -100., 100.>::fromReal< 6.3 >();  // scaled: 100

// Constructs i8sq1 with a value range from -50. to 50. and a real value 35.67
auto sqR2 = i8sq1<-50., 50.>::fromReal< 35.67 >();  // scaled: 71
```

#### From Literals

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

### Runtime Construction Through Q

Unlike the `Q` type, `Sq` does not have a runtime `construct<>()` method. Direct construction of `Sq` instances from runtime variables is not possible; instead, construction through a `Q` variable via the `qVar.toSq< realMin, realMax, ovfBxOvrd >()` method is necessary to enforce proper runtime overflow checks:

```cpp
// Constructing Q instance from a runtime scaled integer
int32_t someVariable = fpm::scaled<16, int32_t>()
auto qV = i32q16<-10., 50.>::fromReal< 42.5 >();  // note: Q not Sq

// Constructing Sq instance from Q value; this Sq has similar properties than Q
auto sqFromQ = qV.toSq();  // Sq<int32_t, 16, -10., 50.>, real value: 42.5

// toSq<>() can be used to clamp the Q value to a different Sq type
auto sqFromQ2 = qV.toSq<0., 38., Ovf::clamp>();  // i32sq16<0.,38.>, v: 38|real

// if the target Sq type has a wider range, no clamping is needed
auto SqFromQ3 = qV.toSq<-20., 50.>();  // i32sq16<-20.,50.>, real value: 42.5
```

Additionally, `Q` implements a unary `+` operator, which provides a shortcut for constructing a similar `Sq`-typed variable from a `Q` variable. This unary operator functions similarly to `qVar.toSq()`. Using `+qVar` is a shorthand that automatically converts a `Q` variable `qVar` to an `Sq` type that retains the same properties as `qVar`:

```cpp
auto qVal = fpm::Q<int32_t, 16, -10., 50.>::fromReal< -8.8 >();

// conversion of a Q value to an Sq value with similar properties:
auto sqVal = +qVal;  // i32sq16<-10., 50.>, real value -8.8
```

Furthermore, when used in formulas, `Q` variables are implicitly converted to similar `Sq` variables. This implicit conversion feature is designed to streamline the syntax by reducing the clutter caused by explicit conversion overhead. For instance, in expressions involving multiple operations or different data types, `Q` variables can be seamlessly integrated without the need to manually convert each one, thus simplifying the code and enhancing its readability.

Here's a rewritten section on accessing values in an `Sq` variable, aligned with the structure used for describing the `Q` class's value access methods:

## Value Access

The actual `Sq` value stored at runtime can be accessed through specific member functions. These functions provide controlled access to both the scaled representation and the real-value representation of the `Sq` type:

- **Accessing the Scaled Value:**
  The scaled value, which is the internally stored scaled integer, can be accessed using the `scaled()` member function. This is particularly useful for operations that require interaction with systems needing the integer directly or when performing low-level operations that depend on the scaled format.
  ```cpp
  auto scaledVal = sqVar.scaled();  // Retrieve the internal scaled value
  ```

- **Accessing the Real Value:**
  To retrieve the real value as a floating-point number, the `real()` function is used. This method converts the internal scaled integer back to its double precision floating-point representation. It is primarily useful for applications that require precise numerical outputs, such as for display or in high-level calculations. However, it's important to handle this value carefully due to the potential for floating-point inaccuracies and the computational overhead associated with floating-point operations.
  ```cpp
  double realVal = sqVar.real();  // Get the real value as double
  ```
  Similar to `Q`, the `real<int>()` variant can be used here to retrieve the integer part of the real value with the fractional part truncated. This truncation must be handled with care to avoid unintended data loss, especially when the type has a large fractional part.

## Rescaling and Casting

Rescaling and casting of `Sq` variables, compared to their counterparts in `Q` types, are considerably more restricted. The design of `Sq` types emphasizes compile-time checks and static value management, which inherently limits dynamic operations such as rescaling and casting. Here are the key aspects of rescaling and casting for `Sq` variables:

**Conditions for Rescaling:**
Rescaling is permitted only if the target `Sq` type has the same base type (`base_t`) and a different number of fractional bits (`f`). Importantly, the target type must also have a value range that is the same or wider than the source type to avoid potential overflows. This ensures that the precision and range can be adjusted without altering the fundamental data type or exceeding the numerical capacity.

```cpp
i16sq4<-100., 100.> sqSource = 25.75_i16sq4;  // scaled: 412

// Safe rescaling via assignment to a target type with the same base type
// and a compatible real value range
i16sq6<-100., 200.> sqRescaled = sqSource;  // scaled: 1648, real: 25.75

// Basically the same, but a bit more explicit.
auto sqRescaled2 = i16sq6<-100., 200.>::fromSq( sqSource );  // scaled: 1648
```

**Restrictions on Casting:**
Casting between different base types requires an explicit `static_cast` to ensure that changes in data representation are handled correctly. The target type must have a value range that is the same or broader than the source type to ensure that no overflow errors occur during the casting process.

```cpp
i16sq4<-60., 60.> sqI16 = -56.7_i16sq4;  // scaled: -907, real: -56.6875

// Explicit cast required to a target type with a different base type
auto sqI32 = static_cast< i32sq3<-60., 60.> >( sqI16 );  // s: -453, r: 56.625
auto sqI8 = static_cast< i8sqm4<-60., 60.> >( sqI32 );   // s: -3, r: -48
```

*Note*: In contrast to the `Q` type, there is no `safe_cast` and no `force_cast` for the `Sq` type. Both functions would contradict the foundational concept of `Sq`, which is designed to ensure compile-time safety and immutability of values, preventing any changes that could lead to runtime check overhead, errors, or value inconsistencies.

**Compile-Time Overflow Checks:**
The `Sq` type enforces `Ovf::error` as its inherent - and only - overflow behavior. If the target type's value range is narrower than the source type's, the compiler will block the operation to prevent any risk of overflow, thus maintaining strict type safety.

```cpp
i32sq4<-100., 100.> sqSource = -78.1_i32sq4;

// Error - target value range is narrower
i32sq5<-50., 50.> sqRescaled = sqSource;

// Error - target value range is narrower
auto sqCast = static_cast< i16sq4<-50., 50.> >( sqSource );
```

**Using `Q` for Handling Overflows:**
Dynamic overflow handling, such as clamping, must be implemented through `Q` types since `Sq` does not support runtime overflow-checking behaviors. This approach is necessary when adjusting value ranges dynamically or when dealing with potential overflows.

```cpp
i32sq4<-100., 100.> sqSource = -78.1_i32sq4;  // scaled: -1249

// Handling overflows by clamping via Q type; scaled: -960, real: -60
auto sqClamped = i32q4<-60., 60.>::fromSq< Ovf::clamp >( sqSource ).toSq();
```

This technique provides precise control over the timing of overflow checks within the `Sq` scope. By using `Q` for intermediate calculations, developers can ensure that overflows are addressed exactly when needed.

*Additional Note*: There are only a few edge cases where overflow handling on `Sq` through `Q` is even needed. These typically arise when the `Sq` variable is constructed from a variable of a `Q` type that uses explicit `Ovf::allowed`. In such cases, the potential for overflow exists because the `Q` type permits it, and the value, which potentially is out of range, can be passed to `Sq` via the `toSq()` method. Crucially, when such out-of-range values are passed, the compiler will still assume that the value is within the specified range of the `Sq` type. This might be desired behavior for some edge case computations, where handling or ignoring overflow could be beneficial. This is precisely why `Ovf::allowed` exists — to enable these specific scenarios where standard overflow management practices are intentionally bypassed.
