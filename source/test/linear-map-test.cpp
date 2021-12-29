#include "rs-graphics-core/linear-map.hpp"
#include "rs-unit-test.hpp"

using namespace RS::Graphics::Core;

void test_rs_graphics_core_linear_map() {

    LinearMap<double> map;

    TEST(map.empty());
    TEST_EQUAL(map.min(), 0);
    TEST_EQUAL(map.max(), 0);
    TEST_EQUAL(map[0], 0);
    TEST_EQUAL(map[10], 0);

    TRY(map.insert(5, 10));
    TEST(! map.empty());
    TEST_EQUAL(map.min(), 5);
    TEST_EQUAL(map.max(), 5);
    TEST_EQUAL(map[0], 10);
    TEST_EQUAL(map[5], 10);
    TEST_EQUAL(map[10], 10);

    TRY(map.insert(10, 20));
    TEST_EQUAL(map.min(), 5);
    TEST_EQUAL(map.max(), 10);
    TEST_EQUAL(map[4], 10);
    TEST_EQUAL(map[5], 10);
    TEST_EQUAL(map[6], 12);
    TEST_EQUAL(map[7], 14);
    TEST_EQUAL(map[8], 16);
    TEST_EQUAL(map[9], 18);
    TEST_EQUAL(map[10], 20);
    TEST_EQUAL(map[11], 20);

    TRY(map.insert(15, 100, 10));
    TRY(map.insert(20, -10));
    TEST_EQUAL(map.min(), 5);
    TEST_EQUAL(map.max(), 20);
    TEST_EQUAL(map[4], 10);
    TEST_EQUAL(map[5], 10);
    TEST_EQUAL(map[6], 12);
    TEST_EQUAL(map[7], 14);
    TEST_EQUAL(map[8], 16);
    TEST_EQUAL(map[9], 18);
    TEST_EQUAL(map[10], 20);
    TEST_EQUAL(map[11], 36);
    TEST_EQUAL(map[12], 52);
    TEST_EQUAL(map[13], 68);
    TEST_EQUAL(map[14], 84);
    TEST_EQUAL(map[15], 55);
    TEST_EQUAL(map[16], 6);
    TEST_EQUAL(map[17], 2);
    TEST_EQUAL(map[18], -2);
    TEST_EQUAL(map[19], -6);
    TEST_EQUAL(map[20], -10);
    TEST_EQUAL(map[21], -10);

    TRY(map.erase(10));
    TEST_EQUAL(map.min(), 5);
    TEST_EQUAL(map.max(), 20);
    TEST_EQUAL(map[4], 10);
    TEST_EQUAL(map[5], 10);
    TEST_EQUAL(map[6], 19);
    TEST_EQUAL(map[7], 28);
    TEST_EQUAL(map[8], 37);
    TEST_EQUAL(map[9], 46);
    TEST_EQUAL(map[10], 55);
    TEST_EQUAL(map[11], 64);
    TEST_EQUAL(map[12], 73);
    TEST_EQUAL(map[13], 82);
    TEST_EQUAL(map[14], 91);
    TEST_EQUAL(map[15], 55);
    TEST_EQUAL(map[16], 6);
    TEST_EQUAL(map[17], 2);
    TEST_EQUAL(map[18], -2);
    TEST_EQUAL(map[19], -6);
    TEST_EQUAL(map[20], -10);
    TEST_EQUAL(map[21], -10);

    TRY(map.erase(5, 15));
    TEST_EQUAL(map.min(), 20);
    TEST_EQUAL(map.max(), 20);
    TEST_EQUAL(map[4], -10);
    TEST_EQUAL(map[5], -10);
    TEST_EQUAL(map[6], -10);
    TEST_EQUAL(map[7], -10);
    TEST_EQUAL(map[8], -10);
    TEST_EQUAL(map[9], -10);
    TEST_EQUAL(map[10], -10);
    TEST_EQUAL(map[11], -10);
    TEST_EQUAL(map[12], -10);
    TEST_EQUAL(map[13], -10);
    TEST_EQUAL(map[14], -10);
    TEST_EQUAL(map[15], -10);
    TEST_EQUAL(map[16], -10);
    TEST_EQUAL(map[17], -10);
    TEST_EQUAL(map[18], -10);
    TEST_EQUAL(map[19], -10);
    TEST_EQUAL(map[20], -10);
    TEST_EQUAL(map[21], -10);

    TRY(map.erase(0, 100));
    TEST(map.empty());
    TEST_EQUAL(map.min(), 0);
    TEST_EQUAL(map.max(), 0);
    TEST_EQUAL(map[0], 0);
    TEST_EQUAL(map[10], 0);

    TRY((map = {
        {10, 100},
        {20, 200, 300, 400},
        {30, 500, 600},
    }));
    TEST(! map.empty());
    TEST_EQUAL(map.min(), 10);
    TEST_EQUAL(map.max(), 30);
    TEST_EQUAL(map[5], 100);
    TEST_EQUAL(map[10], 100);
    TEST_EQUAL(map[15], 150);
    TEST_EQUAL(map[20], 300);
    TEST_EQUAL(map[25], 450);
    TEST_EQUAL(map[30], 550);
    TEST_EQUAL(map[35], 600);

}
