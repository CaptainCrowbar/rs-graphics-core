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

    const auto rgbs = [] {
        std::vector<Double3> v;
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                for (int k = 0; k < 10; ++k)
                    v.push_back({0.1 * i, 0.1 * j, 0.1 * k});
        return v;
    }();

    const auto xyzs = [] {
        std::vector<Double3> v(rgbs.size());
        std::transform(rgbs.begin(), rgbs.end(), v.begin(), [] (auto& c) { return LinearRGB().to_base(c); });
        return v;
    }();

}

void test_rs_graphics_core_colour_space_linear_rgb() {

    Double3 rgb2, xyz;

    for (auto rgb1: rgbs) {
        TRY(xyz = LinearRGB().to_base(rgb1));
        TEST(is_colour_in_gamut<CIEXYZ>(xyz));
        TRY(rgb2 = LinearRGB().from_base(xyz));
        TEST_EQUAL(fmt(rgb2), fmt(rgb1));
        // printc("rgb {0:f2} => xyz {1:f4}", rgb1, xyz);
    }

}

void test_rs_graphics_core_colour_space_srgb() {

    Double3 rgb2, srgb;

    for (auto rgb1: rgbs) {
        TRY(srgb = sRGB().from_base(rgb1));
        TEST(is_colour_in_gamut<CIEXYZ>(srgb));
        TRY(rgb2 = sRGB().to_base(srgb));
        TEST_EQUAL(fmt(rgb2), fmt(rgb1));
        // printc("rgb {0:f2} => srgb {1:f4}", rgb1, srgb);
    }

}
