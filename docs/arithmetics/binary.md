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

---

## Modulo (%)

---
