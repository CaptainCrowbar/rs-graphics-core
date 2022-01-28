#include "rs-graphics-core/colour.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include "test/colour-space-test.hpp"
#include "test/vector-test.hpp"
#include <stdexcept>

using namespace RS::Graphics::Core;
using namespace RS::Graphics::Core::Test;

void test_rs_graphics_core_colour_floating_point_elements() {

    Rgbad c;

    TEST_EQUAL(c.str(), "[0,0,0,0]");

    TEST_EQUAL(&c.R() - c.begin(), 0);
    TEST_EQUAL(&c.G() - c.begin(), 1);
    TEST_EQUAL(&c.B() - c.begin(), 2);
    TEST_EQUAL(&c.alpha() - c.begin(), 3);

    TEST_EQUAL(c.R(), 0);      TRY(c.R() = 0.25);   TEST_EQUAL(c.R(), 0.25);
    TEST_EQUAL(c.G(), 0);      TRY(c.G() = 0.5);    TEST_EQUAL(c.G(), 0.5);
    TEST_EQUAL(c.B(), 0);      TRY(c.B() = 0.75);   TEST_EQUAL(c.B(), 0.75);
    TEST_EQUAL(c.alpha(), 0);  TRY(c.alpha() = 1);  TEST_EQUAL(c.alpha(), 1);

    TEST_VECTORS(c.as_vector(), Double4(0.25,0.5,0.75,1), 1e-15);
    TEST_VECTORS(c, Double4(0.25,0.5,0.75,1), 1e-15);
    TEST_EQUAL(c.str(), "[0.25,0.5,0.75,1]");

    TRY((c = {0.2,0.4,0.6}));      TEST_EQUAL(c.str(), "[0.2,0.4,0.6,1]");
    TRY((c = {0.2,0.4,0.6,0.8}));  TEST_EQUAL(c.str(), "[0.2,0.4,0.6,0.8]");

}

void test_rs_graphics_core_colour_floating_point_arithmetic() {

    Rgbad a = {0.1,0.2,0.3,0.4};
    Rgbad b = {0.8,0.6,0.4,0.2};
    Rgbad c;
    Double4 d = {2,3,4,5};

    TRY(c = + a);    TEST_VECTORS(c, Double4(0.1,0.2,0.3,0.4), 1e-10);
    TRY(c = - a);    TEST_VECTORS(c, Double4(-0.1,-0.2,-0.3,-0.4), 1e-10);
    TRY(c = a + b);  TEST_VECTORS(c, Double4(0.9,0.8,0.7,0.6), 1e-10);
    TRY(c = a - b);  TEST_VECTORS(c, Double4(-0.7,-0.4,-0.1,0.2), 1e-10);
    TRY(c = a * 2);  TEST_VECTORS(c, Double4(0.2,0.4,0.6,0.8), 1e-10);
    TRY(c = 2 * a);  TEST_VECTORS(c, Double4(0.2,0.4,0.6,0.8), 1e-10);
    TRY(c = a / 2);  TEST_VECTORS(c, Double4(0.05,0.1,0.15,0.2), 1e-10);
    TRY(c = a * d);  TEST_VECTORS(c, Double4(0.2,0.6,1.2,2), 1e-10);
    TRY(c = d * a);  TEST_VECTORS(c, Double4(0.2,0.6,1.2,2), 1e-10);
    TRY(c = b / d);  TEST_VECTORS(c, Double4(0.4,0.2,0.1,0.04), 1e-10);

}

void test_rs_graphics_core_colour_floating_point_standard_colours() {

    TEST_EQUAL(Rgbad::clear().str(),    "[0,0,0,0]");
    TEST_EQUAL(Rgbad::black().str(),    "[0,0,0,1]");
    TEST_EQUAL(Rgbad::white().str(),    "[1,1,1,1]");
    TEST_EQUAL(Rgbad::red().str(),      "[1,0,0,1]");
    TEST_EQUAL(Rgbad::yellow().str(),   "[1,1,0,1]");
    TEST_EQUAL(Rgbad::green().str(),    "[0,1,0,1]");
    TEST_EQUAL(Rgbad::cyan().str(),     "[0,1,1,1]");
    TEST_EQUAL(Rgbad::blue().str(),     "[0,0,1,1]");
    TEST_EQUAL(Rgbad::magenta().str(),  "[1,0,1,1]");

}

