# Scaling

One of the foundational declarations is the definition of `scaling_t` as an alias for `int`. The decision to make `scaling_t` signed is strategic because the framework needs to support both positive and negative fractional values:

- **Positive Scaling Values**: These are used to represent fractional parts, where positive scaling factors enhance the precision of the fractional component by specifying how many bits are dedicated to values below the integer point.
- **Negative Scaling Values**: Negative scaling is used to represent larger numbers than those typically representable with the given bits of the used integral base type. By scaling the real number by \(2^f\), where \(f\) is negative, large numbers can be represented at the cost of reducing the precision of the integral part of the number by \(2^{|f|}\), effectively balancing the range and precision according to specific needs.

This flexibility ensures that the framework can adapt to a variety of numerical ranges and precision requirements, making it highly versatile for different applications.
