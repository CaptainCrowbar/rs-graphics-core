# Colour Space

_[Core Graphics Library by Ross Smith](index.html)_

```c++
#include "rs-graphics-core/colour-space.hpp"
namespace RS::Graphics::Core;
```

## Contents

* TOC
{:toc}

## Colour space concept

### Requirements

`CS` is the colour space; `N` is the number of channels in this colour space
(not counting an alpha channel, which is handled separately); `NB` is the
number of channels in its base space.

```c++
using CS::base = [some colour space];
```

Another colour space from which this one is defined. For the `CIEXYZ` space,
the root of the colour space relationship tree, `CIEXYZ::base` is itself. For
all other colour space classes, the base must be another colour space that
has already been defined. Circular dependencies are not allowed.

```c++
static bool CS::is_polar;
```

True if the colour space uses cylindrical coordinates instead of Cartesian.
The polar coordinate is always assumed to be the first one.

```c++
static bool CS::is_unit;
```

True if in-gamut colour values are always within the unit cube (i.e. all
coordinates are in the range `[0,1]`).

```c++
static constexpr std::array<char,N> CS::channels;
```

This is a list of letters representing the colour channels. All elements must
be ASCII upper or lower case letters, with no duplicates. Channel IDs are
case sensitive. Capital A is reserved for the alpha channel and may not be
used here.

```c++
static Vector<T,N> CS::from_base(Vector<T,NB> colour);
```

Colour conversion function from the base space to this one.

```c++
static Vector<T,NB> CS::to_base(Vector<T,N> colour);
```

Colour conversion function from this space to its base space.

### Notes

The rest of this documentation assumes that the reader is familiar with the
basic concepts of colour theory. As a general rule of thumb, if you don't
know what colour space you should be using, you probably want `LinearRGB` for
internal calculations (such as colour blending or interpolation), and `sRGB`
for reading and writing image data. Your image I/O library may handle that
conversion automatically, or it may require you to convert the colour space
when reading or writing pixels.

As a simplification, I treat colour spaces as stateless. When the definition
of a colour space includes state, such as a choice of standard illuminant,
this will need to be hardcoded into the colour space class, with different
classes(probably template instantiations) for different illuminants. See the
`WorkingSpace` and `NonlinearSpace` templates below for examples.

Definitions of a colour space sometimes vary in the scaling of the
coordinates. Here I use the unit range for CIE XYZ and closely related spaces,
instead of the 0-100 range used in some sources, although I keep the 100-based
scale for CIE L\*a\*b\* and CIE L\*u\*v\* since that is used universally. For
polar spaces such as HSL and HSV, I use a unit scale (i.e. a fraction of a
circle) for the angular coordinate, instead of expressing it in radians or
degrees.

The term "working space" refers to the linear form of a nonlinear RGB space
such as sRGB, before the transfer function ("gamma") has been applied. The
base space of a nonlinear space should be the corresponding working space,
and the working space's own base space should normally be CIE XYZ.

The following conditions are not checked, but behaviour is undefined if a
colour space violates any of them:

1. `CS::base` must not be the same as `CS` (apart from the CIE XYZ special case).
2. The graph of colour spaces and their base spaces must be a directed acyclic graph, with no loops or disconnected subgraphs.
3. The channel list must be the correct length for the colour space's number of channels.
4. The channel list must contain only ASCII upper and lower case letters, excluding capital A.
5. The channel list must not contain any duplicates.

## Colour space classes

| Colour space       | Base space         | Polar?  | Unit?  | Description                               |
| ------------       | ----------         | ------  | -----  | -----------                               |
| `CIEXYZ`           | `CIEXYZ`           | No      | Yes    | CIE 1931 XYZ colour space                 |
| `CIExyY`           | `CIEXYZ`           | No      | Yes    | CIE 1931 xyY colour space                 |
| `CIELab`           | `CIEXYZ`           | No      | No     | CIE 1976 L\*a\*b\* colour space           |
| `CIELuv`           | `CIEXYZ`           | No      | No     | CIE 1976 L\*u\*v\* colour space           |
| `sRGB`             | `LinearRGB`        | No      | Yes    | Widely used sRGB standard colour space    |
| `LinearRGB`        | `CIEXYZ`           | No      | Yes    | Working space for sRGB                    |
| `AdobeRGB`         | `LinearAdobeRGB`   | No      | Yes    | Adobe RGB (1998) colour space             |
| `LinearAdobeRGB`   | `CIEXYZ`           | No      | Yes    | Working space for Adobe RGB               |
| `ProPhoto`         | `LinearProPhoto`   | No      | Yes    | ProPhoto (or ROMM RGB) colour space       |
| `LinearProPhoto`   | `CIEXYZ`           | No      | Yes    | Working space for ProPhoto                |
| `WideGamut`        | `LinearWideGamut`  | No      | Yes    | Adobe Wide Gamut (or opRGB) colour space  |
| `LinearWideGamut`  | `CIEXYZ`           | No      | Yes    | Working space for Wide Gamut              |
| `HSL`              | `LinearRGB`        | Yes     | Yes    | Polar transformation of linear RGB        |
| `HSV`              | `LinearRGB`        | Yes     | Yes    | Polar transformation of linear RGB        |

