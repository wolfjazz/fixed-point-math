# Unary Operators

---

## Unary Plus (+)

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

---

## Unary Minus (-)

The unary minus operator inverts the sign of the value and the limits of the `Sq` input type. This transformation essentially mirrors the value range around the origin.

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

---
