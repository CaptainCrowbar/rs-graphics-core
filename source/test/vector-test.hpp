#pragma once

#include "rs-unit-test.hpp"
#include <cmath>
#include <cstdlib>

// Check that every element of two vectors matches within epsilon

#define TEST_VECTORS(lhs, rhs, epsilon) \
    try { \
        REQUIRE(lhs.size() == rhs.size()); \
        auto rs_unit_test_lhs = lhs; \
        auto rs_unit_test_rhs = rhs; \
        auto rs_unit_test_epsilon = epsilon; \
        for (size_t i = 0; i < lhs.size(); ++i) { \
            if (std::abs((rs_unit_test_rhs)[i] - (rs_unit_test_lhs)[i]) > (rs_unit_test_epsilon)) { \
                FAIL_TEST("Vectors are not close enough: " \
                    << # lhs << " = " << (rs_unit_test_lhs) << ", " \
                    << # rhs << " = " << (rs_unit_test_rhs) << ", " \
                    << "epsilon = " << (rs_unit_test_epsilon)); \
                break; \
            } \
        } \
    } \
    catch (const std::exception& ex) { \
        FAIL_TEST("Unexpected exception: " << ex.what()); \
    } \
    catch (...) { \
        FAIL_TEST("Unexpected exception"); \
    }