void test_rs_graphics_core_colour_integral_elements() {

    Rgba8 c;

    TEST_EQUAL(c.str(), "[0,0,0,0]");

    TEST_EQUAL(&c.R() - c.begin(), 0);
    TEST_EQUAL(&c.G() - c.begin(), 1);
    TEST_EQUAL(&c.B() - c.begin(), 2);
    TEST_EQUAL(&c.alpha() - c.begin(), 3);

    TEST_EQUAL(c.R(), 0);      TRY(c.R() = 0x40);      TEST_EQUAL(c.R(), 0x40);
    TEST_EQUAL(c.G(), 0);      TRY(c.G() = 0x80);      TEST_EQUAL(c.G(), 0x80);
    TEST_EQUAL(c.B(), 0);      TRY(c.B() = 0xc0);      TEST_EQUAL(c.B(), 0xc0);
    TEST_EQUAL(c.alpha(), 0);  TRY(c.alpha() = 0xff);  TEST_EQUAL(c.alpha(), 0xff);

    TEST_VECTORS(c.as_vector(), Double4(0x40,0x80,0xc0,0xff), 0);
    TEST_VECTORS(c, Double4(0x40,0x80,0xc0,0xff), 0);
    TEST_EQUAL(c.str(), "[64,128,192,255]");

    TRY((c = {0x20,0x40,0x60}));       TEST_EQUAL(c.str(), "[32,64,96,255]");
    TRY((c = {0x20,0x40,0x60,0x80}));  TEST_EQUAL(c.str(), "[32,64,96,128]");

}

void test_rs_graphics_core_colour_integral_standard_colours() {

    TEST_EQUAL(Rgba8::black().str(),    "[0,0,0,255]");
    TEST_EQUAL(Rgba8::white().str(),    "[255,255,255,255]");
    TEST_EQUAL(Rgba8::red().str(),      "[255,0,0,255]");
    TEST_EQUAL(Rgba8::yellow().str(),   "[255,255,0,255]");
    TEST_EQUAL(Rgba8::green().str(),    "[0,255,0,255]");
    TEST_EQUAL(Rgba8::cyan().str(),     "[0,255,255,255]");
    TEST_EQUAL(Rgba8::blue().str(),     "[0,0,255,255]");
    TEST_EQUAL(Rgba8::magenta().str(),  "[255,0,255,255]");

}

void test_rs_graphics_core_colour_integral_arithmetic() {

    Rgba8 a = {10,20,30,40};
    Rgba8 b = {80,60,40,20};
    Rgba8 c;
    Vector<uint8_t, 4> d = {2,3,4,5};

    TRY(c = + a);    TEST_VECTORS(c, Int4(10,20,30,40), 0);
    TRY(c = - a);    TEST_VECTORS(c, Int4(246,236,226,216), 0);
    TRY(c = a + b);  TEST_VECTORS(c, Int4(90,80,70,60), 0);
    TRY(c = a - b);  TEST_VECTORS(c, Int4(186,216,246,20), 0);
    TRY(c = a * 2);  TEST_VECTORS(c, Int4(20,40,60,80), 0);
    TRY(c = 2 * a);  TEST_VECTORS(c, Int4(20,40,60,80), 0);
    TRY(c = a / 2);  TEST_VECTORS(c, Int4(5,10,15,20), 0);
    TRY(c = a * d);  TEST_VECTORS(c, Int4(20,60,120,200), 0);
    TRY(c = d * a);  TEST_VECTORS(c, Int4(20,60,120,200), 0);
    TRY(c = b / d);  TEST_VECTORS(c, Int4(40,20,10,4), 0);

}

