# Core Graphics Library

By Ross Smith

_[GitHub repository](https://github.com/CaptainCrowbar/rs-graphics-core)_

## Overview

```c++
#include "rs-graphics-core.hpp"
namespace RS::Graphics::Core;
```

This library defines core graphics classes and functions that will be relevant
to 2D and 3D graphics, as well as related fields such as physics.

This is a headers-only library; there is nothing to build (unless you want to
run the unit tests), and no binaries to install. The CMake file includes an
`install` target to copy the headers into `/usr/local/include` or the
equivalent. Headers can be included individually as required, or the entire
library can be included using `"rs-graphics-core.hpp"`.

[My unit test library](https://github.com/CaptainCrowbar/rs-unit-test) is used
for this library's unit tests. This a header-only library, and is not
required if you only want to use this library without building its unit
tests.

## Index

* [Version information](version.html)
* Mathematical utilities
    * [General mathematical utilities](maths.html)
    * [Root finding](root-finding.html)
* Containers
    * [Linear interpolated map](linear-map.html)
    * [Multi-dimensional array](multi-array.html)
* Linear algebra
    * [Vector](vector.html)
    * [Matrix](matrix.html)
    * [Quaternion](quaternion.html)
    * [Transforms](transform.html)
    * [Geometric primitives](geometry.html)
* Procedural generation
    * [Pseudo-random noise](noise.html)
