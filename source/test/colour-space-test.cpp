#include "rs-graphics-core/colour-space.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include "test/colour-space-test.hpp"
#include "test/vector-test.hpp"
#include <vector>

using namespace RS::Graphics::Core;
using namespace RS::Graphics::Core::Test;

void test_rs_graphics_core_colour_space_ciexyy() {

    Double3 xyy, xyz;

    for (auto& sample: samples()) {
        TRY(xyy = CIExyY::from_base(sample.CIEXYZ));
        TEST(is_colour_in_gamut<CIExyY>(xyy));
        TEST_VECTORS(xyy, sample.CIExyY, 1e-4);
        TRY(xyz = CIExyY::to_base(sample.CIExyY));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-4);
    }

}

void test_rs_graphics_core_colour_space_cielab() {

    Double3 colour, xyz;

    for (auto& sample: samples()) {
        TRY(colour = CIELab::from_base(sample.CIEXYZ));
        TEST(is_colour_in_gamut<CIELab>(colour));
        TEST_VECTORS(colour, sample.CIELab, 0.01);
        TRY(xyz = CIELab::to_base(sample.CIELab));
        TEST_VECTORS(xyz, sample.CIEXYZ, 0.01);
    }

}

void test_rs_graphics_core_colour_space_cieluv() {

    Double3 colour, xyz;

    for (auto& sample: samples()) {
        TRY(colour = CIELuv::from_base(sample.CIEXYZ));
        TEST(is_colour_in_gamut<CIELuv>(colour));
        TEST_VECTORS(colour, sample.CIELuv, 0.01);
        TRY(xyz = CIELuv::to_base(sample.CIELuv));
        TEST_VECTORS(xyz, sample.CIEXYZ, 0.01);
    }

}

void test_rs_graphics_core_colour_space_srgb() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples()) {
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

    for (auto& sample: samples()) {
        TRY(linear = LinearAdobeRGB::from_base(sample.CIEXYZ));
        TRY(nonlinear = AdobeRGB::from_base(linear));
        TEST(is_colour_in_gamut<AdobeRGB>(nonlinear));
        TEST_VECTORS(nonlinear, sample.AdobeRGB, 1e-3);
        TRY(linear = AdobeRGB::to_base(sample.AdobeRGB));
        TRY(xyz = LinearAdobeRGB::to_base(linear));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-3);
    }

}

void test_rs_graphics_core_colour_space_pro_photo() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples()) {
        TRY(linear = LinearProPhoto::from_base(sample.CIEXYZ));
        TRY(nonlinear = ProPhoto::from_base(linear));
        TEST(is_colour_in_gamut<ProPhoto>(nonlinear));
        TRY(linear = ProPhoto::to_base(nonlinear));
        TRY(xyz = LinearProPhoto::to_base(linear));
        TEST_VECTORS(xyz, sample.CIEXYZ, 1e-6);
    }

}

void test_rs_graphics_core_colour_space_wide_gamut() {

    Double3 linear, nonlinear, xyz;

    for (auto& sample: samples()) {
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

    for (auto& sample: samples()) {
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

    for (auto& sample: samples()) {
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

    for (auto& sample: samples()) {

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