void test_rs_graphics_core_colour_channel_order() {

    Colour<double, LinearRGB, ColourLayout::forward> a;
    Colour<double, LinearRGB, ColourLayout::forward_alpha> b;
    Colour<double, LinearRGB, ColourLayout::alpha_forward> c;
    Colour<double, LinearRGB, ColourLayout::reverse> d;
    Colour<double, LinearRGB, ColourLayout::reverse_alpha> e;
    Colour<double, LinearRGB, ColourLayout::alpha_reverse> f;

    a.R() = 0.1;  a.G() = 0.2;  a.B() = 0.3;
    b.R() = 0.1;  b.G() = 0.2;  b.B() = 0.3;  b.alpha() = 0.4;
    c.R() = 0.1;  c.G() = 0.2;  c.B() = 0.3;  c.alpha() = 0.4;
    d.R() = 0.1;  d.G() = 0.2;  d.B() = 0.3;
    e.R() = 0.1;  e.G() = 0.2;  e.B() = 0.3;  e.alpha() = 0.4;
    f.R() = 0.1;  f.G() = 0.2;  f.B() = 0.3;  f.alpha() = 0.4;

    TEST_VECTORS(a, Double3(0.1,0.2,0.3), 1e-10);
    TEST_VECTORS(b, Double4(0.1,0.2,0.3,0.4), 1e-10);
    TEST_VECTORS(c, Double4(0.4,0.1,0.2,0.3), 1e-10);
    TEST_VECTORS(d, Double3(0.3,0.2,0.1), 1e-10);
    TEST_VECTORS(e, Double4(0.3,0.2,0.1,0.4), 1e-10);
    TEST_VECTORS(f, Double4(0.4,0.3,0.2,0.1), 1e-10);

    a.cs(0) = 0.01;  a.cs(1) = 0.02;  a.cs(2) = 0.03;
    b.cs(0) = 0.01;  b.cs(1) = 0.02;  b.cs(2) = 0.03;  b.cs(3) = 0.04;
    c.cs(0) = 0.01;  c.cs(1) = 0.02;  c.cs(2) = 0.03;  c.cs(3) = 0.04;
    d.cs(0) = 0.01;  d.cs(1) = 0.02;  d.cs(2) = 0.03;
    e.cs(0) = 0.01;  e.cs(1) = 0.02;  e.cs(2) = 0.03;  e.cs(3) = 0.04;
    f.cs(0) = 0.01;  f.cs(1) = 0.02;  f.cs(2) = 0.03;  f.cs(3) = 0.04;

    TEST_VECTORS(a, Double3(0.01,0.02,0.03), 1e-10);
    TEST_VECTORS(b, Double4(0.01,0.02,0.03,0.04), 1e-10);
    TEST_VECTORS(c, Double4(0.04,0.01,0.02,0.03), 1e-10);
    TEST_VECTORS(d, Double3(0.03,0.02,0.01), 1e-10);
    TEST_VECTORS(e, Double4(0.03,0.02,0.01,0.04), 1e-10);
    TEST_VECTORS(f, Double4(0.04,0.03,0.02,0.01), 1e-10);

}

void test_rs_graphics_core_colour_premultiplied_alpha() {

    Rgba8 bc1, bc2, bc3;
    Rgbad fc1, fc2, fc3;

    TRY((bc1 = {50,100,150,200}));
    TRY((fc1 = {0.2,0.4,0.6,0.8}));

    TRY(bc2 = bc1.multiply_alpha());
    TRY(fc2 = fc1.multiply_alpha());

    TEST_VECTORS(bc2, Double4(39,78,118,200), 0);
    TEST_VECTORS(fc2, Double4(0.16,0.32,0.48,0.8), 1e-10);

    TRY(bc3 = bc2.unmultiply_alpha());
    TRY(fc3 = fc2.unmultiply_alpha());

    TEST_VECTORS(bc3, bc1, 1);
    TEST_VECTORS(fc3, fc1, 1e-10);

}

