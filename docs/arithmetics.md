# Arithmetics

The primary capability of the `Sq` type is its extensive set of arithmetic operators and mathematical functions. These are designed to check the operations on a range of values at compile-time to assess potential overflow risks. If the evaluation confirms that the operations are safe within the value range of a specified `Sq` type, the code compiles. This compile-time safety check ensures that the operators and functions are reliable for use at runtime, enhancing the robustness of the `Sq` type.

## Overview

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

**Mathematical Functions**:  
The `Sq` type includes several built-in mathematical functions to extend its usability:

- **Absolute (abs)**: Returns the absolute value of an `Sq` instance.
- **Square (sqr) and Cube (cube)**: Functions to compute the square (\(x^2\)) and cube (\(x^3\)) of an `Sq` instance.
- **Square Root (sqrt) and Cube Root (cbrt)**: Functions to calculate the square root and cube root of an `Sq` instance.
- **Reverse Square Root (rsqrt)**: Calculates the reciprocal of the square root, commonly used in graphics and physics calculations to improve performance.
- **Minimum (min) and Maximum (max)**: Functions to determine the minimum and maximum of two `Sq` instances.

**Clamping Functions**:  
Clamping functions are essential to maintain values within a specific range, both at runtime and compile-time, especially when dealing with edge cases:

- **Clamp to Minimum**: Ensures the `Sq` instance does not fall below a specified minimum value.
- **Clamp to Maximum**: Ensures the `Sq` instance does not exceed a specified maximum value.
- **Clamp to Range**: Restricts the `Sq` instance within a specified minimum and maximum range, combining both of the above functionalities.

These operators and functions make the `Sq` type a powerful tool in the fixed-point math library, allowing for efficient and safe mathematical computations. Each operation is optimized to leverage the static nature of `Sq` values, ensuring computations are both fast and reliable, with checks and balances performed at compile-time to prevent runtime errors and ensure type safety.

## Unary Operators

### Unary Plus (+)

The unary plus operator in the context of the `Sq` type simply copies the `Sq` variable. Typically, the unary plus operator is used for integral promotion, but this concept does not apply to the `Sq` type. The inclusion of this operator is mainly for the sake of completeness, ensuring that the `Sq` type has a consistent set of unary operators.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | Sq::base_t |
| **f** | Sq::f |
| **realMin** | Sq::realMin |
| **realMax** | Sq::realMax |
| | |
| *value* | value |

**Example:**

```cpp
u32sq16<0., 10000.> sqVar = 5678.9_u32sq16;
auto sqVarCopy = +sqVar;  // Copies sqVar without any change in value and type
```

### Unary Minus (-)

The unary minus operator inverts the sign of the value and the limits of the `Sq` type. This transformation essentially mirrors the value range around the origin.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | Sq::base_t |
| **f** | Sq::f |
| **realMin** | -Sq::realMax |
| **realMax** | -Sq::realMin |
| | |
| *value* | -value |

**Example:**

```cpp
i16sq7<-100., 200.> sqVar = 150.0_i16sq7;
auto sqVarInverse = -sqVar;  // i16sq7<-200., 100.>, real value -150.
```

## Binary Operators

### Addition (+)

Adds two `Sq` instances, producing a new `Sq` instance with a proper base type, the larger resolution, the limits added together, and the resultant value.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | smallest integer fitting the resulting range, no smaller than any input;<br>signed if either SqLhs or SqRhs is signed, otherwise unsigned |
| **f** | max( SqLhs::f, SqRhs::f ) |
| **realMin** | SqLhs::realMin + SqRhs::realMin |
| **realMax** | SqLhs::realMax + SqRhs::realMax |
| | |
| *value* | s2s<SqLhs::f, f, base_t\>(lhs) + s2s<SqRhs::f, f, base_t\>(rhs) |

**Example:**

```cpp
i16sq7<-100., 200.> sq1 = 150.0_i16sq7;
u16sq7<0., 100.> sq2 = 50.0_u16sq7;
auto sqAdded = sq1 + sq2;  // i16sq7<-100., 300.>, real value 200.
```

### Subtraction (-)

Subtracts the right-hand side `Sq` instance from the left-hand side `Sq` instance, producing a new `Sq` object with a proper base type, the larger resolution, the limits subtracted, and the resultant value.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | smallest integer fitting the resulting range, no smaller than any input;<br>signed if either SqLhs or SqRhs is signed, otherwise unsigned |
| **f** | max( SqLhs::f, SqRhs::f ) |
| **realMin** | min( SqLhs::realMin - SqRhs::realMax, SqRhs::realMin - SqLhs::realMax ) |
| **realMax** | max( SqLhs::realMax - SqRhs::realMin, SqRhs::realMax - SqLhs::realMin ) |
| | |
| *value* | s2s<SqLhs::f, f, base_t\>(lhs) - s2s<SqRhs::f, f, base_t\>(rhs) |

