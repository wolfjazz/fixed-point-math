# Casting

In the `Q` type framework, casting between different `Q` types with varying base types necessitates explicit type conversion. The simplest form of casting involves using `static_cast< QTo >( qFrom )`, which is straightforward when scaling adjustments are feasible and the target value range is either the same or wider than the source's, and if the target type's overflow behavior is the same or less strict.

```cpp
auto sourceValue = i32q10<0., 1500.>::fromReal< 1024. >();

// cast i32q10 to unsigned u32q12
auto cast1 = static_cast< u32q12<0., 1500.> >(sourceValue);

// cast i32q10 to i16q4, explicitly dropping any overflow check in target type
// (not recommended though, target type will drop any overflow check this way)
auto cast2 = static_cast< i16q4<0., 1030., Ovf::unchecked> >(sourceValue);
```

However, if the target range is narrower and the target type incorporates overflow checks, or if the target type's overflow behavior is more restrictive than that of the source, `static_cast` may not be sufficient. In cases where overflow could occur, and unless the target type handles overflow with `Ovf::clamp` or `Ovf::assert` when constructed, a simple `static_cast` will not properly address potential overflow issues and thus not compile.

To manage potential overflow effectively, `Q` provides three specialized casting functions:

- `static_q_cast< QTo, ovfBxOvrd >( qFrom )`: This function applies the overflow handling as specified by the target type or uses the overridden overflow behavior if provided. It ensures that the conversion adheres to the designated overflow protocols, **performing runtime checks only when necessary** from a static point of view.

- `safe_q_cast< QTo, ovfBxOvrd >( qFrom )`: This function **performs a runtime overflow check regardless of whether it is strictly necessary**. It requires that the overflow behavior is set to a runtime check `Ovf::clamp` or `Ovf::assert`, and does not permit `Ovf::unchecked` and the compile-time check `Ovf::error`. This method is designed for scenarios where ensuring data integrity is critical, and overflow must be actively managed.

- `force_q_cast< QTo >( qFrom )`: This casting function **performs no checks and simply reinterprets the scaled source value**, constructing the specified `ToQ` type around it, potentially truncating bit information. This is essentially a forced value reinterpretation and should be used with caution, as it bypasses all overflow and range checks as well as rescaling. This method is suitable for scenarios where the developer is certain of the data integrity and the applicability of the conversion.

These casting functions provide robust tools for managing different fixed-point types, ensuring that conversions are both safe and efficient, depending on the operational requirements and data integrity needs.

```cpp
auto value = i32q10<0., 1500.>::fromReal< 1234. >();

// cast value to unsigned u32q12 (identical to static_cast)
auto cast1 = static_q_cast< u32q12<0., 1500.> >( value );  // 1234.

// cast value to i16q4 with narrower range, overflow check is dropped explicitly
// --> value intentionally is out of range
auto cast2 = static_q_cast< i16q4<0., 1000.>, Ovf::allowed >( value );  // 1234.

// static-cast cast2 (potentially out of range) to a wider range
// note: no overflow checks are performed (wider range, identical ovf behavior)
auto c3static = static_q_cast< i16q4<-100., 1100.> >( cast2 );  // 1234.

// safe-cast of cast2 is needed to really catch and clamp the out-of-range value
auto c3safe = safe_q_cast< i16q4<-100., 1100.>, Ovf::clamp >( cast2 );  // 1100.

// force-cast cast2 (i16q4) into a u8q1
// --> u8q1<0., 100.>::fromScaled< cast2.scaled() % 256 >() = 32|scaled, 16|real
auto forced = force_q_cast< u8q1<0., 100.> >( cast2 );  // 16.
```

## Range Clamping

The `Q` type framework includes a specialized feature known as "range clamping" to address the nuances of casting between differently signed types of different sizes. This feature ensures that conversions between signed and unsigned types adhere to intuitive expectations regarding value ranges:

- **From Signed to Unsigned**: When a `Q` type with a signed base type is cast to a `Q` type with an unsigned base type of the same or smaller size, any values in the negative range of the signed type are clamped to the lower limit of the unsigned type. This clamping is crucial to avoid underflow and to ensure values remain within the legitimate range of the unsigned type.
- **From Unsigned to Signed**: Similarly, when a `Q` type with an unsigned base type is cast to a `Q` type with a signed base type of the same or smaller size, values that fall in the upper half of the unsigned range are clamped to the upper limit of the signed type. This measure prevents potential overflow, ensuring that the values do not exceed the maximum positive value of the signed type.
- **From Larger Signed Type to Smaller Signed Type**: When casting from a larger signed `Q` type to a smaller signed type, clamping adjusts any values that exceed the storage capabilities of the smaller type to the nearest boundary. This is crucial for preventing data overflow or underflow, helping maintain the integrity of the data.
- **From Larger Unsigned Type to Smaller Unsigned Type**: In cases where a larger unsigned `Q` type is cast to a smaller unsigned type, the values that exceed the maximum representable value of the smaller type are clamped to this maximum. This adaptation is essential for preserving data correctness by preventing overflow.

```cpp
auto iN = i32q10<-1300., 1300.>::fromReal< -1234. >();
auto iP = i32q10<-1300., 1300.>::fromReal< 1234. >();
auto uP = u16q8<0., 255.>::fromReal< 234. >();

// cast negative signed i32q10 to unsigned u32q5
auto iN2u = static_q_cast< u32q5<0., 3000.>, Ovf::clamp >( iN );  // 0.

// cast positive signed i32q10 to smaller unsigned u16q8
auto iP2u = static_q_cast< u16q8<0., 255.>, Ovf::clamp >( iP );  // 255.

// cast negative signed i32q10 to signed i16q2 with narrower value range
auto iN2i = static_q_cast< i16q2<-1000., -0.>, Ovf::clamp >( iN );  // -1000.

// cast positive signed i32q10 to signed i8q1
auto iP2i = static_q_cast< i8q1<-60., 60.>, Ovf::clamp >( iP );  // 60.

// cast unsigned u16q8 to signed i8q1
auto u2i = static_q_cast< i8q1<-60., 60.>, Ovf::clamp >( uP );  // 60.

// cast unsigned u16q8 to unsigned u8q2 with narrower value range
auto u2u = static_q_cast< u8q2<0., 30.>, Ovf::clamp >( uP );  // 30.
```

These adjustments are driven by internal logic designed to mirror what would typically be expected from the real values during such type conversions. By incorporating this feature, the framework helps preserve data integrity and provides a logical, safe transition between signed and unsigned `Q` types, reflecting the expected behavior of real-world values.

### How Does Range Clamping Work?

The process of range clamping in the `Q` type framework is designed to handle conversions between types of different sizes and signs meticulously to avoid data loss and ensure integrity. Here's a detailed breakdown of how this process works:

1. **Rescaling the Base Value**: Initially, the base value of the source `Q` type is rescaled in a `scale_t`, which has at least twice the size of the target type but is not smaller than the size of the source type, and retains the same sign as the source `Q` type. This step is crucial as it preserves the sign bit during the scaling process, particularly important when the source type is signed and the operation might involve a reduction in size (down-cast).

2. **Casting to a Transitional Type**: The scaled value is then cast to a transitional `cast_t`, which has twice the size and the sign of the target `Q` type. This intermediary step ensures that the value can be adjusted in a way that conforms to the sign characteristics of the target type, facilitating a smoother transition and accurate clamping in subsequent steps.

3. **Clamping and Final Adjustment**: In the final stage, the value now represented in `cast_t` allows for proper clamping. For instance, if the original value was negative and the target type is unsigned, the value falls within the upper half of the value range of `cast_t`. This positioning is key as it indicates that the value requires clamping to the lower limit of the unsigned target type to be clamped intuitively. Conversely, if the original value was positive and fits into the value range of the target type, the cast value will fit and not be clamped. The larger intermediate type (`cast_t`) provides the necessary headroom to accommodate these adjustments without overflow or underflow, ensuring that the final value is accurate and within the expected range.
