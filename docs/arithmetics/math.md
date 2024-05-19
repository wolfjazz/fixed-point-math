# Mathematical Functions

---

## Absolute (abs)

Returns the absolute value of an `Sq` instance `v`. Produces a new `Sq` instance with a proper unsigned base type and absolute limits, and the resultant absolute value.

**Constraints:**

If the type is signed, the corresponding minimum integral value must not be in the value range. For instance, if `int8_t` is the base type, a scaled value of -128 is not allowed.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | smallest unsigned integer fitting the resulting range, no smaller than the input |
| **f** | *Sq::f* |
| **realMin** | *0.0*, if 0 is in real range of source type, otherwise<br>*min( abs(Sq::realMin), abs(Sq::realMax) )*  |
| **realMax** | *max( abs(Sq::realMin), abs(Sq::realMax) )* |
| | |
| *value* | *\| v.value \|* |

**Example:**

```cpp
i16sq7<-100., 200.> sq = -75.0_i16sq7;
auto absValue = abs(sq);  // u16sq7<0., 200.>, real value 75.0
```

---

## Square (sqr)

Computes the square (\(x^2\)) of an `Sq` instance `v`. Produces a new `Sq` instance with a proper base type and squared limits, and the resultant squared value.  
During computation, an intermediate calculation type is used, which has twice the size and the sign of the source base type.

**Formula:**

\({x^2}_{real} \Longleftrightarrow {\left((x*2^f)*(x*2^f)*2^{-f} = x^2*2^f\right)}_{scaled}\)

**Output:**

| `Sq` | |
|-|-|
| **base_t** | *smallest integer fitting the resulting range, no smaller than int32_t;<br>signed if the common type of int32_t and the source base type is signed, otherwise unsigned* |
| **f** | *Sq::f* |
| **realMin** | *0.0*, if 0 is in real range of source type, otherwise<br>*min( Sq::realMin*Sq::realMin, Sq::realMax*Sq::realMax )*  |
| **realMax** | *max( Sq::realMin*Sq::realMin, Sq::realMax*Sq::realMax )* |
| | |
| *value* | *v.value \* v.value / 2^f* |

**Example:**

```cpp
i16sq7<-100., 200.> sq = -10.0_i16sq7;
auto squaredValue = sqr(sq);  // i32sq7<0., 40000.>, real value 100.0
```

---

## Square Root (sqrt)

Calculates the square root of an `Sq` instance `v`. The result is an instance of a new `Sq` type, with an appropriate base type, the roots of the source limits, and the resultant value root. The computation uses `uint64_t` as an intermediate calculation type, and the square root is computed using a binary search algorithm *isqrt* taken from *Hacker's Delight, 2nd ed.*

**Constraints:**

- The size of the source base type must be smaller than or equal to the size of `uint32_t`.
- The scaling `f` must be smaller than the number of digits in the source base type.
- `Sq::realMin` must be greater than or equal to 0, i.e. there must not be any negative values in the real value range.

**Formula:**

\({sqrt(x)}_{real} \Longleftrightarrow {\left(\left((x*2^f)*2^f\right)^{1/2} = x^{1/2}*2^f\right)}_{scaled}\)

**Output:**

| `Sq` | |
|-|-|
| **base_t** | *Sq::base_t* |
| **f** | *Sq::f* |
| **realMin** | *floor( sqrt(Sq::realMin) )* |
| **realMax** | *ceil( sqrt(Sq::realMax) )* |
| | |
| *value* | *isqrt( v.value \* 2^f )* |

**Example:**

```cpp
i16sq7<0., 200.> sq = 100.0_i16sq7;
auto sqrtValue = sqrt(sq);  // i16sq7<0., 15.>, real value 10.0
```

---

## Inverse Square Root (rsqrt)

Calculates the reciprocal of the square root of an `Sq` instance `v`, commonly used in graphics and physics calculations to improve performance. This results in a new `Sq` instance with an appropriate base type, the reverse square root of the limits, and the resultant value.

The computation uses `uint64_t` as an intermediate calculation type for the square root, which is computed using a binary search algorithm *isqrt* taken from *Hacker's Delight, 2nd ed.*

