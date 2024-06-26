# Getting Started

To begin using the Fixed-Point Math Library in your project, start by including the library header file from the *inc* directory in your source code (make sure that the *inc* directory is known to the compiler):

```cpp
#include <fpm.hpp>
```

Next, make use of the library's definitions and types by adding the following namespace directives to your source files:

```cpp
using namespace fpm::types;
using Ovf = fpm::Ovf;  // optional
```

This setup will allow you to easily access and utilize the various fixed-point types provided by the library in your applications. For clarity, some examples in this documentation may include (parts of) these lines of prefix explicitly. Moving forward, these lines will not be shown in any further examples within this documentation.
