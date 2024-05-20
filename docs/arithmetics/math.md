# Mathematical Functions

**Note**: Although multiplication and division are used for rescaling *value* in the following sections for clarity, the actual implementation relies on the `s2s` function. By default, this function uses shift operations unless multiplication/division was explicitly selected. See [Helpers](../utilities/helpers.md) for details.

---

## Absolute (abs)

Returns the absolute value of an `Sq` instance `v`. Produces a new `Sq` instance with a proper unsigned base type and absolute limits, and the resultant absolute value.

**Constraints:**

If the type is signed, the corresponding minimum integral value must not be in the value range. For instance, if `int8_t` is the base type, a scaled value of -128 is not allowed. This is because the absolute value of the minimum integer exceeds the value range of the signed type.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | *Sq::base_t* |
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
| **realMin** | *0.0*, if 0 is in real range of source type, otherwise<br>*min( Sq::realMin\*Sq::realMin, Sq::realMax\*Sq::realMax )*  |
| **realMax** | *max( Sq::realMin\*Sq::realMin, Sq::realMax\*Sq::realMax )* |
| | |
| *value* | *v.value \* v.value / 2^f* |

**Example:**

```cpp
i16sq7<-100., 200.> sq = -10.0_i16sq7;
auto squaredValue = sqr(sq);  // i32sq7<0., 40000.>, real value 100.0
```

---

## Square Root (sqrt)

Calculates the square root of an `Sq` instance `v`. The result is an instance of a new `Sq` type, with an appropriate base type, the roots of the source limits, and the resultant value root. The computation uses `uint64_t` as an intermediate calculation type, and the square root of the value is computed using a binary search algorithm `isqrt` taken from *Hacker's Delight, 2nd ed.*

The root of each limit is approximated at compile-time using the inverse of the famous inverse square root implementation from *Quake III Arena*, optimized for `uint64_t` and utilizing the Newton-Raphson method. The new lower limit is then rounded down and the new upper limit is rounded up to correct for (most) approximation inaccuracies and to ensure clean limits in the resulting type.

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

The runtime implementation uses the `sqrt` function for the square root, which employs `uint64_t` as an intermediate calculation type and computes the root using the `isqrt` binary search algorithm from *Hacker's Delight, 2nd ed.* The inverse root of each limit is approximated at compile-time using the famous inverse square root implementation from *Quake III Arena*, optimized for `uint64_t` and utilizing the Newton-Raphson method. The new lower limit is then rounded down and the new upper limit is rounded up to correct for (most) approximation inaccuracies and to ensure clean limits in the resulting type.

Note that the result will saturate at the maximum representable real value \(\small thMax\) if the runtime scaled value \(\small x*2^f\) is smaller than or equal to \(\small limit = 2^f / thMax^2\).

**Constraints:**

- The size of the source base type must be smaller than or equal to the size of `uint32_t`.
- The scaling `f` must be smaller than the number of digits in the source base type.
- To avoid division by 0, `Sq::realMin` must be equal to or greater than the type's resolution, i.e. only scaled values >= 1 are permitted.

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

Computes the cube (\(x^3\)) of an `Sq` instance `v`. Produces a new `Sq` instance with a proper base type and cubedc limits, and the resultant cubed value.  
During computation, an intermediate calculation type is used, which has twice the size and the sign of the source base type.

**Formula:**

\({x^3}_{real} \Longleftrightarrow {\left((x*2^f)*(x*2^f)*2^{-f}*(x*2^f)*2^{-f} = x^3*2^f\right)}_{scaled}\)

**Output:**

| `Sq` | <span style="font-weight:normal;font-size:8pt">*l1=Sq::realMin\*Sq::realMin\*Sq::realMin, l2=Sq::realMin\*Sq::realMin\*Sq::realMax,<br>l3=Sq::realMin\*Sq::realMax\*Sq::realMax, l4=Sq::realMax\*Sq::realMax\*Sq::realMax*</span> |
|-|-|
| **base_t** | *smallest integer fitting the resulting range, no smaller than int32_t;<br>signed if the common type of int32_t and the source base type is signed, otherwise unsigned* |
| **f** | *Sq::f* |
| **realMin** | *min( l1, l2, l3, l4 )*  |
| **realMax** | *max( l1, l2, l3, l4 )* |
| | |
| *value* | *sqr(v) \* v.value / 2^f* |