void test_rs_graphics_core_colour_conversion_between_colour_spaces() {

    using C_sRGB = Colour<double, sRGB, ColourLayout::forward>;
    using C_AdobeRGB = Colour<double, AdobeRGB, ColourLayout::forward>;
    using C_CIEXYZ = Colour<double, CIEXYZ, ColourLayout::forward>;
    using C_CIExyY = Colour<double, CIExyY, ColourLayout::forward>;
    using C_CIELab = Colour<double, CIELab, ColourLayout::forward>;
    using C_CIELuv = Colour<double, CIELuv, ColourLayout::forward>;
    using C_HSL = Colour<double, HSL, ColourLayout::forward>;
    using C_HSV = Colour<double, HSV, ColourLayout::forward>;

    C_sRGB sRGB_1, sRGB_2;
    C_AdobeRGB AdobeRGB_1, AdobeRGB_2;
    C_CIEXYZ CIEXYZ_1, CIEXYZ_2;
    C_CIExyY CIExyY_1, CIExyY_2;
    C_CIELab CIELab_1, CIELab_2;
    C_CIELuv CIELuv_1, CIELuv_2;
    C_HSL HSL_1, HSL_2;
    C_HSV HSV_1, HSV_2;

    for (auto& sample: samples()) {

        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, sRGB_2));          TEST_VECTORS(sRGB_2, sample.sRGB, 1e-10);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, AdobeRGB_2));      TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, CIEXYZ_2));        TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, CIExyY_2));        TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, CIELab_2));        TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, CIELuv_2));        TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, HSL_2));           TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(sRGB_1 = C_sRGB(sample.sRGB));              TRY(convert_colour(sRGB_1, HSV_2));           TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, sRGB_2));      TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, AdobeRGB_2));  TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 1e-10);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, CIEXYZ_2));    TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, CIExyY_2));    TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, CIELab_2));    TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, CIELuv_2));    TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, HSL_2));       TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(AdobeRGB_1 = C_AdobeRGB(sample.AdobeRGB));  TRY(convert_colour(AdobeRGB_1, HSV_2));       TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, sRGB_2));        TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, AdobeRGB_2));    TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, CIEXYZ_2));      TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 1e-10);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, CIExyY_2));      TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, CIELab_2));      TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, CIELuv_2));      TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, HSL_2));         TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(CIEXYZ_1 = C_CIEXYZ(sample.CIEXYZ));        TRY(convert_colour(CIEXYZ_1, HSV_2));         TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, sRGB_2));        TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, AdobeRGB_2));    TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, CIEXYZ_2));      TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, CIExyY_2));      TEST_VECTORS(CIExyY_2, sample.CIExyY, 1e-10);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, CIELab_2));      TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, CIELuv_2));      TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, HSL_2));         TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(CIExyY_1 = C_CIExyY(sample.CIExyY));        TRY(convert_colour(CIExyY_1, HSV_2));         TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, sRGB_2));        TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, AdobeRGB_2));    TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, CIEXYZ_2));      TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, CIExyY_2));      TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, CIELab_2));      TEST_VECTORS(CIELab_2, sample.CIELab, 1e-10);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, CIELuv_2));      TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, HSL_2));         TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(CIELab_1 = C_CIELab(sample.CIELab));        TRY(convert_colour(CIELab_1, HSV_2));         TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, sRGB_2));        TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, AdobeRGB_2));    TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, CIEXYZ_2));      TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, CIExyY_2));      TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, CIELab_2));      TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, CIELuv_2));      TEST_VECTORS(CIELuv_2, sample.CIELuv, 1e-10);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, HSL_2));         TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(CIELuv_1 = C_CIELuv(sample.CIELuv));        TRY(convert_colour(CIELuv_1, HSV_2));         TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, sRGB_2));           TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, AdobeRGB_2));       TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, CIEXYZ_2));         TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, CIExyY_2));         TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, CIELab_2));         TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, CIELuv_2));         TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, HSL_2));            TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 1e-10);
        TRY(HSL_1 = C_HSL(sample.HSL));                 TRY(convert_colour(HSL_1, HSV_2));            TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, sRGB_2));           TEST_VECTORS(sRGB_2, sample.sRGB, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, AdobeRGB_2));       TEST_VECTORS(AdobeRGB_2, sample.AdobeRGB, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, CIEXYZ_2));         TEST_VECTORS(CIEXYZ_2, sample.CIEXYZ, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, CIExyY_2));         TEST_VECTORS(CIExyY_2, sample.CIExyY, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, CIELab_2));         TEST_VECTORS(CIELab_2, sample.CIELab, 0.1);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, CIELuv_2));         TEST_VECTORS(CIELuv_2, sample.CIELuv, 0.1);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, HSL_2));            TEST_VECTORS_HSPACE(HSL_2, sample.HSL, 0.001);
        TRY(HSV_1 = C_HSV(sample.HSV));                 TRY(convert_colour(HSV_1, HSV_2));            TEST_VECTORS_HSPACE(HSV_2, sample.HSV, 1e-10);

    }

}

