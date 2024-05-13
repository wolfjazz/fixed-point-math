# Overflow Behaviors

The `Q` type provides a robust handling of overflow scenarios through different behaviors that can be specified during the type definition. Understanding and selecting the appropriate overflow behavior is critical to ensure that your application handles edge cases in a predictable and controlled manner. The library currently implements four types of overflow behaviors (from strictest to most lenient):

- **Error (`Ovf::error`)**: This behavior causes a compiler error if overflow is possible. It is the strictest behavior and the **default setting**, ensuring that potential overflow scenarios are addressed during development rather than at runtime.

- **Assert (`Ovf::assert`)**: This overflow behavior triggers a runtime assertion and calls the application-defined `ovfAssertTrap()` function if overflow occurs. This is useful for debugging and development phases where catching errors immediately is crucial.

- **Clamp (`Ovf::clamp`)**: With this behavior, values that would normally overflow are clamped at runtime to the maximum or minimum value within the range defined for the `Q` type. This prevents overflow while still allowing the application to continue running.

- **Unchecked (`Ovf::unchecked`), Allowed (`Ovf::allowed`)**: This setting disables overflow checking entirely. It allows the value to wrap around according to the standard behavior of the underlying data type. "Unchecked" and "Allowed" refer to the same behavior, however, the choice of term may fit better semantically depending on the context or usage within specific parts of your application.

Choosing the right overflow behavior depends on your application’s requirements for safety, debugging, and performance. It may even vary between different builds if corresponding declarations are used.
