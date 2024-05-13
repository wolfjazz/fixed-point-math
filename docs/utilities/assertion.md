# Static Assertion

The library defines four compile-time-only static assertion functions to ensure that `Q` or `Sq` types conform to expected specifications:

- **static_assert_base**: This function statically asserts whether the `base_t` of a given `Q` or `Sq` type matches the specified base type `Base`.
  ```cpp
  // definition in fpm:
  template< std::integral Base, class QSq >
  consteval void static_assert_base() noexcept { /* ... */ }

  // use as follows:
  using someType = u8q4<1., 10.>;
  auto someVariable = someType::fromReal< 5.5 >();

  fpm::static_assert_base< uint8_t, someType >();
  fpm::static_assert_base< uint8_t, decltype(someVariable) >();
  ```
- **static_assert_scale**: This function statically asserts that the scaling factor `f` of a given `Q` or `Sq` type is as specified.
  ```cpp
  // definition in fpm:
  template< scaling_t f, class QSq >
  consteval void static_assert_scale() noexcept { /* ... */ }

  // use as follows:
  using someType = u8q4<1., 10.>;
  auto someVariable = someType::fromReal< 5.5 >();

  fpm::static_assert_scale< 4, someType >();
  fpm::static_assert_scale< 4, decltype(someVariable) >();
  ```
- **static_assert_limits**: This function statically asserts that the real value range of a given `Q` or `Sq` type falls within the specified `min` and `max` values.
  ```cpp
  // definition in fpm:
  template< double min, double max, class QSq >
  consteval void static_assert_limits() noexcept { /* ... */ }

  // use as follows:
  using someType = u8q4<1., 10.>;
  auto someVariable = someType::fromReal< 5.5 >();

  fpm::static_assert_limits< 1., 10., someType >();
  fpm::static_assert_limits< 1., 10., decltype(someVariable) >();
  ```
- **static_assert_specs**: This comprehensive function statically asserts all critical properties (`base_t`, `f`, `min` and `max`) of a given `Q` or `Sq` type.
  ```cpp
  // definition in fpm:
  template< std::integral Base, scaling_t f, double min, double max, class QSq >
  consteval void static_assert_specs() noexcept { /* ... */ }

  // use as follows:
  using someType = u8q4<1., 10.>;
  auto someVariable = someType::fromReal< 5.5 >();

  fpm::static_assert_specs< uint8_t, 4, 1., 10., someType >();
  fpm::static_assert_specs< uint8_t, 4, 1., 10., decltype(someVariable) >();
  ```

These assertions are especially useful for verifying the type of intermediate results after calculations with `Sq` types. If the core types of an application are changed, these assertions might trigger a compilation error, even if the formulas and calculations still compile. This ensures that certain type properties remain consistent throughout the development process, and any changes to the underlying type definitions are immediately flagged.
