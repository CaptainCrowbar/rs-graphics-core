#include "rs-graphics-core/colour.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include "test/vector-test.hpp"

using namespace RS::Graphics::Core;

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

    // These should fail to compile

    // sRgbad s = {0.1,0.2,0.3,0.4};
    // sRgbad t = {0.8,0.6,0.4,0.2};
    // sRgbad u;

    // TRY(u = + s);    TEST_VECTORS(u, Double4(0.1,0.2,0.3,0.4), 1e-10);
    // TRY(u = - s);    TEST_VECTORS(u, Double4(-0.1,-0.2,-0.3,-0.4), 1e-10);
    // TRY(u = s + t);  TEST_VECTORS(u, Double4(0.9,0.8,0.7,0.6), 1e-10);
    // TRY(u = s - t);  TEST_VECTORS(u, Double4(-0.7,-0.4,-0.1,0.2), 1e-10);
    // TRY(u = s * 2);  TEST_VECTORS(u, Double4(0.2,0.4,0.6,0.8), 1e-10);
    // TRY(u = 2 * s);  TEST_VECTORS(u, Double4(0.2,0.4,0.6,0.8), 1e-10);
    // TRY(u = s / 2);  TEST_VECTORS(u, Double4(0.05,0.1,0.15,0.2), 1e-10);

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
