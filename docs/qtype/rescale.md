# Rescaling

In situations where the base type remains the same but the fractional part `f` differs between two `Q` types, rescaling is necessary. This process is referred to as "up-scaling" when `f` increases, and "down-scaling" when `f` decreases.

- **Up-Scaling**: Up-scaling is achieved by **multiplying** the source value by \(2^{to-from}\), effectively increasing the number of fractional bits to fit the higher resolution of the target type. While this process is technically lossless because no data is discarded, it doesn't enhance the actual resolution or detail of the original data; the additional fractional bits are essentially filled with zeros.

- **Down-Scaling**: Down-scaling reduces the number of fractional bits by **dividing** the source value by \(2^{from-to}\). This operation is inherently lossy as it involves discarding the least significant bits of the fractional part. Consequently, the precision of the original value is reduced to match the lower resolution of the target type, resulting in a loss of detail that reflects the decreased fractional granularity.

To rescale a `Q` type to another with the same base type but different `f`, you can use simple assignment, or, if necessary, the `QTo::fromQ<ovfBxOvrd>( qFrom )` constructor function, both of which handle the scaling adjustments internally:

```cpp
auto sourceValue = i32q10<-500., 1500.>::fromReal< 1024. >();

// Rescale from f=10 to f=8 and a wider value range via assignment
i32q8<-510., 1500.> targetValue1 = sourceValue;  // automatically rescaled

// Rescale from f=10 to f=8 and clamp to the narrower value range via fromQ<>()
auto targetValue2 = i32q8<-450., 1450.>::fromQ<Ovf::clamp>( sourceValue );
```

Overflow checks during scaling adjustments are unnecessary if the real value range of the target type is the same as or wider than that of the source type, and if the overflow behavior of the target type is the same or less strict than that of the source type. Under these conditions, values can be directly assigned.

However, if the target value range is narrower than the source's, or if the target type implements a stricter overflow behavior than the source type, you may need to use the `fromQ<ovfBx>(.)` function to specify a different overflow behavior. Typically, the value is clamped to fit within the target type's range, ensuring correct handling of potential overflow situations without causing compilation errors or runtime assertions. This adjustment does not permanently change the overflow behavior of the target type but applies the specified overflow handling only for the conversion to the target type.

```cpp
using source_t = i32q10<-500., 1500.>;   // i32q10, Ovf::error
using overflow_t = i32q10<-500., 1500., Ovf::allowed>;
using coarser_t = i32q8<-500., 1500.>;   // i32q8, same value range as source_t
using wider_t = i32q8<-600., 1600.>;     // i32q8, wider value range
using narrower_t = i32q8<-400., 1500.>;  // i32q8, narrower value range

auto source = source_t::fromReal< 1024.7 >();
auto ovfSrc = overflow_t::fromReal< -555.5 >();  // value out of range

/* same value range: direct assignment can be used if ovf behavior allows it */
coarser_t a1 = source;                        // ok
auto a2 = coarser_t::fromQ( source );         // also ok
// coarser_t a3 = ovfSrc;                     // error (stricter ovf behavior)
auto a4 = coarser_t::fromQ<Ovf::clamp>( ovfSrc );  // ok (runtime clamp needed)
i32q8<-500., 1500., Ovf::clamp> a5 = ovfSrc;  // ok (clamping target)

/* wider value range: direct assignment can be used if ovf behavior allows it */
wider_t b1 = source;                             // ok
auto b2 = wider_t::fromQ( source );              // also ok
// wider_t b3 = ovfSrc;                          // error (stricter ovf beh.)
auto b4 = wider_t::fromQ<Ovf::clamp>( ovfSrc );  // ok (runtime clamp needed)
i32q8<-600., 1600., Ovf::clamp> b5 = ovfSrc;     // ok (clamping target)

/* narrower range: fromQ<>() is required unless target type checks anyway */
// narrower_t c1 = source;  // error (narrower range)
auto c2 = narrower_t::fromQ<Ovf::clamp>( source );  // ok
auto c3 = narrower_t::fromQ<Ovf::clamp>( ovfSrc );  // ok
i32q8<-400., 1500., Ovf::clamp> c4 = ovfSrc;        // ok (clamping target)
i32q8<-400., 1500., Ovf::unchecked> c5 = ovfSrc;    // ok (check discarded)
```