**Example:**

```cpp
i16sq7<-100., 200.> sq1 = 150.0_i16sq7;
u16sq7<0., 100.> sq2 = 50.0_u16sq7;
auto sqSub = sq1 - sq2;  // i16sq7<-200., 200.>, real value 100.
```

### Multiplication (*)

#### `Sq` * `Sq`

Multiplies two `Sq` instances, producing a new `Sq` object with a proper base type, the larger resolution, the multiplied limits, and the resultant value.  
During multiplication, an intermediate calculation type is used, which has twice the size and the sign of the unpromoted,    common base type derived from the two input types. For instance, if the input base types are `int8_t` and `uint8_t`, the common base type and calculation type would be `uint8_t` and `uint16_t`, respectively. Even if one of the operands is negative and the calculation type is unsigned, the operation will still yield the correct result as integer wrapping ensures the desired result when cast to the signed base type of the resultant `Sq` type.

**Output:**

| `Sq` | <span style="font-weight:normal;font-size:8pt">*lhsMin = SqLhs::realMin, lhsMax = SqLhs::realMax*, *rhsMin = SqRhs::realMin, rhsMax = SqRhs::realMax*</span> |
|------|-|
| **base_t** | smallest integer fitting the resulting range, no smaller than any input;<br>signed if either SqLhs or SqRhs is signed, otherwise unsigned |
| **f** | max( SqLhs::f, SqRhs::f ) |
| **realMin** | min( min( lhsMin\*rhsMax, rhsMin\*lhsMax ), min( lhsMin\*rhsMin, lhsMax\*rhsMax ) ) |
| **realMax** | max( max( lhsMax\*rhsMin, rhsMax\*lhsMin ), max( lhsMin\*rhsMin, lhsMax\*rhsMax ) ) |
| | |
| *value* | s2s<2*f, f, base_t\>( s2s<SqLhs::f, f, calc_t\>(lhs) \* s2s<SqRhs::f, f, calc_t\>(rhs) ) |

**Example:**

```cpp
i16sq7<-200., 100.> sq1 = -150.0_i16sq7;
u16sq7<0., 100.> sq2 = 50.0_u16sq7;
auto sqMul = sq1 * sq2;  // i32sq7<-20000., 10000.>, real value -7500.
```

#### `Sq` * Integral Constant

Multiplies an `Sq` instance with an integral constant. The result is a new `Sq` type with a proper base type, the same resolution, and both the limits and the value multiplied with the integral constant.

**Note that plain integer literals do not work**, as the C++ language does not support the necessary operator overloads with integers in the compile-time context (yet). The `_ic` literal provided by this library can be used to construct an integral constant with `unsigned int` type, which is less tedious than the `std::integral_constant<T,v>` trait from the standard library.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | smallest integer fitting the resulting range, no smaller than the input;<br>signed if either Sq or the integral constant is signed, otherwise unsigned |
| **f** | Sq::f |
| **realMin** | min( Sq::realMin * ic, Sq::realMax * ic ) |
| **realMax** | max( Sq::realMin * ic, Sq::realMax * ic ) |
| | |
| *value* | static_cast<base_t\>(value) * static_cast<base_t\>(ic) |

**Example:**

```cpp
i16sq7<-200., 100.> sq1 = -150.0_i16sq7;
u16sq7<0., 100.> sq2 = 50.0_u16sq7;
auto sqMul1 = sq1 * 3_ic;    // i32sq7<-600., 300.>, real value: -450.0
auto sqMul2 = -40_ic * sq2;  // i32sq7<-4000., 0.>, real value: -2000.0
```

### Division (/)

### Modulo (%)

## Comparison Operators

### (Non-) Equality (==, !=)

### Ordering (<, >, <=, >=)

## Shift Operators

### Left Shift (<<)

### Right Shift (>>)

## Mathematical Functions

### Absolute (abs)

### Square (sqr)

### Square Root (sqrt)

### Inverse Square Root (rsqrt)

### Cube (cube)

### Cube Root (cbrt)

### Minimum (min)

### Maximum (max)

## Clamping Functions

### Clamp to Minimum (clampLower)

### Clamp to Maximum (clampUpper)

### Clamp to Range (clamp)
