#include "rs-graphics-core/colour-space.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include "test/vector-test.hpp"
#include <vector>

using namespace RS::Graphics::Core;

namespace {

    // Source for CIE and nonlinear RGB colour space samples:
    // https://www.easyrgb.com/en/convert.php

    // Colours are selected to be in gamut for all colour spaces of interest.
    // Differences in 4th-5th decimal place are probably due to differences
    // in matrix approximations between this source and Lindbloom.

    struct Sample {
        Double3 sRGB;
        Double3 AdobeRGB;
        Double3 CIEXYZ;
        Double3 CIExyY;
        Double3 CIELab;
        Double3 CIELuv;
        Double3 HSL;
        Double3 HSV;
    };

    const std::vector<Sample> samples = {
        // sRGB             AdobeRGB                    CIEXYZ                      CIExyY                      CIELab                    CIELuv                     HSL                            HSV
        {{0.00,0.00,0.00},  {0.00000,0.00000,0.00000},  {0.00000,0.00000,0.00000},  {0.00000,0.00000,0.00000},  {0.000,0.000,0.000},      {0.000,0.000,0.000},       {0.000000,0.000000,0.000000},  {0.000000,0.000000,0.000000}},
        {{0.25,0.50,0.75},  {0.34673,0.49610,0.73614},  {0.19180,0.20160,0.52305},  {0.20929,0.21998,0.20160},  {52.018,0.093,-39.363},   {52.018,-25.362,-60.283},  {0.609011,0.822549,0.286699},  {0.609011,0.902636,0.522522}},
        {{0.25,0.75,0.50},  {0.46453,0.74442,0.50928},  {0.24645,0.39995,0.26667},  {0.26991,0.43803,0.39995},  {69.466,-49.554,22.224},  {69.466,-52.311,38.425},   {0.390990,0.822506,0.286702},  {0.390990,0.902610,0.522517}},
        {{0.50,0.25,0.75},  {0.44385,0.25813,0.73171},  {0.20076,0.11962,0.50676},  {0.24271,0.14462,0.11962},  {41.155,51.411,-56.449},  {41.155,16.371,-86.719},   {0.724326,0.822542,0.286701},  {0.724326,0.902631,0.522525}},
        {{0.50,0.75,0.25},  {0.58009,0.74442,0.29901},  {0.28430,0.42288,0.11477},  {0.34589,0.51448,0.42288},  {71.069,-40.908,55.644},  {71.069,-32.079,71.662},   {0.275685,0.822541,0.286707},  {0.275685,0.902631,0.522534}},
        {{0.50,0.25,0.75},  {0.44385,0.25813,0.73171},  {0.20076,0.11962,0.50676},  {0.24271,0.14462,0.11962},  {41.155,51.411,-56.449},  {41.155,16.371,-86.719},   {0.724326,0.822542,0.286701},  {0.724326,0.902631,0.522525}},
        {{0.75,0.25,0.50},  {0.65033,0.25813,0.48896},  {0.27233,0.16296,0.21957},  {0.41586,0.24885,0.16296},  {47.360,56.522,-8.041},   {47.360,76.888,-20.831},   {0.942342,0.822535,0.286699},  {0.942342,0.902628,0.522518}},
        {{0.75,0.50,0.25},  {0.68464,0.49610,0.27311},  {0.30123,0.26787,0.08396},  {0.46126,0.41017,0.26787},  {58.777,18.585,43.798},   {58.777,50.243,45.130},    {0.057661,0.822550,0.286693},  {0.057661,0.902636,0.522512}},
        {{0.85,0.85,0.85},  {0.84589,0.84589,0.84589},  {0.65779,0.69207,0.75355},  {0.31273,0.32902,0.69207},  {86.607,0.000,-0.000},    {86.607,-0.000,-0.000},    {0.527616,0.000018,0.692068},  {0.527616,0.000016,0.692074}},
    };

}

void test_rs_graphics_core_colour_space_ciexyy() {

    Double3 xyy, xyz;

    for (auto& sample: samples) {
        TRY(xyy = CIExyY::from_base(sample.CIEXYZ));
        TEST(is_colour_in_gamut<CIExyY>(xyy));
        TEST_VECTORS(xyy, sample.CIExyY, 1e-4);
        TRY(xyz = CIExyY::to_base(sample.CIExyY));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-4);
    }

}

