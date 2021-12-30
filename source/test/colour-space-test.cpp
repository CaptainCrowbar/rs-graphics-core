#include "rs-graphics-core/colour-space.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-format/format.hpp"
#include "rs-unit-test.hpp"
#include <vector>

using namespace RS::Graphics::Core;
using namespace RS::Format;
using namespace RS::Format::Literals;

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
        TRY(xyz = CIEXYZ::from_base(rgb1));
        TRY(rgb2 = LinearRGB::from_base(xyz));
        TEST(is_colour_in_gamut<CIEXYZ>(xyz));
        for (int i = 0; i < 3; ++i)
            TEST_NEAR(rgb2[i], rgb1[i], 1e-10);
    }

}