void test_rs_graphics_core_colour_conversion_between_representations() {

    Colour<double, LinearRGB, ColourLayout::forward>          cdf;
    Colour<double, LinearRGB, ColourLayout::forward_alpha>    cdfa;
    Colour<double, LinearRGB, ColourLayout::alpha_forward>    cdaf;
    Colour<double, LinearRGB, ColourLayout::reverse>          cdr;
    Colour<double, LinearRGB, ColourLayout::reverse_alpha>    cdra;
    Colour<double, LinearRGB, ColourLayout::alpha_reverse>    cdar;
    Colour<uint16_t, LinearRGB, ColourLayout::forward>        cuf;
    Colour<uint16_t, LinearRGB, ColourLayout::forward_alpha>  cufa;
    Colour<uint16_t, LinearRGB, ColourLayout::alpha_forward>  cuaf;
    Colour<uint16_t, LinearRGB, ColourLayout::reverse>        cur;
    Colour<uint16_t, LinearRGB, ColourLayout::reverse_alpha>  cura;
    Colour<uint16_t, LinearRGB, ColourLayout::alpha_reverse>  cuar;

    TRY((cdfa = {0.2,0.4,0.6,0.8}));

    TRY(convert_colour(cdfa, cuf));
    TRY(convert_colour(cdfa, cufa));
    TRY(convert_colour(cdfa, cuaf));
    TRY(convert_colour(cdfa, cur));
    TRY(convert_colour(cdfa, cura));
    TRY(convert_colour(cdfa, cuar));

    TEST_VECTORS(cuf,   Int3(0x3333,0x6666,0x9999),         0);
    TEST_VECTORS(cufa,  Int4(0x3333,0x6666,0x9999,0xcccc),  0);
    TEST_VECTORS(cuaf,  Int4(0xcccc,0x3333,0x6666,0x9999),  0);
    TEST_VECTORS(cur,   Int3(0x9999,0x6666,0x3333),         0);
    TEST_VECTORS(cura,  Int4(0x9999,0x6666,0x3333,0xcccc),  0);
    TEST_VECTORS(cuar,  Int4(0xcccc,0x9999,0x6666,0x3333),  0);

    TRY(convert_colour(cuf, cdar));   TEST_VECTORS(cdar, Double4(1.0,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cufa, cdar));  TEST_VECTORS(cdar, Double4(0.8,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cuaf, cdar));  TEST_VECTORS(cdar, Double4(0.8,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cur, cdar));   TEST_VECTORS(cdar, Double4(1.0,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cura, cdar));  TEST_VECTORS(cdar, Double4(0.8,0.6,0.4,0.2),  1e-10);
    TRY(convert_colour(cuar, cdar));  TEST_VECTORS(cdar, Double4(0.8,0.6,0.4,0.2),  1e-10);

    TRY((cufa = {0x3333,0x6666,0x9999,0xcccc}));

    TRY(convert_colour(cufa, cdf));
    TRY(convert_colour(cufa, cdfa));
    TRY(convert_colour(cufa, cdaf));
    TRY(convert_colour(cufa, cdr));
    TRY(convert_colour(cufa, cdra));
    TRY(convert_colour(cufa, cdar));

    TEST_VECTORS(cdf,   Double3(0.2,0.4,0.6),      1e-10);
    TEST_VECTORS(cdfa,  Double4(0.2,0.4,0.6,0.8),  1e-10);
    TEST_VECTORS(cdaf,  Double4(0.8,0.2,0.4,0.6),  1e-10);
    TEST_VECTORS(cdr,   Double3(0.6,0.4,0.2),      1e-10);
    TEST_VECTORS(cdra,  Double4(0.6,0.4,0.2,0.8),  1e-10);
    TEST_VECTORS(cdar,  Double4(0.8,0.6,0.4,0.2),  1e-10);

    TRY(convert_colour(cdf, cuar));   TEST_VECTORS(cuar, Int4(0xffff,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdfa, cuar));  TEST_VECTORS(cuar, Int4(0xcccc,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdaf, cuar));  TEST_VECTORS(cuar, Int4(0xcccc,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdr, cuar));   TEST_VECTORS(cuar, Int4(0xffff,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdra, cuar));  TEST_VECTORS(cuar, Int4(0xcccc,0x9999,0x6666,0x3333),  0);
    TRY(convert_colour(cdar, cuar));  TEST_VECTORS(cuar, Int4(0xcccc,0x9999,0x6666,0x3333),  0);

}

