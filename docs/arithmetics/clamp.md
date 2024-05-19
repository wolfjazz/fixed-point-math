# Clamping Functions

Clamping functions are essential for maintaining values within specific ranges, ensuring efficient and safe mathematical computations with `Sq` instances. Each operation is optimized to leverage the static nature of `Sq` values, ensuring computations are both fast and reliable, with checks and balances performed at compile-time to prevent runtime errors and ensure type safety.

---

## Clamp to Minimum

Ensures the `Sq` instance does not fall below a specified minimum value. This function is available in two versions: one for runtime limits and one for compile-time limits.

### Runtime Clamp to Minimum

**Constraints:**

- The minimum value (`lo`) of type `SqLo` must be implicitly convertible to the type of the `Sq` instance being clamped (`v`). **Implicitly Convertible** from A to B means:
    - A and B are `Sq` types with the same base type.
    - A to B is scalable: the number of shifted bits is at most the number of bits in each type.
    - The real value range of B includes the real value range of A.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | Sq::base_t |
| **f** | Sq::f |
| **realMin** | SqLo::realMin |
| **realMax** | Sq::realMax |
| | |
| *value* | (v < lo) ? s2s<SqLo::f, f, base_t\>(lo.value) : v.value |

**Example:**

```cpp
i16sq7<-100., 200.> sq = 50.0_i16sq7;
i16sq7<50., 150.> sqLo = 100.0_i16sq7;
auto clampedMin = clampLower(sq, sqLo);  // i16sq7<50., 200.>, real: 100.
```

### Compile-Time Clamp to Minimum

Compile-time limits can be either real `double` values or `Sq` literals.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | Sq::base_t |
| **f** | Sq::f |
| **realMin** | realMin |
| **realMax** | Sq::realMax |
| | |
| *lo* | fpm::scaled<f, base_t\>(realMin) |
| *value* | (v < lo) ? lo : v |

**Example:**

```cpp
i16sq2<-100., 200.> sq = -100.0_i16sq2;
auto clampedMin = clampLower<-50.0>(sq);  // i16sq2<-50., 200.>, real: -50.
auto clampedMin2 = clampLower<20.25_i16q2>(sq);  // i16sq2<20.25, 200.>, real: 20.25
```

---

## Clamp to Maximum

Ensures the `Sq` instance does not exceed a specified maximum value. This function is available in two versions: one for runtime limits and one for compile-time limits.

### Runtime Clamp to Maximum

**Constraints:**

- The maximum value (`hi`) of type `SqHi` must be implicitly convertible to the type of the `Sq` instance being clamped (`v`). **Implicitly Convertible** from A to B means:
    - A and B are `Sq` types with the same base type.
    - A to B is scalable: the number of shifted bits is at most the number of bits in each type.
    - The real value range of B includes the real value range of A.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | Sq::base_t |
| **f** | Sq::f |
| **realMin** | Sq::realMin |
| **realMax** | SqHi::realMax |
| | |
| *value* | (hi < v) ? s2s<SqHi::f, f, base_t\>(hi.value) : v.value |

**Example:**

```cpp
i16sq7<-100., 200.> sq = 180.0_i16sq7;
i16sq7<-100., 150.> sqHi = 140.0_i16sq7;
auto clampedMax = clampUpper(sq, sqHi);  // i16sq7<-100., 150.>, real: 140.
```

### Compile-Time Clamp to Maximum

Compile-time limits can be either real `double` values or `Sq` literals.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | Sq::base_t |
| **f** | Sq::f |
| **realMin** | Sq::realMin |
| **realMax** | realMax |
| | |
| *hi* | fpm::scaled<f, base_t\>(realMax) |
| *value* | (hi < v) ? hi : v |

**Example:**

```cpp
i16sq1<-100., 200.> sq = 180.5_i16sq1;
auto clampedMax = clampUpper<160.0>(sq);  // i16sq7<-100., 160.>, real: 160.
auto clampedMax2 = clampUpper<10.5_i16sq1>(sq);  // i16sq7<-100., 10.5>, real: 10.5
```

---

## Clamp to Range

Restricts the `Sq` instance within a specified minimum and maximum range, combining both the clamp to minimum and clamp to maximum functionalities. This function is available in two versions: one for runtime limits and one for compile-time limits.

### Runtime Clamp to Range

**Constraints:**

The minimum value (`lo`) of type `SqLo` and maximum value (`hi`) of type `SqHi` must be implicitly convertible to the type of the `Sq` instance being clamped (`v`).
  **Implicitly Convertible** from A to B means:

  - A and B are `Sq` types with the same base type.
  - A to B is scalable: the number of shifted bits is at most the number of bits in each type.
  - The real value range of B includes the real value range of A.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | Sq::base_t |
| **f** | Sq::f |
| **realMin** | SqLo::realMin |
| **realMax** | SqHi::realMax |
| | |
| *value* | (v < lo) ? s2s<SqLo::f, f, base_t\>(lo.value)<br>: (hi < v) ? s2s<SqHi::f, f, base_t\>(hi.value) : v.value |

**Example:**

```cpp
i16sq7<-100., 200.> sq = -20.0_i16sq7;
i16sq7<-50., 200.> sqLo = 10.0_i16sq7;
i16sq7<-100., 180.> sqHi = 150.0_i16sq7;
auto clamped = clamp(sq, sqLo, sqHi);  // i16sq7<-50., 180.>, real: 10.
```

### Compile-Time Clamp to Range

Compile-time limits can be either real `double` values or `Sq` literals.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | Sq::base_t |
| **f** | Sq::f |
| **realMin** | realMin |
| **realMax** | realMax |
| | |
| *lo* | fpm::scaled<f, base_t\>(realMin) |
| *hi* | fpm::scaled<f, base_t\>(realMax) |
| *value* | (v < lo) ? lo : (hi < v) ? hi : v |

**Example:**

```cpp
i16sq7<-100., 200.> sq = -88.5_i16sq7;
auto clamped = clamp<-40., 200.>(sq);  // i16sq7<-40., 200.>, real: -40.
auto clamped2 = clamp<-20_i16sq7, 150_i16sq7>(sq);  // i16sq7<-20., 150.>, real: -20.
```

---
