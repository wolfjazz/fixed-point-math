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
