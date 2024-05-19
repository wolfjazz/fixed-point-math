# Shift Operators

The shift operators facilitate shifting `Sq` instances left or right by a specified number of bits, leveraging integral constants (using the `_ic` literal) for compile-time evaluation. This ensures that the resolution and type constraints are maintained, while the limits and values are adjusted accordingly.

---

## Left Shift (<<)

Shifts the scaled value of an `Sq` instance `v` to the left by a specified number of bits, given as an integral constant (using the `_ic` literal). The result is a new `Sq` type with the same base type, adjusted limits, and the scaled value shifted.
Effectively, a left shift is similar to a multiplication by a power of 2.

**Constraints:**

The shift amount must be a non-negative integral constant.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | *Sq::base_t* |
| **f** | *Sq::f* |
| **realMin** | *( Sq::scaledMin << ic ) \* \(2^{-f}\)* |
| **realMax** | *( Sq::scaledMax << ic ) \* \(2^{-f}\)* |
| | |
| *value* | *v.value << ic* |

**Example:**

```cpp
i32sq16<-100., 200.> sq = 150.0_i32sq16;
auto sqShiftLeft = sq << 2_ic;  // i32sq16<-400., 800.>, real value 600.
```

---

## Right Shift (>>)

Shifts the scaled value of an `Sq` instance `v` to the right by a specified number of bits, given as an integral constant (using the `_ic` literal). The result is a new `Sq` type with the same base type, adjusted limits, and the scaled value shifted. Effectively, a right shift is similar to a division by a power of 2, with the result rounded towards \(\small-\infty\) to the nearest integer.

**Constraints:**

The shift amount must be a non-negative integral constant.

**Output:**

| `Sq` | |
|------|-|
| **base_t** | *Sq::base_t* |
| **f** | *Sq::f* |
| **realMin** | *( Sq::scaledMin >> ic ) \* \(2^{-f}\)* |
| **realMax** | *( Sq::scaledMax >> ic ) \* \(2^{-f}\)* |
| | |
| *value* | *v.value \>\> ic* |

**Example:**

```cpp
i16sq4<-200., 1000.> sq = -147.7_i16sq4;  // scaled: -2363
auto sqShiftRight = sq >> 3_ic;  // i16sq4<-25., 125.>, sc: -296, real: -18.5
```

---