void test_rs_graphics_core_colour_space_cielab() {

    Double3 colour, xyz;

    for (auto& sample: samples) {
        TRY(colour = CIELab::from_base(sample.CIEXYZ));
        TEST(is_colour_in_gamut<CIELab>(colour));
        TEST_VECTORS(colour, sample.CIELab, 0.01);
        TRY(xyz = CIELab::to_base(sample.CIELab));
        TEST_VECTORS(xyz, sample.CIEXYZ, 0.01);
    }

}

void test_rs_graphics_core_colour_space_cieluv() {

    Double3 colour, xyz;

    for (auto& sample: samples) {
        TRY(colour = CIELuv::from_base(sample.CIEXYZ));
        TEST(is_colour_in_gamut<CIELuv>(colour));
        TEST_VECTORS(colour, sample.CIELuv, 0.01);
        TRY(xyz = CIELuv::to_base(sample.CIELuv));
        TEST_VECTORS(xyz, sample.CIEXYZ, 0.01);
    }

}

void test_rs_graphics_core_colour_space_srgb() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples) {
        TRY(linear = LinearRGB::from_base(sample.CIEXYZ));
        TRY(nonlinear = sRGB::from_base(linear));
        TEST(is_colour_in_gamut<sRGB>(nonlinear));
        TEST_VECTORS(nonlinear, sample.sRGB, 1e-4);
        TRY(linear = sRGB::to_base(sample.sRGB));
        TRY(xyz = LinearRGB::to_base(linear));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-4);
    }

}

void test_rs_graphics_core_colour_space_adobe_rgb() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples) {
        TRY(linear = LinearAdobeRGB::from_base(sample.CIEXYZ));
        TRY(nonlinear = AdobeRGB::from_base(linear));
        TEST(is_colour_in_gamut<AdobeRGB>(nonlinear));
        TEST_VECTORS(nonlinear, sample.AdobeRGB, 1e-3);
        TRY(linear = AdobeRGB::to_base(sample.AdobeRGB));
        TRY(xyz = LinearAdobeRGB::to_base(linear));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-3);
    }

}

void test_rs_graphics_core_colour_space_wide_gamut() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples) {
        TRY(linear = LinearWideGamut::from_base(sample.CIEXYZ));
        TRY(nonlinear = WideGamut::from_base(linear));
        TEST(is_colour_in_gamut<WideGamut>(nonlinear));
        TRY(linear = WideGamut::to_base(nonlinear));
        TRY(xyz = LinearWideGamut::to_base(linear));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-6);
    }

}

void test_rs_graphics_core_colour_space_hsl() {

    Double3 rgb, polar, xyz;

    for (auto& sample: samples) {
        TRY(rgb = LinearRGB::from_base(sample.CIEXYZ));
        TRY(polar = HSL::from_base(rgb));
        TEST(is_colour_in_gamut<HSL>(polar));
        TEST_VECTORS(polar, sample.HSL, 1e-5);
        TRY(rgb = HSL::to_base(sample.HSL));
        TRY(xyz = LinearRGB::to_base(rgb));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-5);
    }

}

void test_rs_graphics_core_colour_space_hsv() {

    Double3 rgb, polar, xyz;

    for (auto& sample: samples) {
        TRY(rgb = LinearRGB::from_base(sample.CIEXYZ));
        TRY(polar = HSV::from_base(rgb));
        TEST(is_colour_in_gamut<HSV>(polar));
        TEST_VECTORS(polar, sample.HSV, 1e-5);
        TRY(rgb = HSV::to_base(sample.HSV));
        TRY(xyz = LinearRGB::to_base(rgb));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-5);
    }

}