[Bruce Lindbloom's site](http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html)
is my main source for the RGB/XYZ matrices.



<!--

class CIEXYZ {
    using base = CIEXYZ;
    static constexpr bool is_polar = false;
    static constexpr bool is_unit = true;
    static constexpr std::array<char, 3> channels = {{ 'X', 'Y', 'Z' }};
    template <typename T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept { return colour; }
    template <typename T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept { return colour; }
};

class CIExyY {
    using base = CIEXYZ;
    static constexpr bool is_polar = false;
    static constexpr bool is_unit = true;
    static constexpr std::array<char, 3> channels = {{ 'x', 'y', 'Y' }};
    template <typename T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
    template <typename T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
};

class CIELab {
    using base = CIEXYZ;
    static constexpr bool is_polar = false;
    static constexpr bool is_unit = false;
    static constexpr std::array<char, 3> channels = {{ 'L', 'a', 'b' }};
    template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
    template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
};

class CIELuv {
    using base = CIEXYZ;
    static constexpr bool is_polar = false;
    static constexpr bool is_unit = false;
    static constexpr std::array<char, 3> channels = {{ 'L', 'u', 'v' }};
    template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
    template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
};

template <int64_t M00, int64_t M01, int64_t M02,
    int64_t M10, int64_t M11, int64_t M12,
    int64_t M20, int64_t M21, int64_t M22,
    int64_t Divisor>
class WorkingSpace {
    using base = CIEXYZ;
    static constexpr bool is_polar = false;
    static constexpr bool is_unit = true;
    static constexpr std::array<char, 3> channels = {{ 'R', 'G', 'B' }};
    template <typename T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
    template <typename T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
};

template <typename WorkingSpace, int64_t GammaNumerator, int64_t GammaDenominator>
class NonlinearSpace {
    using base = WorkingSpace;
    static constexpr bool is_polar = false;
    static constexpr bool is_unit = true;
    static constexpr std::array<char, 3> channels = {{ 'R', 'G', 'B' }};
    template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
    template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
};

using LinearRGB = WorkingSpace<
     4'124'564,  3'575'761,  1'804'375,
     2'126'729,  7'151'522,    721'750,
       193'339,  1'191'920,  9'503'041,
    10'000'000
>;

class sRGB {
    using base = LinearRGB;
    static constexpr bool is_polar = false;
    static constexpr bool is_unit = true;
    static constexpr std::array<char, 3> channels = {{ 'R', 'G', 'B' }};
    template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
    template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
};

using LinearAdobeRGB = WorkingSpace<
     5'767'309,  1'855'540,  1'881'852,
     2'973'769,  6'273'491,    752'741,
       270'343,    706'872,  9'911'085,
    10'000'000
>;

using AdobeRGB = NonlinearSpace<LinearAdobeRGB, 22, 10>;

using LinearProPhoto = WorkingSpace<
     7'976'749,  1'351'917,    313'534,
     2'880'402,  7'118'741,        857,
             0,          0,  8'252'100,
    10'000'000
>;

class ProPhoto {
    using base = LinearProPhoto;
    static constexpr bool is_polar = false;
    static constexpr bool is_unit = true;
    static constexpr std::array<char, 3> channels = {{ 'R', 'G', 'B' }};
    template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
    template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
};

using LinearWideGamut = WorkingSpace<
     7'161'046,  1'009'296,  1'471'858,
     2'581'874,  7'249'378,    168'748,
             0,    517'813,  7'734'287,
    10'000'000
>;

using WideGamut = NonlinearSpace<LinearWideGamut, 563, 256>;

class HSL {
    using base = LinearRGB;
    static constexpr bool is_polar = true;
    static constexpr bool is_unit = true;
    static constexpr std::array<char, 3> channels = {{ 'H', 'S', 'L' }};
    template <typename T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
    template <typename T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
};

class HSV {
    using base = LinearRGB;
    static constexpr bool is_polar = true;
    static constexpr bool is_unit = true;
    static constexpr std::array<char, 3> channels = {{ 'H', 'S', 'V' }};
    template <typename T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept {
    template <typename T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept {
};

## Conversion functions

template <typename CS1, typename CS2, typename T> Vector<T, CS2::channels.size()> convert_colour_space(Vector<T, int(CS1::channels.size())> colour) {

## Utility functions

template <typename CS, typename T, int N> constexpr bool is_colour_in_gamut(Vector<T, N> colour) noexcept {
template <typename CS, typename T, int N> constexpr void clamp_colour(Vector<T, N>& colour) noexcept {

-->