void test_rs_graphics_core_colour_hex_representation() {

    Rgb8 a;
    Rgba8 b;
    Rgbd c;
    Rgbad d;

    sRgb8 e;
    sRgba8 f;
    sRgbd g;
    sRgbad h;

    TRY(a = Rgb8("123456"));        TEST_EQUAL(a.R(), 0x12);  TEST_EQUAL(a.G(), 0x34);  TEST_EQUAL(a.B(), 0x56);  TEST_EQUAL(a.alpha(), 0xff);
    TRY(b = Rgba8("123456"));       TEST_EQUAL(b.R(), 0x12);  TEST_EQUAL(b.G(), 0x34);  TEST_EQUAL(b.B(), 0x56);  TEST_EQUAL(b.alpha(), 0xff);
    TRY(a = Rgb8("(789abcde)"));    TEST_EQUAL(a.R(), 0x78);  TEST_EQUAL(a.G(), 0x9a);  TEST_EQUAL(a.B(), 0xbc);  TEST_EQUAL(a.alpha(), 0xff);
    TRY(b = Rgba8("(789abcde)"));   TEST_EQUAL(b.R(), 0x78);  TEST_EQUAL(b.G(), 0x9a);  TEST_EQUAL(b.B(), 0xbc);  TEST_EQUAL(b.alpha(), 0xde);
    TRY(e = sRgb8("123456"));       TEST_EQUAL(e.R(), 0x12);  TEST_EQUAL(e.G(), 0x34);  TEST_EQUAL(e.B(), 0x56);  TEST_EQUAL(e.alpha(), 0xff);
    TRY(f = sRgba8("123456"));      TEST_EQUAL(f.R(), 0x12);  TEST_EQUAL(f.G(), 0x34);  TEST_EQUAL(f.B(), 0x56);  TEST_EQUAL(f.alpha(), 0xff);
    TRY(e = sRgb8("(789abcde)"));   TEST_EQUAL(e.R(), 0x78);  TEST_EQUAL(e.G(), 0x9a);  TEST_EQUAL(e.B(), 0xbc);  TEST_EQUAL(e.alpha(), 0xff);
    TRY(f = sRgba8("(789abcde)"));  TEST_EQUAL(f.R(), 0x78);  TEST_EQUAL(f.G(), 0x9a);  TEST_EQUAL(f.B(), 0xbc);  TEST_EQUAL(f.alpha(), 0xde);

    TRY(c = Rgbd("123456"));
    TEST_NEAR(c.R(), 0.070588, 1e-6);
    TEST_NEAR(c.G(), 0.203922, 1e-6);
    TEST_NEAR(c.B(), 0.337255, 1e-6);
    TEST_EQUAL(c.alpha(), 1);
    TRY(d = Rgbad("123456"));
    TEST_NEAR(d.R(), 0.070588, 1e-6);
    TEST_NEAR(d.G(), 0.203922, 1e-6);
    TEST_NEAR(d.B(), 0.337255, 1e-6);
    TEST_EQUAL(d.alpha(), 1);
    TRY(c = Rgbd("(789abcde)"));
    TEST_NEAR(c.R(), 0.470588, 1e-6);
    TEST_NEAR(c.G(), 0.603922, 1e-6);
    TEST_NEAR(c.B(), 0.737255, 1e-6);
    TEST_EQUAL(c.alpha(), 1);
    TRY(d = Rgbad("(789abcde)"));
    TEST_NEAR(d.R(), 0.470588, 1e-6);
    TEST_NEAR(d.G(), 0.603922, 1e-6);
    TEST_NEAR(d.B(), 0.737255, 1e-6);
    TEST_NEAR(d.alpha(), 0.870588, 1e-6);
    TRY(g = sRgbd("123456"));
    TEST_NEAR(g.R(), 0.070588, 1e-6);
    TEST_NEAR(g.G(), 0.203922, 1e-6);
    TEST_NEAR(g.B(), 0.337255, 1e-6);
    TEST_EQUAL(g.alpha(), 1);
    TRY(h = sRgbad("123456"));
    TEST_NEAR(h.R(), 0.070588, 1e-6);
    TEST_NEAR(h.G(), 0.203922, 1e-6);
    TEST_NEAR(h.B(), 0.337255, 1e-6);
    TEST_EQUAL(h.alpha(), 1);
    TRY(g = sRgbd("(789abcde)"));
    TEST_NEAR(g.R(), 0.470588, 1e-6);
    TEST_NEAR(g.G(), 0.603922, 1e-6);
    TEST_NEAR(g.B(), 0.737255, 1e-6);
    TEST_EQUAL(g.alpha(), 1);
    TRY(h = sRgbad("(789abcde)"));
    TEST_NEAR(h.R(), 0.470588, 1e-6);
    TEST_NEAR(h.G(), 0.603922, 1e-6);
    TEST_NEAR(h.B(), 0.737255, 1e-6);
    TEST_NEAR(h.alpha(), 0.870588, 1e-6);

    TEST_THROW(Rgba8(""), std::invalid_argument);
    TEST_THROW(Rgba8("12345"), std::invalid_argument);
    TEST_THROW(Rgba8("1234567"), std::invalid_argument);
    TEST_THROW(Rgba8("123456789"), std::invalid_argument);
    TEST_THROW(Rgba8("abcdefgh"), std::invalid_argument);

    TRY((a = {0xfe,0xdc,0xba}));
    TRY((b = {0x98,0x76,0x54,0x32}));
    TRY((c = {0.2,0.4,0.6}));
    TRY((d = {0.8,0.6,0.4,0.2}));
    TRY((e = {0xfe,0xdc,0xba}));
    TRY((f = {0x98,0x76,0x54,0x32}));
    TRY((g = {0.2,0.4,0.6}));
    TRY((h = {0.8,0.6,0.4,0.2}));

    TEST_EQUAL(a.hex(), "fedcba");
    TEST_EQUAL(b.hex(), "98765432");
    TEST_EQUAL(c.hex(), "336699");
    TEST_EQUAL(d.hex(), "cc996633");
    TEST_EQUAL(e.hex(), "fedcba");
    TEST_EQUAL(f.hex(), "98765432");
    TEST_EQUAL(g.hex(), "336699");
    TEST_EQUAL(h.hex(), "cc996633");

}

