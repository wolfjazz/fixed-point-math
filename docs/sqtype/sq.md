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

#### Aliases for Common Types

Aliases exist for the most common `Sq` (and `Q`) types, covering all integral types up to 32 bits and scalings `f` ranging from \(\small-bitnum/2\) to \(\small bitnum-1\).  
Here are examples of some type aliases for `Sq` types:

- **`i32sq16< realMin, realMax >`**: Alias for `fpm::Sq<int32_t, 16, realMin, realMax>`. This configuration uses a 32-bit signed integer with 16 fractional bits, suitable for a real value range defined by `realMin` and `realMax`.
- **`u16sq7< realMin, realMax >`**: Alias for `fpm::Sq<uint16_t, 7, realMin, realMax>`. This version employs a 16-bit unsigned integer with 7 fractional bits, accommodating the specified real value range.
- **`i8qm2<>`**: Alias for `fpm::Sq<int8_t, -2>`. This configuration uses an 8-bit integer with `-2` fractional bits, effectively scaling the range upwards to accommodate larger values. The symmetric real value range achievable with this setup is from `-508.` to `508.`, facilitated by the negative scaling which expands the representable value range at the cost of precision (\(2^2\)).

#### Literals for Sq Type Aliases

To enhance usability, literals corresponding to each type alias are provided, allowing for immediate and clear value initialization:

- `-45.4_i32sq16` results in an `i32sq16<-45.4, -45.4>` initialized with a real value of `-45.4`.
- `15.5_u16sq7` creates an `u16sq7<15.5, 15.5>` with the real value `15.5`.
- `256_i8sqm2` creates an `i8sqm2<256, 256>` with a real value of `256`.

It's crucial to include an underscore between the numeric value and the literal suffix, as in `15.5_u16sq7` and `-45.4_i32sq16`, to ensure correct parsing and association in the source code.

**Why is the Value Range Restricted to the Literal Value?**

When an `Sq` type (or `Q` type) is initialized from a literal, the value range is intentionally set to be exactly the same as the value. This approach ensures that when compile-time calculations are performed involving these `Sq` types' limits, the potential for value range expansions that could lead to overflow (which would not compile) is minimized. Essentially, by limiting the `Sq` type to the precise value given in the literal, the type becomes highly predictable and efficient in computations, particularly in environments where precise control over numerical limits and performance is critical. This strategy simplifies handling edge cases and ensures that calculations remain as streamlined and efficient as possible, especially when multiple `Sq` types interact within an expression or algorithm.
