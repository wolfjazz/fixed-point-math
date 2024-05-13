# Rescaling and Casting

Rescaling and casting of `Sq` variables, compared to their counterparts in `Q` types, are considerably more restricted. The design of `Sq` types emphasizes compile-time checks and static value management, which inherently limits dynamic operations such as rescaling and casting. Here are the key aspects of rescaling and casting for `Sq` variables:

**Conditions for Rescaling:**
Rescaling is permitted only if the target `Sq` type has the same base type (`base_t`) and a different number of fractional bits (`f`). Importantly, the target type must also have a value range that is the same or wider than the source type to avoid potential overflows. The code does not compile if this is not the case. These measures ensure that the precision and range can be adjusted without altering the fundamental data type or exceeding the numerical capacity.

```cpp
i16sq4<-100., 100.> sqSource = 25.75_i16sq4;  // scaled: 412

// Safe rescaling via assignment to a target type with the same base type
// and a compatible real value range
i16sq6<-100., 200.> sqRescaled = sqSource;  // scaled: 1648, real: 25.75

// Basically the same, but a bit more explicit.
auto sqRescaled2 = i16sq6<-100., 200.>::fromSq( sqSource );  // scaled: 1648
```

**Restrictions on Casting:**
Casting between different base types requires an explicit `static_cast` to ensure that changes in data representation are handled correctly. The target type must have a value range that is the same or broader than the source type to ensure that no overflow errors occur during the casting process.

```cpp
i16sq4<-60., 60.> sqI16 = -56.7_i16sq4;  // scaled: -907, real: -56.6875

// Explicit cast required to a target type with a different base type
auto sqI32 = static_cast< i32sq3<-60., 60.> >( sqI16 );  // s: -453, r: 56.625
auto sqI8 = static_cast< i8sqm4<-60., 60.> >( sqI32 );   // s: -3, r: -48
```

*Note*: In contrast to the `Q` type, there is no `safe_cast` and no `force_cast` for the `Sq` type. Both functions would contradict the foundational concept of `Sq`, which is designed to ensure compile-time safety and immutability of values, preventing any changes that could lead to runtime check overhead, errors, or value inconsistencies.

**Compile-Time Overflow Checks:**
The `Sq` type enforces `Ovf::error` as its inherent - and only - overflow behavior. If the target type's value range is narrower than the source type's, the compiler will block the operation to prevent any risk of overflow, thus maintaining strict type safety.

```cpp
i32sq4<-100., 100.> sqSource = -78.1_i32sq4;

// Error - target value range is narrower
i32sq5<-50., 50.> sqRescaled = sqSource;

// Error - target value range is narrower
auto sqCast = static_cast< i16sq4<-50., 50.> >( sqSource );
```

**Using `Q` for Handling Overflows:**
Dynamic overflow handling, such as clamping, must be implemented through `Q` types since `Sq` does not support runtime overflow-checking behaviors. This approach is necessary when adjusting value ranges dynamically or when dealing with potential overflows.

```cpp
i32sq4<-100., 100.> sqSource = -78.1_i32sq4;  // scaled: -1249

// Handling overflows by clamping via Q type; scaled: -960, real: -60
auto sqClamped = i32q4<-60., 60.>::fromSq< Ovf::clamp >( sqSource ).toSq();
```

This technique provides precise control over the timing of overflow checks within the `Sq` scope. By using `Q` for intermediate calculations, developers can ensure that overflows are addressed exactly when needed.

*Additional Note*: There are only a few edge cases where overflow handling on `Sq` through `Q` is even needed. These typically arise when the `Sq` variable is constructed from a variable of a `Q` type that uses explicit `Ovf::allowed`. In such cases, the potential for overflow exists because the `Q` type permits it, and the value, which potentially is out of range, can be passed to `Sq` via the `toSq()` method. Crucially, when such out-of-range values are passed, the compiler will still assume that the value is within the specified range of the `Sq` type. This might be desired behavior for some edge case computations, where handling or ignoring overflow could be beneficial. This is precisely why `Ovf::allowed` exists — to enable these specific scenarios where standard overflow management practices are intentionally bypassed.
