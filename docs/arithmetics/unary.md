# Unary Operators

---

## Unary Plus (+)

The unary plus operator in the context of the `Sq` type simply copies the `Sq` variable `v`. Typically, the unary plus operator is used for integral promotion, but this concept does not apply to the `Sq` type. The inclusion of this operator is mainly for the sake of completeness, ensuring that the `Sq` type has a consistent set of unary operators.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | *Sq::base_t* |
| **f** | *Sq::f* |
| **realMin** | *Sq::realMin* |
| **realMax** | *Sq::realMax* |
| | |
| *value* | *v.value* |

**Example:**

```cpp
u32sq16<0., 10000.> sqVar = 5678.9_u32sq16;
auto sqVarCopy = +sqVar;  // Copies sqVar without any change in value and type
```

---

## Unary Minus (-)

The unary minus operator inverts the sign of the value `v` and the limits of the `Sq` input type. This transformation essentially mirrors the value range around the origin.

**Constraints:**

For a signed base type, the minimum integer must not be within the value range because its negation will cause an overflow, resulting in the same negative value. This is why the default real value range is symmetric around 0. For example, an `i8q2<>` has a default symmetric real value range of -31.75 to 31.75. Although a type like `i8q2<-32., 31.75>` can still be manually declared, the negation operator is not available for this type.

**Output:**

| `Sq` | |
|-|-|
| **base_t** | *Sq::base_t* |
| **f** | *Sq::f* |
| **realMin** | *-Sq::realMax* |
| **realMax** | *-Sq::realMin* |
| | |
| *value* | *-v.value* |

**Example:**

```cpp
i16sq7<-100., 200.> sqVar = 150.0_i16sq7;
auto sqVarInverse = -sqVar;  // i16sq7<-200., 100.>, real value -150.
```

---
