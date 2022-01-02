#include "rs-graphics-core/colour.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include "test/colour-space-test.hpp"
#include "test/vector-test.hpp"

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
