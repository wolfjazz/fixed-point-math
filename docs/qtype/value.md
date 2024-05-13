# Value Access

Within the `Q` class, the actual scaled value that is stored in runtime memory can be accessed through specific member functions. These functions provide direct and controlled access to both the scaled representation and the real-value representation of the `Q` type:

- **Accessing the Scaled Value:**
  The scaled value can be accessed using the `scaled()` member function. This function returns the value in its internal scaled format, which is useful for low-level operations or when interfacing with systems that require the scaled integer directly.
  ```cpp
  auto scaledVal = qValue.scaled();  // Access the internal scaled value
  ```

- **Accessing the Real Value:**
  To obtain the real value as a floating-point number, you can use the `real()` function. This method converts the value back to its double precision floating-point representation, primarily useful for debugging reasons. It's important to use this function cautiously due to potential floating-point inaccuracies and the computational overhead associated with floating-point operations.
  ```cpp
  double realVal = qValue.real();  // Get the real value as double
  ```

- **Accessing the Real Value as an Integer:**
  For accessing the real value in any integer format, use `real<typename>()`. This method converts the unscaled real value to the given integral type by truncating it according to C++ rules, discarding any fractional digits. This truncation must be handled with care to avoid unintended data loss, especially when the type has a large fractional part.
  ```cpp
  // Get the real value as an integer, truncating fractional digits
  int intRealVal = qValue.real<int>();
  ```
