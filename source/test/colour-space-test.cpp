#include "rs-graphics-core/colour-space.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include "test/vector-test.hpp"
#include <algorithm>
#include <vector>

using namespace RS::Graphics::Core;
using namespace RS::Graphics::Core::Detail;

namespace {

    // Source: https://www.easyrgb.com/en/convert.php

    // Colours are selected to be in gamut for all colour spaces of interest.
    // Differences in 4th-5th decimal place are probably due to differences
    // in matrix approximations between this source and Lindbloom.

    struct Sample {
        Double3 sRGB;
        Double3 AdobeRGB;
        Double3 HSL;
        Double3 HSV;
        Double3 CIEXYZ;
        Double3 CIExyY;
        Double3 CIELab;
        Double3 CIELuv;
    };

    const std::vector<Sample> samples = {
        // sRGB             AdobeRGB                    HSL                         HSV                         CIEXYZ                      CIExyY                      CIELab                    CIELuv
        {{0.00,0.00,0.00},  {0.00000,0.00000,0.00000},  {0.00000,0.00000,0.00000},  {0.00000,0.00000,0.00000},  {0.00000,0.00000,0.00000},  {0.00000,0.00000,0.00000},  {0.000,0.000,0.000},      {0.000,0.000,0.000}},
        {{0.25,0.50,0.75},  {0.34673,0.49610,0.73614},  {0.58333,0.50000,0.50000},  {0.58333,0.66667,0.75000},  {0.19180,0.20160,0.52305},  {0.20929,0.21998,0.20160},  {52.018,0.093,-39.363},   {52.018,-25.362,-60.283}},
        {{0.25,0.75,0.50},  {0.46453,0.74442,0.50928},  {0.41667,0.50000,0.50000},  {0.41667,0.66667,0.75000},  {0.24645,0.39995,0.26667},  {0.26991,0.43803,0.39995},  {69.466,-49.554,22.224},  {69.466,-52.311,38.425}},
        {{0.50,0.25,0.75},  {0.44385,0.25813,0.73171},  {0.75000,0.50000,0.50000},  {0.75000,0.66667,0.75000},  {0.20076,0.11962,0.50676},  {0.24271,0.14462,0.11962},  {41.155,51.411,-56.449},  {41.155,16.371,-86.719}},
        {{0.50,0.75,0.25},  {0.58009,0.74442,0.29901},  {0.25000,0.50000,0.50000},  {0.25000,0.66667,0.75000},  {0.28430,0.42288,0.11477},  {0.34589,0.51448,0.42288},  {71.069,-40.908,55.644},  {71.069,-32.079,71.662}},
        {{0.50,0.25,0.75},  {0.44385,0.25813,0.73171},  {0.75000,0.50000,0.50000},  {0.75000,0.66667,0.75000},  {0.20076,0.11962,0.50676},  {0.24271,0.14462,0.11962},  {41.155,51.411,-56.449},  {41.155,16.371,-86.719}},
        {{0.75,0.25,0.50},  {0.65033,0.25813,0.48896},  {0.91667,0.50000,0.50000},  {0.91667,0.66667,0.75000},  {0.27233,0.16296,0.21957},  {0.41586,0.24885,0.16296},  {47.360,56.522,-8.041},   {47.360,76.888,-20.831}},
        {{0.75,0.50,0.25},  {0.68464,0.49610,0.27311},  {0.08333,0.50000,0.50000},  {0.08333,0.66667,0.75000},  {0.30123,0.26787,0.08396},  {0.46126,0.41017,0.26787},  {58.777,18.585,43.798},   {58.777,50.243,45.130}},
        {{0.85,0.85,0.85},  {0.84589,0.84589,0.84589},  {0.00000,0.00000,0.85000},  {0.00000,0.00000,0.85000},  {0.65779,0.69207,0.75355},  {0.31273,0.32902,0.69207},  {86.607,0.000,-0.000},    {86.607,-0.000,-0.000}},
    };

}

void test_rs_graphics_core_colour_space_ciexyy() {

    Double3 cie, colour;

    for (auto& sample: samples) {
        TRY(colour = CIExyY().from_base(sample.CIEXYZ));
        TEST_VECTORS(colour, sample.CIExyY, 1e-4);
        TRY(cie = CIExyY().to_base(sample.CIExyY));
        TEST_VECTORS(cie, sample.CIEXYZ, 1e-4);
    }

}

void test_rs_graphics_core_colour_space_srgb() {

    Double3 cie, linear, nonlinear;

    for (auto& sample: samples) {
        TRY(linear = LinearRGB().from_base(sample.CIEXYZ));
        TRY(nonlinear = sRGB().from_base(linear));
        TEST_VECTORS(nonlinear, sample.sRGB, 1e-4);
        TRY(linear = sRGB().to_base(sample.sRGB));
        TRY(cie = LinearRGB().to_base(linear));
        TEST_VECTORS(cie, sample.CIEXYZ, 1e-4);
    }

}

void test_rs_graphics_core_colour_space_adobe_rgb() {

    Double3 cie, linear, nonlinear;

    for (auto& sample: samples) {
        TRY(linear = LinearAdobeRGB().from_base(sample.CIEXYZ));
        TRY(nonlinear = AdobeRGB().from_base(linear));
        TEST_VECTORS(nonlinear, sample.AdobeRGB, 1e-3);
        TRY(linear = AdobeRGB().to_base(sample.AdobeRGB));
        TRY(cie = LinearAdobeRGB().to_base(linear));
        TEST_VECTORS(cie, sample.CIEXYZ, 1e-3);
    }

}

void test_rs_graphics_core_colour_space_wide_gamut() {

    Double3 cie, linear, nonlinear;

    for (auto& sample: samples) {
        TRY(linear = LinearWideGamut().from_base(sample.CIEXYZ));
        TRY(nonlinear = WideGamut().from_base(linear));
        TEST(is_colour_in_gamut<WideGamut>(nonlinear));
        TRY(linear = WideGamut().to_base(nonlinear));
        TRY(cie = LinearWideGamut().to_base(linear));
        TEST_VECTORS(cie, sample.CIEXYZ, 1e-3);
    }

}