Note that the result will saturate at the maximum representable real value \(\small thMax\) if the runtime scaled value \(\small x*2^f\) is smaller than or equal to \(\small limit = 2^f / thMax^2\).

**Constraints:**

- The size of the source base type must be smaller than or equal to the size of `uint32_t`.
- The scaling `f` must be smaller than the number of digits in the source base type.
- `Sq::realMin` must be equal to or greater than the type's resolution, i.e. only scaled values >= 1 are permitted.

**Formula:**

\({sqrt^{-1}(x)}_{real} \Longleftrightarrow {\left(2^{2f}/\left((x*2^f)*2^f\right)^{1/2} = 2^f/x^{1/2}\right)}_{scaled}\)

**Output:**

| `Sq` | |
|-|-|
| **base_t** | *Sq::base_t* |
| **f** | *Sq::f* |
| **realMin** | *floor( rsqrt(Sq::realMax) )* |
| **realMax** | *min( thMax, ceil( rsqrt(Sq::realMin) ) )* ; *thMax* is the largest possible real value |
| | |
| *value* | *thMax \* 2^f* , if *v.value < limit*, else *2^2f / sqrt(v)* |

**Example:**

```cpp
i16sq7<1., 100.> sq = 25.0_i16sq7;
auto rsqrtValue = rsqrt(sq);  // i16sq7<0.0, 1.0>, real value 0.195313

auto sq2 = i8sq6<i8sq6<>::resolution, 1.>::fromScaled< 1 >();
auto rsqrtValue2 = rsqrt(sq2);  // i8sq6<0., 1.984375>, real value 1.984375
```

---

## Cube (cube)

Computes the cube (\(x^3\)) of an `Sq` instance.

**Constraints:**

- The real value range of the `Sq` instance must be within the cube root of the maximum representable value for the type to avoid overflow.

**Formula:**

\({x^3}_{real} \Longleftrightarrow {\left((x*2^f)*(x*2^f)*2^{-f}*(x*2^f)*2^{-f} = x^3*2^f\right)}_{scaled}\)

**Output:**

| `Result` | |
|----------|-|
| **Type** | `Sq` |
| **Value** | The cube of the `Sq` instance |

**Example:**

```cpp
i16sq7<-100., 100.> sq = 5.0_i16sq7;
auto cubeValue = cube(sq);  // i16sq7<-1000., 1000.>, real value 125.0
```

---

## Cube Root (cbrt)

Calculates the cube root of an `Sq` instance.

**Constraints:**

- None.

**Formula:**

\({cube(x)}_{real} \Longleftrightarrow {\left(\left((x*2^f)*2^f*2^f\right)^{1/3} = x^{1/3}*2^f\right)}_{scaled}\)

**Output:**

| `Result` | |
|----------|-|
| **Type** | `Sq` |
| **Value** | The cube root of the `Sq` instance |

**Example:**

```cpp
i16sq7<-1000., 1000.> sq = 125.0_i16sq7;
auto cbrtValue = cbrt(sq);  // i16sq7<-10., 10.>, real value 5.0
```

---

## Minimum (min)

Determines the minimum of two `Sq` instances.

**Constraints:**

- Both `Sq` instances must have the same base type and resolution.

**Output:**

| `Result` | |
|----------|-|
| **Type** | `Sq` |
| **Value** | The minimum of the two `Sq` instances |

**Example:**

```cpp
i16sq7<-100., 200.> sq1 = 50.0_i16sq7;
i16sq7<-100., 200.> sq2 = 150.0_i16sq7;
auto minValue = min(sq1, sq2);  // i16sq7<-100., 200.>, real value 50.0
```

---

## Maximum (max)

Determines the maximum of two `Sq` instances.

**Constraints:**

- Both `Sq` instances must have the same base type and resolution.

**Output:**

| `Result` | |
|----------|-|
| **Type** | `Sq` |
| **Value** | The maximum of the two `Sq` instances |

**Example:**

```cpp
i16sq7<-100., 200.> sq1 = 50.0_i16sq7;
i16sq7<-100., 200.> sq2 = 150.0_i16sq7;
auto maxValue = max(sq1, sq2);  // i16sq7<-100., 200.>, real value 150.0
```

---