void test_rs_graphics_core_colour_alpha_blending() {

    Rgbaf a, b, c, pa, pb, pc;
    Rgba8 d, e, f, pd, pe, pf;

    TRY((a = {0.2,0.4,0.6,0.8}));
    TRY((b = {0.9,0.8,0.7,0.6}));
    TRY(c = alpha_blend(a, b));
    TEST_VECTORS(c, Double4(0.291304,0.452174,0.613043,0.92), 1e-6);

    TRY(pa = a.multiply_alpha());
    TRY(pb = b.multiply_alpha());
    TRY(pc = alpha_blend(pa, pb, Pma::all));
    TRY(c = pc.unmultiply_alpha());
    TEST_VECTORS(c, Double4(0.291304,0.452174,0.613043,0.92), 1e-6);

    TRY((d = {50,100,150,200}));
    TRY((e = {250,225,200,175}));
    TRY(f = alpha_blend(d, e));
    TEST_VECTORS(f, Double4(82,120,158,238), 0);

    TRY(pd = d.multiply_alpha());
    TRY(pe = e.multiply_alpha());
    TRY(pf = alpha_blend(pd, pe, Pma::all));
    TRY(f = pf.unmultiply_alpha());
    TEST_VECTORS(f, Double4(82,120,158,238), 1);

}

