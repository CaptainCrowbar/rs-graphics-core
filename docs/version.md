# Version Information

_[Core Graphics Library by Ross Smith](index.html)_

```c++
#include "rs-graphics-core/version.hpp"
namespace RS::Graphics::Core;
```

This header supplies library version information.

```c++
std::array<int, 3> version() noexcept;
```

Returns an array containing the major, minor, and patch version numbers.

```c++
std::string version_string();
```

Returns the version numbers as a string (e.g. `"1.23.456"`).