**Example:**

```cpp
i16sq7<-100., 100.> sq = -5.0_i16sq7;
auto cubeValue = cube(sq);  // i32sq7<-1e6, 1e6>, real value -125.0
```

---

## Cube Root (cbrt)

Calculates the cube root of an `Sq` instance `v`. The result is an instance of a new `Sq` type, with the same base type, the roots of the source limits, and the resultant value root. The computation uses `uint64_t` as an intermediate calculation type, and the cube root of the value is computed using an algorithm based on `icbrt` from *Hacker's Delight, 2nd ed.*

The compile-time cube root of each limit is approximated using a binary search algorithm, with a target accuracy of `1e-6` but at most 200 iterations. The new lower limit is then rounded down and the new upper limit is rounded up to correct for (most) approximation inaccuracies and to ensure clean limits in the resulting type.

**Constraints:**

- The size of the source base type must be smaller than or equal to the size of `uint32_t`.
- The scaling `f` must be smaller than or equal to 16.
- `Sq::realMin` must be greater than or equal to 0, i.e. there must not be any negative values in the real value range.

**Formula:**

\({cbrt(x)}_{real} \Longleftrightarrow {\left(\left((x*2^f)*2^f*2^f\right)^{1/3} = x^{1/3}*2^f\right)}_{scaled}\)

**Output:**

| `Sq` | |
|-|-|
| **base_t** | *Sq::base_t* |
| **f** | *Sq::f* |
| **realMin** | *floor( cbrt(Sq::realMin) )* |
| **realMax** | *ceil( cbrt(Sq::realMax) )* |
| | |
| *value* | *icbrt( v.value \* 2^f \* 2^f )* |

**Example:**

```cpp
i16sq7<0., 200.> sq = 125.0_i16sq7;
auto cbrtValue = cbrt(sq);  // i16sq7<0., 6.>, real value 5.0
```

---

## Minimum (min)

Determines the minimum of two similar `Sq` instances `v1` and `v2`. Produces a new `Sq` instance with the same base type and scaling, the minimum limits and the resultant minimum value.

**Constraints:**

Both `Sq` instances must be similar, i.e. have the same base type and scaling.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | *Sq1::base_t* |
| **f** | *Sq1::f* |
| **realMin** | *min( Sq1::realMin, Sq2::realMin )* |
| **realMax** | *min( Sq1::realMax, Sq2::realMax )* |
| | |
| *value* | *(v1.value > v2.value) ? v2.value : v1.value* |

**Example:**

```cpp
i16sq7<-100., 200.> sq1 = 50.0_i16sq7;
i16sq7<-250., 250.> sq2 = 150.0_i16sq7;
auto minValue = min(sq1, sq2);  // i16sq7<-250., 200.>, real value 50.0
```

---

## Maximum (max)

Determines the maximum of two similar `Sq` instances `v1` and `v2`. Produces a new `Sq` instance with the same base type and scaling, the maximum limits and the resultant maximum value.

**Constraints:**

Both `Sq` instances must be similar, i.e. have the same base type and scaling.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | *Sq1::base_t* |
| **f** | *Sq1::f* |
| **realMin** | *max( Sq1::realMin, Sq2::realMin )* |
| **realMax** | *max( Sq1::realMax, Sq2::realMax )* |
| | |
| *value* | *(v1.value < v2.value) ? v2.value : v1.value* |

**Example:**

```cpp
i16sq7<-100., 200.> sq1 = 50.0_i16sq7;
i16sq7<-200., 250.> sq2 = 150.0_i16sq7;
auto maxValue = max(sq1, sq2);  // i16sq7<-100., 250.>, real value 150.0
```

---
