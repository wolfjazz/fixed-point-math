# Construction

The `Q` class can be instantiated using various integral types and scaling factors to fit specific application requirements.

`Q` cannot be constructed directly from floating-point values at runtime to maintain type safety and prevent inadvertent data loss or misinterpretations. Instead, `Q` provides static constructor methods to create instances from real values, ensuring conversions are explicit and controlled:

- To construct a `Q` instance from a real constexpr value, you can use the static `fromReal< real >()` constructor function, which converts a floating-point number to the corresponding fixed-point representation. For example:
  ```cpp
  using fpm::Q;

  // Construct from real value
  auto a = Q<uint16_t, 8, 100., 200.>::fromReal< 155. >();
  ```
  The `auto` keyword is very handy here because it spares us from needing to specify the type twice, as the compiler deduces the type at compile time.
- Additionally, Q supports construction from scaled constexpr integer values through the static `fromScaled< scaled >()` constructor function, which takes an integer already scaled to the fixed-point format:
  ```cpp
  // Construct from scaled value
  auto b = Q<uint32_t, 8, 1000., 2000.>::fromScaled< 307200 >();
  auto c = Q<int32_t, 18, -10., 15.>::fromScaled< fpm::scaled<18>(12.) >();
  ```

- For creating instances of `Q` types with scaled integral runtime values, the constructor function  
`construct<ovfBxOvrd>( scaled )` is essential. It conducts a runtime overflow check as desired, adhering to the overflow behavior of the `Q` type, or, if specified, the behavior override provided via the template parameter `ovfBxOvrd`. This function ensures that values are instantiated safely, respecting the defined overflow strategies to prevent data corruption or unexpected behavior. It's important to note that this method can only be used with `scaled` integer values, as the runtime construction from double values is explicitly prohibited by the library's design to maintain type safety (no floats at runtime).
  ```cpp
  // Construct with runtime scaled value, clamped to target value range
  int32_t scaled = 123456;  // scaled value, q16; real=1.88379
  auto qVal = Q<int32_t, 16, -100., 100.>::construct<Ovf::clamp>(scaled);

  // Overflow override not necessary if target type inherently clamps
  auto qVal2 = Q<int32_t, 16, -100., 100., Ovf::clamp>::construct(scaled);
  ```

These construction methods provide precise control over how values are initialized in the `Q` type, ensuring adherence to the defined numerical ranges and overflow behaviors. Values are defined within the `< >` brackets at compile-time, and within the `( )` at runtime. It's important not to forget the `()` at the end of the static functions `Q<...>::fromReal<.>()` and `Q<...>::fromScaled<.>()`, as functions require parentheses to invoke them.

## Type Aliases

Construction of `Q` instances using type aliases looks as follows:
```cpp
using namespace fpm::types;  // predefined type alias templates

// user-defined type aliases with specific value ranges
using i32qm7_t = i32qm7<4e10, 5e10, Ovf::clamp>;
using i16q6_t = i16q6<>;

auto a = u16q8<100., 200.>::fromReal< 155. >();
auto c = i32q18<1000., 1500.>::fromScaled< 314572800 >();
auto d = i32qm7_t::fromReal< 4.567e10 >();
auto e = i16q6_t::fromReal< 444.4 >();

i32qm7_t f = 6.2e10_i32qm7;  // clamped to 5e10
i16q6_t g = -333.33_i16q6;
```

In the examples provided in this document, these aliases will mostly be used for construction, as they offer a more concise and familiar notation.
