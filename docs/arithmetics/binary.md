# Binary Operators

---

## Addition (+)

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

---

## Subtraction (-)

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

---

## Multiplication (*)

### Sq * Sq

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

### Sq * Integral Constant

Multiplies an `Sq` instance with an integral constant. The result is a new `Sq` type with a proper base type, the same resolution, and both the limits and the value multiplied with the integral constant.

**Note that plain integer literals do not work**, as the C++ language does not support the necessary operator overloads with integers in the compile-time context (yet). The `_ic` literal provided by this library can be used to construct an integral constant with `unsigned int` type, which is less tedious than the `std::integral_constant<T,v>` trait from the standard library.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | smallest integer fitting the resulting range, no smaller than any input;<br>signed if either Sq or the integral constant is signed, otherwise unsigned |
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

---

## Division (/)

### Sq / Sq

Divides one `Sq` instance by another, producing a new `Sq` object that properly balances the type, resolution, and limits based on the divisor and dividend. The division uses an intermediate calculation type that can handle a divisor in \(2^{2f}\) representation, ensuring adequate size and sign handling.

**Constraints:**

The dividend (`SqRhs`) must not have real values from the range \((-1,+1)\) within its value range. This constraint is crucial as it prevents division by zero and avoids any undesired expansion of the resulting value range that could lead to significant restrictions on further computations.

**Output:**

| `Sq` | <span style="font-weight:normal;font-size:8pt">*lhsMin = SqLhs::realMin, lhsMax = SqLhs::realMax*, *rhsMin = SqRhs::realMin, rhsMax = SqRhs::realMax*</span> |
|------|-|
| **base_t** | smallest integer fitting the resulting range, no smaller than any input;<br>signed if either SqLhs or SqRhs is signed, otherwise unsigned |
| **f** | max( SqLhs::f, SqRhs::f ) |
| **realMin** | min( min( lhsMin/rhsMax, lhsMin/rhsMin ), min( lhsMax/rhsMin, lhsMax/rhsMax) ) |
| **realMax** | max( max( lhsMin/rhsMax, lhsMin/rhsMin ), max( lhsMax/rhsMin, lhsMax/rhsMax) ) |
| | |
| *value* | static_cast<base_t\>( s2s<SqLhs::f, 2*f, calc_t\>(lhs) / s2s<SqRhs::f, f, calc_t\>(rhs) ) |

**Example:**

```cpp
i16sq7<-200., 100.> sq1 = -150.0_i16sq7;
u16sq7<1.0, 100.> sq2 = 50.0_u16sq7;
auto sqDiv = sq1 / sq2;  // i16sq7<-200., 100.>, real value -3.0
```

### Sq / Integral Constant

Divides an `Sq` instance by an integral constant. The result maintains the `Sq` type's base type and resolution, adjusting limits and value based on the constant.

**Note that plain integer literals do not work**, as the C++ language does not support the necessary operator overloads with integers in the compile-time context (yet). The `_ic` literal provided by this library can be used to construct an integral constant with `unsigned int` type, which is less tedious than the `std::integral_constant<T,v>` trait from the standard library.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | smallest integer fitting the resulting range, no smaller than any input;<br>signed if either Sq or the integral constant is signed, otherwise unsigned |
| **f** | Sq::f |
| **realMin** | min( Sq::realMin / ic, Sq::realMax / ic ) |
| **realMax** | max( Sq::realMin / ic, Sq::realMax / ic ) |
| | |
| *value* | static_cast<base_t\>(lhs) / static_cast<base_t\>(ic) |

**Example:**

```cpp
i16sq7<-200., 100.> sq1 = -150.0_i16sq7;
auto sqDiv = sq1 / 5_ic;  // i16sq7<-40., 20.>, real value: -30.0
```

### Integral Constant / Sq

Divides an integral constant by an `Sq` instance. The result maintains the `Sq` type's base type and resolution, adjusting limits and value based on the constant. The division uses an intermediate calculation type that can handle a divisor in \(2^{2f}\) representation, ensuring adequate size and sign handling.

**Constraints:**

The dividend (`SqRhs`) must not have real values from the range \((-1,1)\) within its value range. This constraint is crucial as it prevents division by zero and avoids any undesired expansion of the resulting value range that could lead to significant restrictions on further computations.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | smallest integer fitting the resulting range, no smaller than any input;<br>signed if either SqRhs or the integral constant is signed, otherwise unsigned |
| **f** | SqRhs::f |
| **realMin** | min( ic / SqRhs::realMin, ic / SqRhs::realMax ) |
| **realMax** | max( ic / SqRhs::realMin, ic / SqRhs::realMax ) |
| | |
| *value* | static_cast<base_t\>( s2s<0, 2*f, calc_t\>(ic) / static_cast<calc_t>(rhs) ) |

**Example:**

```cpp
i16sq7<-200., -10.> sq = -150.0_i16sq7;
auto sqDiv = -1500_ic / sq;  // i16sq7<7.5, 150.>, real value: -10.0
```

---

## Modulo (%)

### Sq % Sq

Calculates the modulo of one `Sq` instance by another, resulting in a new `Sq` object with adjusted type and limits based on the operands. This operation uses the unpromoted common type of the two input types as intermediate calculation type, corrected for the potential change in scaling, ensuring adequate size and sign handling.

**Constraints:**

The dividend (`SqRhs`) must not have real values from the range \((-resolution,resolution)\) within its value range. This constraint is crucial as it prevents a modulo zero, as this is not defined.

**Output:**

| `Sq` | <span style="font-weight:normal;font-size:8pt">*lhsMin = SqLhs::realMin, lhsMax = SqLhs::realMax*, *rhsMin = SqRhs::realMin, rhsMax = SqRhs::realMax*</span> |
|------|-|
| **base_t** | smallest integer fitting the resulting range, no smaller than any input;<br>signed if either SqLhs or SqRhs is signed, otherwise unsigned |
| **f** | max( SqLhs::f, SqRhs::f ) |
| **realMin** | max( lhsMin, signum(lhsMin) * max( abs(rhsMin), abs(rhsMax) ) ) |
| **realMax** | min( lhsMax, signum(lhsMax) * max( abs(rhsMin), abs(rhsMax) ) ) |
| | |
| *value* | static_cast<base_t\>( s2s<SqLhs::f, f, calc_t\>(lhs) % s2s<SqRhs::f, f, calc_t\>(rhs) ) |

**Example:**

```cpp
i16sq7<-200., 100.> sq1 = -150.0_i16sq7;
u16sq7<10., 100.> sq2 = 45.0_u16sq7;
auto sqMod = sq1 % sq2;  // i16sq7<-100., 100.>, real value -15.0
```

---
