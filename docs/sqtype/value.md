# Value Access

The actual `Sq` value stored at runtime can be accessed through specific member functions. These functions provide controlled access to both the scaled representation and the real-value representation of the `Sq` type:

- **Accessing the Scaled Value:**
  The scaled value, which is the internally stored scaled integer, can be accessed using the `scaled()` member function. This is particularly useful for operations that require interaction with systems needing the integer directly or when performing low-level operations that depend on the scaled format.
  ```cpp
  auto scaledVal = sqVar.scaled();  // Retrieve the internal scaled value
  ```

- **Accessing the Real Value:**
  To retrieve the real value as a floating-point number, the `real()` function is used. This method converts the internal scaled integer back to its double precision floating-point representation. It is primarily useful for applications that require precise numerical outputs, such as for display or in high-level calculations. However, it's important to handle this value carefully due to the potential for floating-point inaccuracies and the computational overhead associated with floating-point operations.
  ```cpp
  double realVal = sqVar.real();  // Get the real value as double
  ```
  Similar to `Q`, the `real<int>()` variant can be used here to retrieve the integer part of the real value with the fractional part truncated. This truncation must be handled with care to avoid unintended data loss, especially when the type has a large fractional part.
