#include "rs-graphics-core/colour.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include "test/vector-test.hpp"

using namespace RS::Graphics::Core;

void test_rs_graphics_core_colour_traits() {

    TEST_EQUAL(sizeof(Rgb8), 3u);
    TEST_EQUAL(sizeof(Rgb16), 6u);
    TEST_EQUAL(sizeof(Rgbf), 12u);
    TEST_EQUAL(sizeof(Rgbd), 24u);
    TEST_EQUAL(sizeof(Rgba8), 4u);
    TEST_EQUAL(sizeof(Rgba16), 8u);
    TEST_EQUAL(sizeof(Rgbaf), 16u);
    TEST_EQUAL(sizeof(Rgbad), 32u);

}

void test_rs_graphics_core_colour_floating_point_elements_() {

    Rgbad c;

    TEST_EQUAL(c.str(), "[0,0,0,0]");

    TEST_EQUAL(&c.R() - c.begin(), 0);
    TEST_EQUAL(&c.G() - c.begin(), 1);
    TEST_EQUAL(&c.B() - c.begin(), 2);
    TEST_EQUAL(&c.alpha() - c.begin(), 3);
    TEST_EQUAL(&c.α() - c.begin(), 3);

    TEST_EQUAL(c.R(), 0);      TRY(c.R() = 0.25);   TEST_EQUAL(c.R(), 0.25);
    TEST_EQUAL(c.G(), 0);      TRY(c.G() = 0.5);    TEST_EQUAL(c.G(), 0.5);
    TEST_EQUAL(c.B(), 0);      TRY(c.B() = 0.75);   TEST_EQUAL(c.B(), 0.75);
    TEST_EQUAL(c.alpha(), 0);  TRY(c.alpha() = 1);  TEST_EQUAL(c.alpha(), 1);
    TEST_EQUAL(c.α(), 1);      TRY(c.α() = 0.5);    TEST_EQUAL(c.α(), 0.5);

    TEST_VECTORS(c.as_vector(), Double4(0.25,0.5,0.75,0.5), 1e-15);
    TEST_EQUAL(c.str(), "[0.25,0.5,0.75,0.5]");

    TRY((c = {0.2,0.4,0.6}));      TEST_EQUAL(c.str(), "[0.2,0.4,0.6,1]");
    TRY((c = {0.2,0.4,0.6,0.8}));  TEST_EQUAL(c.str(), "[0.2,0.4,0.6,0.8]");

}

void test_rs_graphics_core_colour_integer_elements_() {

    Rgba8 c;

    TEST_EQUAL(c.str(), "[0,0,0,0]");

    TEST_EQUAL(&c.R() - c.begin(), 0);
    TEST_EQUAL(&c.G() - c.begin(), 1);
    TEST_EQUAL(&c.B() - c.begin(), 2);
    TEST_EQUAL(&c.alpha() - c.begin(), 3);
    TEST_EQUAL(&c.α() - c.begin(), 3);

    TEST_EQUAL(c.R(), 0);      TRY(c.R() = 0x40);   TEST_EQUAL(c.R(), 0x40);
    TEST_EQUAL(c.G(), 0);      TRY(c.G() = 0x80);   TEST_EQUAL(c.G(), 0x80);
    TEST_EQUAL(c.B(), 0);      TRY(c.B() = 0xc0);   TEST_EQUAL(c.B(), 0xc0);
    TEST_EQUAL(c.alpha(), 0);  TRY(c.alpha() = 1);  TEST_EQUAL(c.alpha(), 1);
    TEST_EQUAL(c.α(), 1);      TRY(c.α() = 0x80);   TEST_EQUAL(c.α(), 0x80);

    TEST_VECTORS(c.as_vector(), Double4(0x40,0x80,0xc0,0x80), 0);
    TEST_EQUAL(c.str(), "[64,128,192,128]");

    TRY((c = {0x20,0x40,0x60}));       TEST_EQUAL(c.str(), "[32,64,96,255]");
    TRY((c = {0x20,0x40,0x60,0x80}));  TEST_EQUAL(c.str(), "[32,64,96,128]");

}
