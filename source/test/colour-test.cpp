#include "rs-graphics-core/colour.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include "test/vector-test.hpp"

using namespace RS::Graphics::Core;

void test_rs_graphics_core_colour_elements() {

    Rgbad c;

    TEST_EQUAL(c.str(), "[0,0,0,0]");

    TEST_EQUAL(&c.R() - c.begin(), 0);
    TEST_EQUAL(&c.G() - c.begin(), 1);
    TEST_EQUAL(&c.B() - c.begin(), 2);
    TEST_EQUAL(&c.A() - c.begin(), 3);

    TEST_EQUAL(c.R(), 0);  TRY(c.R() = 0.25);  TEST_EQUAL(c.R(), 0.25);
    TEST_EQUAL(c.G(), 0);  TRY(c.G() = 0.5);   TEST_EQUAL(c.G(), 0.5);
    TEST_EQUAL(c.B(), 0);  TRY(c.B() = 0.75);  TEST_EQUAL(c.B(), 0.75);
    TEST_EQUAL(c.A(), 0);  TRY(c.A() = 1);     TEST_EQUAL(c.A(), 1);

    TEST_VECTORS(c.as_vector(), Double4(0.25,0.5,0.75,1.0), 1e-15);
    TEST_EQUAL(c.str(), "[0.25,0.5,0.75,1]");

    TRY((c = {0.2,0.4,0.6}));      TEST_EQUAL(c.str(), "[0.2,0.4,0.6,1]");
    TRY((c = {0.2,0.4,0.6,0.8}));  TEST_EQUAL(c.str(), "[0.2,0.4,0.6,0.8]");

}
