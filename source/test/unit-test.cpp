// This file is generated by the rs-update-tests script

#include "rs-unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // version-test.cpp
    UNIT_TEST(rs_graphics_core_version)

    // maths-test.cpp
    UNIT_TEST(rs_graphics_core_maths_euclidean_division)
    UNIT_TEST(rs_graphics_core_maths_symmetric_division)
    UNIT_TEST(rs_graphics_core_maths_interpolate)
    UNIT_TEST(rs_graphics_core_maths_numeric_literals)

    // root-finding-test.cpp
    UNIT_TEST(rs_graphics_core_root_finding_newton_raphson)

    // linear-map-test.cpp
    UNIT_TEST(rs_graphics_core_linear_map)

    // vector-test.cpp
    UNIT_TEST(rs_graphics_core_integer_vector_construction)
    UNIT_TEST(rs_graphics_core_integer_vector_arithmetic)
    UNIT_TEST(rs_graphics_core_integer_vector_hash)
    UNIT_TEST(rs_graphics_core_floating_vector_construction)
    UNIT_TEST(rs_graphics_core_floating_vector_arithmetic)

    // multi-array-test.cpp
    UNIT_TEST(rs_graphics_core_multi_array_2d)
    UNIT_TEST(rs_graphics_core_multi_array_3d)

    // matrix-test.cpp
    UNIT_TEST(rs_graphics_core_matrix_basics)
    UNIT_TEST(rs_graphics_core_matrix_inversion)

    // quaternion-test.cpp
    UNIT_TEST(rs_graphics_core_quaternion)

    // transform-test.cpp
    UNIT_TEST(rs_graphics_core_2d_transforms)
    UNIT_TEST(rs_graphics_core_3d_transforms)
    UNIT_TEST(rs_graphics_core_projective_geometry)
    UNIT_TEST(rs_graphics_core_primitive_transforms)
    UNIT_TEST(rs_graphics_core_quaternion_transforms)

    // geometry-test.cpp
    UNIT_TEST(rs_graphics_core_geometry_box)
    UNIT_TEST(rs_graphics_core_geometry_sphere)

    // colour-space-test.cpp
    UNIT_TEST(rs_graphics_core_colour_space_srgb)

    // colour-test.cpp
    UNIT_TEST(rs_graphics_core_colour)

    // noise-test.cpp
    UNIT_TEST(rs_graphics_core_noise_result_stability)
    UNIT_TEST(rs_graphics_core_noise_multiple_sources)
    UNIT_TEST(rs_graphics_core_noise_statistics)

    // unit-test.cpp

    return RS::UnitTest::end_tests();

}