void test_rs_graphics_core_colour_space_conversion() {

    Double3 c;

    for (auto& sample: samples) {

        TRY((c = convert_colour_space<sRGB, sRGB>(sample.sRGB)));              TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<sRGB, AdobeRGB>(sample.sRGB)));          TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<sRGB, CIEXYZ>(sample.sRGB)));            TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<sRGB, CIExyY>(sample.sRGB)));            TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<sRGB, CIELab>(sample.sRGB)));            TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<sRGB, CIELuv>(sample.sRGB)));            TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<sRGB, HSL>(sample.sRGB)));               TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<sRGB, HSV>(sample.sRGB)));               TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, sRGB>(sample.AdobeRGB)));      TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, AdobeRGB>(sample.AdobeRGB)));  TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, CIEXYZ>(sample.AdobeRGB)));    TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, CIExyY>(sample.AdobeRGB)));    TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, CIELab>(sample.AdobeRGB)));    TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<AdobeRGB, CIELuv>(sample.AdobeRGB)));    TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<AdobeRGB, HSL>(sample.AdobeRGB)));       TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<AdobeRGB, HSV>(sample.AdobeRGB)));       TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, sRGB>(sample.CIEXYZ)));          TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, AdobeRGB>(sample.CIEXYZ)));      TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, CIEXYZ>(sample.CIEXYZ)));        TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, CIExyY>(sample.CIEXYZ)));        TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, CIELab>(sample.CIEXYZ)));        TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<CIEXYZ, CIELuv>(sample.CIEXYZ)));        TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<CIEXYZ, HSL>(sample.CIEXYZ)));           TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<CIEXYZ, HSV>(sample.CIEXYZ)));           TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<CIExyY, sRGB>(sample.CIExyY)));          TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<CIExyY, AdobeRGB>(sample.CIExyY)));      TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<CIExyY, CIEXYZ>(sample.CIExyY)));        TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<CIExyY, CIExyY>(sample.CIExyY)));        TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<CIExyY, CIELab>(sample.CIExyY)));        TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<CIExyY, CIELuv>(sample.CIExyY)));        TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<CIExyY, HSL>(sample.CIExyY)));           TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<CIExyY, HSV>(sample.CIExyY)));           TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<CIELab, sRGB>(sample.CIELab)));          TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<CIELab, AdobeRGB>(sample.CIELab)));      TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<CIELab, CIEXYZ>(sample.CIELab)));        TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<CIELab, CIExyY>(sample.CIELab)));        TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<CIELab, CIELab>(sample.CIELab)));        TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<CIELab, CIELuv>(sample.CIELab)));        TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<CIELab, HSL>(sample.CIELab)));           TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<CIELab, HSV>(sample.CIELab)));           TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<CIELuv, sRGB>(sample.CIELuv)));          TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<CIELuv, AdobeRGB>(sample.CIELuv)));      TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<CIELuv, CIEXYZ>(sample.CIELuv)));        TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<CIELuv, CIExyY>(sample.CIELuv)));        TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<CIELuv, CIELab>(sample.CIELuv)));        TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<CIELuv, CIELuv>(sample.CIELuv)));        TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<CIELuv, HSL>(sample.CIELuv)));           TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<CIELuv, HSV>(sample.CIELuv)));           TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<HSL, sRGB>(sample.HSL)));                TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<HSL, AdobeRGB>(sample.HSL)));            TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<HSL, CIEXYZ>(sample.HSL)));              TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<HSL, CIExyY>(sample.HSL)));              TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<HSL, CIELab>(sample.HSL)));              TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<HSL, CIELuv>(sample.HSL)));              TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<HSL, HSL>(sample.HSL)));                 TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<HSL, HSV>(sample.HSL)));                 TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);
        TRY((c = convert_colour_space<HSV, sRGB>(sample.HSV)));                TEST_VECTORS(c, sample.sRGB, 0.001);
        TRY((c = convert_colour_space<HSV, AdobeRGB>(sample.HSV)));            TEST_VECTORS(c, sample.AdobeRGB, 0.001);
        TRY((c = convert_colour_space<HSV, CIEXYZ>(sample.HSV)));              TEST_VECTORS(c, sample.CIEXYZ, 0.001);
        TRY((c = convert_colour_space<HSV, CIExyY>(sample.HSV)));              TEST_VECTORS(c, sample.CIExyY, 0.001);
        TRY((c = convert_colour_space<HSV, CIELab>(sample.HSV)));              TEST_VECTORS(c, sample.CIELab, 0.1);
        TRY((c = convert_colour_space<HSV, CIELuv>(sample.HSV)));              TEST_VECTORS(c, sample.CIELuv, 0.1);
        TRY((c = convert_colour_space<HSV, HSL>(sample.HSV)));                 TEST_VECTORS_HSPACE(c, sample.HSL, 0.001);
        TRY((c = convert_colour_space<HSV, HSV>(sample.HSV)));                 TEST_VECTORS_HSPACE(c, sample.HSV, 0.001);

    }

}
