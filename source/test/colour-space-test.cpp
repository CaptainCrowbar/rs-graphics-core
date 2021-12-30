#include "rs-graphics-core/colour-space.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-format/format.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <vector>

using namespace RS::Graphics::Core;
using namespace RS::Graphics::Core::Detail;
using namespace RS::Format;
using namespace RS::Format::Literals;

namespace {

    const auto fmt = "{0:f4}"_fmt;

    // These should all be within the sRGB gamut
    const std::vector<Double2> xys = {
        /**/           /**/           {0.30, 0.60},  /**/           /**/           /**/           /**/           /**/           /**/
        /**/           /**/           {0.30, 0.55},  {0.35, 0.55},  /**/           /**/           /**/           /**/           /**/
        /**/           /**/           {0.30, 0.50},  {0.35, 0.50},  {0.40, 0.50},  /**/           /**/           /**/           /**/
        /**/           /**/           {0.30, 0.45},  {0.35, 0.45},  {0.40, 0.45},  {0.45, 0.45},  /**/           /**/           /**/
        /**/           {0.25, 0.40},  {0.30, 0.40},  {0.35, 0.40},  {0.40, 0.40},  {0.45, 0.40},  {0.50, 0.40},  {0.55, 0.40},  /**/
        /**/           {0.25, 0.35},  {0.30, 0.35},  {0.35, 0.35},  {0.40, 0.35},  {0.45, 0.35},  {0.50, 0.35},  {0.55, 0.35},  {0.60, 0.35},
        /**/           {0.25, 0.30},  {0.30, 0.30},  {0.35, 0.30},  {0.40, 0.30},  {0.45, 0.30},  {0.50, 0.30},  {0.55, 0.30},  /**/
        /**/           {0.25, 0.25},  {0.30, 0.25},  {0.35, 0.25},  {0.40, 0.25},  {0.45, 0.25},  /**/           /**/           /**/
        {0.20, 0.20},  {0.25, 0.20},  {0.30, 0.20},  {0.35, 0.20},  {0.40, 0.20},  /**/           /**/           /**/           /**/
        {0.20, 0.15},  {0.25, 0.15},  {0.30, 0.15},  /**/           /**/           /**/           /**/           /**/           /**/
        {0.20, 0.10},  /**/           /**/           /**/           /**/           /**/           /**/           /**/           /**/
    };

    const auto xyzs = [] {
        std::vector<Double3> v;
        v.push_back({});
        for (int i = 5; i < 100; i += 5) {
            double Y = 0.01 * i;
            for (int j = 5; j < 100; j += 5) {
                double x = 0.01 * j;
                for (int k = 5; k < 100; k += 5) {
                    double y = 0.01 * k;
                    Double3 xyY = {x, y, Y};
                    auto xyz = CIExyY().to_base(xyY);
                    if (! is_colour_in_gamut<CIEXYZ>(xyz))
                        continue;
                    auto lrgb = LinearRGB().from_base(xyz);
                    if (! is_colour_in_gamut<LinearRGB>(lrgb))
                        continue;
                    auto srgb = sRGB().from_base(lrgb);
                    if (! is_colour_in_gamut<sRGB>(srgb))
                        continue;
                    v.push_back(xyz);
                }
            }
        }
        return v;
    }();

}

void test_rs_graphics_core_colour_space_srgb() {

    Double3 c, lin1, lin2, xyz2;

    for (auto xyz1: xyzs) {
        TRY(lin1 = LinearRGB().from_base(xyz1));
        TRY(c = sRGB().from_base(lin1));
        TEST(is_colour_in_gamut<sRGB>(c));
        TRY(lin2 = sRGB().to_base(c));
        TRY(xyz2 = LinearRGB().to_base(lin2));
        TEST_EQUAL(fmt(xyz2), fmt(xyz1));
    }

}

// void test_rs_graphics_core_colour_space_adobe_rgb() {

//     Double3 c, lin1, lin2, xyz2;

//     for (auto xyz1: xyzs) {
//         TRY(lin1 = LinearAdobeRGB().from_base(xyz1));
//         TRY(c = AdobeRGB().from_base(lin1));
//         TEST(is_colour_in_gamut<AdobeRGB>(c));
//         TRY(lin2 = AdobeRGB().to_base(c));
//         TRY(xyz2 = LinearAdobeRGB().to_base(lin2));
//         TEST_EQUAL(fmt(xyz2), fmt(xyz1));
//     }

// }

// void test_rs_graphics_core_colour_space_wide_gamut() {

//     Double3 lin1, lin2, wg, xyz2;
//     size_t in_gamut = 0;

//     for (auto xyz1: xyzs) {
//         TRY(lin1 = LinearWideGamut().from_base(xyz1));
//         if (is_colour_in_gamut<LinearWideGamut>(lin1)) {
//             ++in_gamut;
//             TRY(wg = WideGamut().from_base(lin1));
//             TRY(lin2 = WideGamut().to_base(wg));
//             TRY(xyz2 = LinearWideGamut().to_base(lin2));
//             TEST_EQUAL(fmt(xyz2), fmt(xyz1));
//         } else {
//             // std::cout << LinearRGB().from_base(xyz1) << " " << xyz1 << " " << lin1 << "\n";
//         }
//     }

//     // std::cout << in_gamut << " " << xyzs.size() << " " << (double(in_gamut) / xyzs.size()) << "\n";

// }
