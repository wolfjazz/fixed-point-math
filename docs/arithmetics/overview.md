# Overview

The primary capability of the `Sq` type is its extensive set of arithmetic operators and mathematical functions. These are designed to check the operations on a range of values at compile-time to assess potential overflow risks. If the evaluation confirms that the operations are safe within the value range of a specified `Sq` type, the code compiles. This compile-time safety check ensures that the operators and functions are reliable for use at runtime, enhancing the robustness of the `Sq` type.

**Conversion of `Q` to `Sq`**:  
For variables of `Q` type, the `toSq()` method or the unary `+` operator can be utilized to transform them into a corresponding `Sq` type variable. This transformation ensures that dynamic values handled by the `Q` type are accurately converted into the static context of the `Sq` type, maintaining the integrity and constraints of the originally defined value range. If necessary, this conversion includes an overflow check, which by default adheres to the overflow behavior specified for the `Q` type. However, this behavior can be overridden when using the `toSq()` method. This ensures that any potential overflow issues are addressed during the conversion.

```cpp
auto qVar = i32q10<-1000., 1000.>::fromReal< -555.55 >();
auto sqVar1 = qVar.toSq<-500., 500., Ovf::clamp>();  // clamps; real: -500.0
auto sqVar2 = +qVar;  // inherits the range -1000 to 1000; real: -555.55
```

**Unary Operators**:  
`Sq` types support unary operators to manipulate the sign or value of an instance directly:

- **Unary Plus (+)**: This operator returns the value of the `Sq` instance as is, essentially a no-operation (no-op) in terms of value change.
- **Unary Minus (-)**: This operator returns the negated value of the `Sq` instance, effectively flipping its sign.

**Binary Operators**:  
`Sq` types implement the standard arithmetic operators to perform calculations between instances or between an instance and a scalar (integral constant):

- **Addition (+)**: Adds two `Sq` instances, producing a new `Sq` instance with the resultant value.
- **Subtraction (-)**: Subtracts one `Sq` instance from another.
- **Multiplication (*)**: Multiplies two `Sq` instances or an `Sq` instance with a scalar.
- **Division (/)**: Divides one `Sq` instance by another or by a scalar.
- **Modulo (%)**: Computes the remainder of division between two `Sq` instances or an `Sq` instance and a scalar.

**Comparison Operators**:  
Comparison operations are critical for logic and control flow in programming, and `Sq` types support all standard comparison operators:

- **Equal to (==)**
- **Not equal to (!=)**
- **Less than (<)**
- **Greater than (>)**
- **Less than or equal to (<=)**
- **Greater than or equal to (>=)**

These operators facilitate decisions and comparisons, ensuring that `Sq` instances can be directly used in conditional statements.

**Shift Operators**:  
Shift operators adjust the bit representation of the fixed-point values:

- **Left Shift (<<)**: Shifts the bits of an `Sq` instance to the left, effectively multiplying the value by a power of two.
- **Right Shift (>>)**: Shifts the bits of an `Sq` instance to the right, effectively dividing the value by a power of two, rounded towards \(-\infty\) to the nearest integer.

**Clamping Functions**:  
Clamping functions are essential to maintain values within a specific range, both at runtime and compile-time, especially when dealing with edge cases:

- **Clamp to Minimum**: Ensures the `Sq` instance does not fall below a specified minimum value.
- **Clamp to Maximum**: Ensures the `Sq` instance does not exceed a specified maximum value.
- **Clamp to Range**: Restricts the `Sq` instance within a specified minimum and maximum range, combining both of the above functionalities.

**Mathematical Functions**:  
The `Sq` type includes several built-in mathematical functions to extend its usability:

- **Absolute (abs)**: Returns the absolute value of an `Sq` instance.
- **Square (sqr) and Cube (cube)**: Functions to compute the square (\(x^2\)) and cube (\(x^3\)) of an `Sq` instance.
- **Square Root (sqrt) and Cube Root (cbrt)**: Functions to calculate the square root and cube root of an `Sq` instance.
- **Reverse Square Root (rsqrt)**: Calculates the reciprocal of the square root, commonly used in graphics and physics calculations to improve performance.
- **Minimum (min) and Maximum (max)**: Functions to determine the minimum and maximum of two `Sq` instances.

These operators and functions make the `Sq` type a powerful tool in the fixed-point math library, allowing for efficient and safe mathematical computations. Each operation is optimized to leverage the static nature of `Sq` values, ensuring computations are both fast and reliable, with checks and balances performed at compile-time to prevent runtime errors and ensure type safety.