void test_rs_graphics_core_colour_css_colours() {

    sRgba8 a;
    Rgba8 b;
    sRgbad c;
    Rgbad d;

    TRY(a = css_colour<sRgba8>("Crimson"));       TEST_EQUAL(a,    sRgba8(0xdc, 0x14, 0x3c, 0xff));
    TRY(b = css_colour<Rgba8>("Crimson"));        TEST_EQUAL(b,    Rgba8(0xb7, 0x02, 0x0c, 0xff));
    TRY(c = css_colour<sRgbad>("Crimson"));       TEST_VECTORS(c,  sRgbad(0.862745, 0.0784314, 0.235294, 1), 1e-6);
    TRY(d = css_colour<Rgbad>("Crimson"));        TEST_VECTORS(d,  Rgbad(0.715694, 0.00699541, 0.0451862, 1), 1e-6);
    TRY(a = css_colour<sRgba8>("Forest Green"));  TEST_EQUAL(a,    sRgba8(0x22, 0x8b, 0x22, 0xff));
    TRY(b = css_colour<Rgba8>("Forest Green"));   TEST_EQUAL(b,    Rgba8(0x04, 0x42, 0x04, 0xff));
    TRY(c = css_colour<sRgbad>("Forest Green"));  TEST_VECTORS(c,  sRgbad(0.133333, 0.545098, 0.133333, 1), 1e-6);
    TRY(d = css_colour<Rgbad>("Forest Green"));   TEST_VECTORS(d,  Rgbad(0.0159963, 0.258183, 0.0159963, 1), 1e-6);
    TRY(a = css_colour<sRgba8>("Royal Blue"));    TEST_EQUAL(a,    sRgba8(0x41, 0x69, 0xe1, 0xff));
    TRY(b = css_colour<Rgba8>("Royal Blue"));     TEST_EQUAL(b,    Rgba8(0x0d, 0x24, 0xc0, 0xff));
    TRY(c = css_colour<sRgbad>("Royal Blue"));    TEST_VECTORS(c,  sRgbad(0.254902, 0.411765, 0.882353, 1), 1e-6);
    TRY(d = css_colour<Rgbad>("Royal Blue"));     TEST_VECTORS(d,  Rgbad(0.0528606, 0.141263, 0.752942, 1), 1e-6);
    TRY(a = css_colour<sRgba8>("Saddle Brown"));  TEST_EQUAL(a,    sRgba8(0x8b, 0x45, 0x13, 0xff));
    TRY(b = css_colour<Rgba8>("Saddle Brown"));   TEST_EQUAL(b,    Rgba8(0x42, 0x0f, 0x02, 0xff));
    TRY(c = css_colour<sRgbad>("Saddle Brown"));  TEST_VECTORS(c,  sRgbad(0.545098, 0.270588, 0.0745098, 1), 1e-6);
    TRY(d = css_colour<Rgbad>("Saddle Brown"));   TEST_VECTORS(d,  Rgbad(0.258183, 0.0595112, 0.00651209, 1), 1e-6);
    TRY(a = css_colour<sRgba8>("Transparent"));   TEST_EQUAL(a,    sRgba8(0, 0, 0, 0));
    TRY(b = css_colour<Rgba8>("Transparent"));    TEST_EQUAL(b,    Rgba8(0, 0, 0, 0));
    TRY(c = css_colour<sRgbad>("Transparent"));   TEST_EQUAL(c,    sRgbad(0, 0, 0, 0));
    TRY(d = css_colour<Rgbad>("Transparent"));    TEST_EQUAL(d,    Rgbad(0, 0, 0, 0));
    TRY(a = css_colour<sRgba8>("123456"));        TEST_EQUAL(a,    sRgba8(0x12, 0x34, 0x56, 0xff));
    TRY(b = css_colour<Rgba8>("123456"));         TEST_EQUAL(b,    Rgba8(0x02, 0x09, 0x18, 0xff));
    TRY(c = css_colour<sRgbad>("123456"));        TEST_VECTORS(c,  sRgbad(0.070588, 0.203922, 0.337255, 1.000000), 1e-6);
    TRY(d = css_colour<Rgbad>("123456"));         TEST_VECTORS(d,  Rgbad(0.006049,0.034340,0.093059,1.000000), 1e-6);
    TRY(a = css_colour<sRgba8>("(789abcde)"));    TEST_EQUAL(a,    sRgba8(0x78, 0x9a, 0xbc, 0xde));
    TRY(b = css_colour<Rgba8>("(789abcde)"));     TEST_EQUAL(b,    Rgba8(0x30, 0x52, 0x80, 0xde));
    TRY(c = css_colour<sRgbad>("(789abcde)"));    TEST_VECTORS(c,  sRgbad(0.470588, 0.603922, 0.737255, 0.870588), 1e-6);
    TRY(d = css_colour<Rgbad>("(789abcde)"));     TEST_VECTORS(d,  Rgbad(0.187821,0.323144,0.502887,0.870588), 1e-6);

}
