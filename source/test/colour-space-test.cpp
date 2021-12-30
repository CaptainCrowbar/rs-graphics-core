#include "rs-graphics-core/colour-space.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-format/format.hpp"
#include "rs-unit-test.hpp"
#include <vector>

using namespace RS::Graphics::Core;
using namespace RS::Graphics::Core::Detail;
using namespace RS::Format;
using namespace RS::Format::Literals;

namespace {

    const auto fmt = "{0:f4}"_fmt;

}

void test_rs_graphics_core_colour_space_ciexyz_vs_linear_rgb() {

    std::vector<Double3> rgbs = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0},
        {0.0, 1.0, 1.0},
        {1.0, 0.0, 0.0},
        {1.0, 0.0, 1.0},
        {1.0, 1.0, 0.0},
        {1.0, 1.0, 1.0},
    };

    Double3 rgb2, xyz;

    for (auto rgb1: rgbs) {
        TRY(xyz = LinearRGB::to_base(rgb1));
        TRY(rgb2 = LinearRGB::from_base(xyz));
        TEST(is_colour_in_gamut<CIEXYZ>(xyz));
        TEST_EQUAL(fmt(rgb2), fmt(rgb1));
    }

}
