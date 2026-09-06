#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include <cmath>

#include "../include/MatrixMath.hpp"

using namespace yarep::math;

namespace {
    constexpr float TEST_EPSILON = 0.0001f;

    bool test_near(
            const float lhs,
            const float rhs,
            const float epsilon = TEST_EPSILON) {
        return std::abs(lhs - rhs) <= epsilon;
    }

    void require_mat3_near(
            const Mat3& actual,
            const Mat3& expected,
            const float epsilon = TEST_EPSILON) {
        for (std::size_t row = 0; row < 3; ++row) {
            for (std::size_t column = 0; column < 3; ++column) {
                REQUIRE(
                        test_near(
                            actual(row, column),
                            expected(row, column),
                            epsilon)
                        );
            }
        }
    }

    void require_mat4_near(
            const Mat4& actual,
            const Mat4& expected,
            const float epsilon = TEST_EPSILON) {
        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column) {
                REQUIRE(
                        test_near(
                            actual(row, column),
                            expected(row, column),
                            epsilon)
                        );
            }
        }
    }

    void require_vec4_near(
            const Vec4& actual,
            const Vec4& expected,
            const float epsilon = TEST_EPSILON) {
        REQUIRE(test_near(actual.x, expected.x, epsilon));
        REQUIRE(test_near(actual.y, expected.y, epsilon));
        REQUIRE(test_near(actual.z, expected.z, epsilon));
        REQUIRE(test_near(actual.w, expected.w, epsilon));
    }
}


// ============================================================================
// Mat3 transpose
// ============================================================================

TEST_CASE("Mat3 transpose") {
    constexpr Mat3 matrix{
        Vec3{1, 4, 7},
        Vec3{2, 5, 8},
        Vec3{3, 6, 9}
    };

    constexpr Mat3 expected{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Vec3{7, 8, 9}
    };

    REQUIRE(transpose(matrix) == expected);
}

TEST_CASE("Mat3 transpose identity") {
    constexpr Mat3 identity = Mat3::identity();

    REQUIRE(transpose(identity) == identity);
}

TEST_CASE("Mat3 transpose twice returns original") {
    constexpr Mat3 matrix{
        Vec3{2, -3, 5},
        Vec3{7, 11, -13},
        Vec3{-17, 19, 23}
    };

    REQUIRE(transpose(transpose(matrix)) == matrix);
}


// ============================================================================
// Mat4 transpose
// ============================================================================

TEST_CASE("Mat4 transpose") {
    constexpr Mat4 matrix{
        Vec4{1, 5, 9, 13},
        Vec4{2, 6, 10, 14},
        Vec4{3, 7, 11, 15},
        Vec4{4, 8, 12, 16}
    };

    constexpr Mat4 expected{
        Vec4{1, 2, 3, 4},
        Vec4{5, 6, 7, 8},
        Vec4{9, 10, 11, 12},
        Vec4{13, 14, 15, 16}
    };

    REQUIRE(transpose(matrix) == expected);
}

TEST_CASE("Mat4 transpose identity") {
    constexpr Mat4 identity = Mat4::identity();

    REQUIRE(transpose(identity) == identity);
}

TEST_CASE("Mat4 transpose twice returns original") {
    constexpr Mat4 matrix{
        Vec4{2, -3, 5, 7},
        Vec4{11, -13, 17, 19},
        Vec4{-23, 29, 31, 37},
        Vec4{41, 43, -47, 53}
    };

    REQUIRE(transpose(transpose(matrix)) == matrix);
}


// ============================================================================
// Mat3 determinant
// ============================================================================

TEST_CASE("Mat3 determinant identity") {
    REQUIRE(
            test_near(
                determinant(Mat3::identity()),
                1.0f)
            );
}

TEST_CASE("Mat3 determinant zero matrix") {
    constexpr Mat3 matrix{};

    REQUIRE(
            test_near(
                determinant(matrix),
                0.0f)
            );
}

TEST_CASE("Mat3 determinant diagonal matrix") {
    constexpr Mat3 matrix{
        Vec3{2, 0, 0},
        Vec3{0, -3, 0},
        Vec3{0, 0, 4}
    };

    REQUIRE(
            test_near(
                determinant(matrix),
                -24.0f)
            );
}

TEST_CASE("Mat3 determinant general matrix") {
    // Rows:
    //
    // 1 2 3
    // 0 1 4
    // 5 6 0

    constexpr Mat3 matrix{
        Vec3{1, 0, 5},
        Vec3{2, 1, 6},
        Vec3{3, 4, 0}
    };

    REQUIRE(
            test_near(
                determinant(matrix),
                1.0f)
            );
}


// ============================================================================
// Mat4 determinant
// ============================================================================

TEST_CASE("Mat4 determinant identity") {
    REQUIRE(
            test_near(
                determinant(Mat4::identity()),
                1.0f)
            );
}

TEST_CASE("Mat4 determinant zero matrix") {
    constexpr Mat4 matrix{};

    REQUIRE(
            test_near(
                determinant(matrix),
                0.0f)
            );
}

TEST_CASE("Mat4 determinant diagonal matrix") {
    constexpr Mat4 matrix{
        Vec4{2, 0, 0, 0},
        Vec4{0, -3, 0, 0},
        Vec4{0, 0, 4, 0},
        Vec4{0, 0, 0, 5}
    };

    REQUIRE(
            test_near(
                determinant(matrix),
                -120.0f)
            );
}

TEST_CASE("Mat4 determinant general matrix") {
    // Rows:
    //
    // 1 2 3 4
    // 5 6 7 8
    // 2 6 4 8
    // 3 1 1 2

    constexpr Mat4 matrix{
        Vec4{1, 5, 2, 3},
        Vec4{2, 6, 6, 1},
        Vec4{3, 7, 4, 1},
        Vec4{4, 8, 8, 2}
    };

    REQUIRE(
            test_near(
                determinant(matrix),
                72.0f)
            );
}


// ============================================================================
// Mat3 inverse
// ============================================================================

TEST_CASE("Mat3 inverse identity") {
    const Mat3 result =
            inverse(Mat3::identity());

    require_mat3_near(
            result,
            Mat3::identity()
            );
}

TEST_CASE("Mat3 inverse general matrix") {
    constexpr Mat3 matrix{
        Vec3{1, 0, 5},
        Vec3{2, 1, 6},
        Vec3{3, 4, 0}
    };

    constexpr Mat3 expected{
        Vec3{-24, 20, -5},
        Vec3{18, -15, 4},
        Vec3{5, -4, 1}
    };

    const Mat3 result = inverse(matrix);

    require_mat3_near(result, expected);
}

TEST_CASE("Mat3 inverse multiplied by original returns identity") {
    constexpr Mat3 matrix{
        Vec3{1, 0, 5},
        Vec3{2, 1, 6},
        Vec3{3, 4, 0}
    };

    const Mat3 matrix_inverse =
            inverse(matrix);

    require_mat3_near(
            matrix * matrix_inverse,
            Mat3::identity()
            );

    require_mat3_near(
            matrix_inverse * matrix,
            Mat3::identity()
            );
}

TEST_CASE("Mat3 inverse singular matrix throws") {
    constexpr Mat3 matrix{
        Vec3{1, 2, 3},
        Vec3{2, 4, 6},
        Vec3{3, 6, 9}
    };

    REQUIRE_THROWS(inverse(matrix));
}


// ============================================================================
// Mat4 inverse
// ============================================================================

TEST_CASE("Mat4 inverse identity") {
    const Mat4 result =
            inverse(Mat4::identity());

    require_mat4_near(
            result,
            Mat4::identity()
            );
}

TEST_CASE("Mat4 inverse translation") {
    constexpr Vec3 translation{
        5,
        -3,
        8
    };

    const Mat4 matrix =
            translation_matrix(translation);

    const Mat4 matrix_inverse =
            inverse(matrix);

    const Mat4 expected =
            translation_matrix(
                    Vec3{-5, 3, -8}
                    );

    require_mat4_near(
            matrix_inverse,
            expected
            );
}

TEST_CASE("Mat4 inverse general matrix") {
    constexpr Mat4 matrix{
        Vec4{1, 5, 2, 3},
        Vec4{2, 6, 6, 1},
        Vec4{3, 7, 4, 1},
        Vec4{4, 8, 8, 2}
    };

    constexpr Mat4 expected{
        Vec4{
            -1.0f / 6.0f,
            -5.0f / 6.0f,
            1.0f / 6.0f,
            7.0f / 12.0f
        },
        Vec4{
            1.0f / 18.0f,
            5.0f / 18.0f,
            5.0f / 18.0f,
            -13.0f / 36.0f
        },
        Vec4{
            -1.0f / 18.0f,
            2.0f / 9.0f,
            -5.0f / 18.0f,
            1.0f / 9.0f
        },
        Vec4{
            1.0f / 3.0f,
            -1.0f / 3.0f,
            -1.0f / 3.0f,
            1.0f / 3.0f
        }
    };

    const Mat4 result =
            inverse(matrix);

    require_mat4_near(
            result,
            expected
            );
}

TEST_CASE("Mat4 inverse multiplied by original returns identity") {
    constexpr Mat4 matrix{
        Vec4{1, 5, 2, 3},
        Vec4{2, 6, 6, 1},
        Vec4{3, 7, 4, 1},
        Vec4{4, 8, 8, 2}
    };

    const Mat4 matrix_inverse =
            inverse(matrix);

    require_mat4_near(
            matrix * matrix_inverse,
            Mat4::identity()
            );

    require_mat4_near(
            matrix_inverse * matrix,
            Mat4::identity()
            );
}

TEST_CASE("Mat4 inverse singular matrix throws") {
    constexpr Mat4 matrix{
        Vec4{1, 2, 3, 4},
        Vec4{2, 4, 6, 8},
        Vec4{3, 6, 9, 12},
        Vec4{4, 8, 12, 16}
    };

    REQUIRE_THROWS(inverse(matrix));
}

TEST_CASE("Mat4 inverse accepts small but invertible scale") {
    const Mat4 matrix =
            scale_matrix(
                    Vec3{0.01f, 0.01f, 0.01f}
                    );

    REQUIRE_NOTHROW(
            inverse(matrix)
            );

    const Mat4 result =
            matrix * inverse(matrix);

    REQUIRE(
            nearly_equal(
                result,
                Mat4::identity(),
                0.0001f)
            );
}

// ============================================================================
// nearly_equal
// ============================================================================

TEST_CASE("Mat3 nearly equal") {
    constexpr Mat3 matrix{
        Vec3{1, 4, 7},
        Vec3{2, 5, 8},
        Vec3{3, 6, 9}
    };

    REQUIRE(
            nearly_equal(
                matrix,
                matrix,
                0.0001f)
            );
}

TEST_CASE("Mat3 nearly equal accepts values within epsilon") {
    Mat3 lhs = Mat3::identity();
    Mat3 rhs = lhs;

    rhs(1, 2) += 0.00005f;

    REQUIRE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}

TEST_CASE("Mat3 nearly equal rejects values outside epsilon") {
    Mat3 lhs = Mat3::identity();
    Mat3 rhs = lhs;

    rhs(1, 2) += 0.001f;

    REQUIRE_FALSE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}

TEST_CASE("Mat3 nearly equal includes epsilon boundary") {
    Mat3 lhs{};
    Mat3 rhs{};

    rhs(0, 0) = 0.0001f;

    REQUIRE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}

TEST_CASE("Mat4 nearly equal") {
    constexpr Mat4 matrix{
        Vec4{1, 5, 9, 13},
        Vec4{2, 6, 10, 14},
        Vec4{3, 7, 11, 15},
        Vec4{4, 8, 12, 16}
    };

    REQUIRE(
            nearly_equal(
                matrix,
                matrix,
                0.0001f)
            );
}

TEST_CASE("Mat4 nearly equal accepts values within epsilon") {
    Mat4 lhs = Mat4::identity();
    Mat4 rhs = lhs;

    rhs(3, 1) += 0.00005f;

    REQUIRE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}

TEST_CASE("Mat4 nearly equal rejects values outside epsilon") {
    Mat4 lhs = Mat4::identity();
    Mat4 rhs = lhs;

    rhs(3, 1) += 0.001f;

    REQUIRE_FALSE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}


// ============================================================================
// translation_matrix
// ============================================================================

TEST_CASE("Translation matrix zero translation is identity") {
    const Mat4 result =
            translation_matrix(
                    Vec3{0, 0, 0}
                    );

    REQUIRE(
            result == Mat4::identity()
            );
}

TEST_CASE("Translation matrix") {
    const Mat4 result =
            translation_matrix(
                    Vec3{5, -3, 8}
                    );

    constexpr Mat4 expected{
        Vec4{1, 0, 0, 0},
        Vec4{0, 1, 0, 0},
        Vec4{0, 0, 1, 0},
        Vec4{5, -3, 8, 1}
    };

    REQUIRE(result == expected);
}

TEST_CASE("Translation matrix transforms point") {
    const Mat4 matrix =
            translation_matrix(
                    Vec3{5, -3, 8}
                    );

    constexpr Vec4 point{
        1,
        2,
        3,
        1
    };

    REQUIRE(
            matrix * point
            == Vec4{6,
            -1,
            11,
            1}
            );
}

TEST_CASE("Translation matrix does not translate direction") {
    const Mat4 matrix =
            translation_matrix(
                    Vec3{5, -3, 8}
                    );

    constexpr Vec4 direction{
        1,
        2,
        3,
        0
    };

    REQUIRE(
            matrix * direction
            == direction
            );
}


// ============================================================================
// scale_matrix
// ============================================================================

TEST_CASE("Scale matrix unit scale is identity") {
    const Mat4 result =
            scale_matrix(
                    Vec3{1, 1, 1}
                    );

    REQUIRE(
            result == Mat4::identity()
            );
}

TEST_CASE("Scale matrix") {
    const Mat4 result =
            scale_matrix(
                    Vec3{2, 3, 4}
                    );

    constexpr Mat4 expected{
        Vec4{2, 0, 0, 0},
        Vec4{0, 3, 0, 0},
        Vec4{0, 0, 4, 0},
        Vec4{0, 0, 0, 1}
    };

    REQUIRE(result == expected);
}

TEST_CASE("Scale matrix transforms vector") {
    const Mat4 matrix =
            scale_matrix(
                    Vec3{2, 3, 4}
                    );

    constexpr Vec4 vector{
        1,
        -2,
        3,
        0
    };

    REQUIRE(
            matrix * vector
            == Vec4{2,
            -6,
            12,
            0}
            );
}

TEST_CASE("Scale matrix supports negative scale") {
    const Mat4 matrix =
            scale_matrix(
                    Vec3{-2, 3, -4}
                    );

    constexpr Vec4 vector{
        1,
        2,
        3,
        0
    };

    REQUIRE(
            matrix * vector
            == Vec4{-2,
            6,
            -12,
            0}
            );
}

TEST_CASE("Scale matrix supports zero scale") {
    const Mat4 matrix =
            scale_matrix(
                    Vec3{0, 0, 0}
                    );

    constexpr Vec4 point{
        5,
        -3,
        8,
        1
    };

    REQUIRE(
            matrix * point
            == Vec4{0,
            0,
            0,
            1}
            );
}


// ============================================================================
// rotation_matrix
// ============================================================================

TEST_CASE("Rotation matrix identity quaternion is identity") {
    const Mat4 result =
            rotation_matrix(
                    Quaternion::identity()
                    );

    require_mat4_near(
            result,
            Mat4::identity()
            );
}

TEST_CASE("Rotation matrix ninety degrees around Y") {
    constexpr float half_sqrt_two =
            0.70710678118f;

    constexpr Quaternion rotation{
        0,
        half_sqrt_two,
        0,
        half_sqrt_two
    };

    const Mat4 matrix =
            rotation_matrix(rotation);

    constexpr Vec4 x_axis{
        1,
        0,
        0,
        0
    };

    constexpr Vec4 y_axis{
        0,
        1,
        0,
        0
    };

    constexpr Vec4 z_axis{
        0,
        0,
        1,
        0
    };

    // Right-handed coordinate system:
    //
    // +90° around Y:
    //
    // +X -> -Z
    // +Z -> +X

    require_vec4_near(
            matrix * x_axis,
            Vec4{0, 0, -1, 0}
            );

    require_vec4_near(
            matrix * y_axis,
            y_axis
            );

    require_vec4_near(
            matrix * z_axis,
            Vec4{1, 0, 0, 0}
            );
}


// ============================================================================
// trs_matrix
// ============================================================================

TEST_CASE("TRS identity is identity") {
    const Mat4 result =
            trs_matrix(
                    Vec3{0, 0, 0},
                    Quaternion::identity(),
                    Vec3{1, 1, 1}
                    );

    require_mat4_near(
            result,
            Mat4::identity()
            );
}

TEST_CASE("TRS applies scale then rotation then translation") {
    constexpr float half_sqrt_two =
            0.70710678118f;

    constexpr Quaternion rotation{
        0,
        half_sqrt_two,
        0,
        half_sqrt_two
    };

    const Mat4 matrix =
            trs_matrix(
                    Vec3{10, 20, 30},
                    rotation,
                    Vec3{2, 3, 4}
                    );

    constexpr Vec4 point{
        1,
        2,
        3,
        1
    };

    // Scale:
    //
    // (1,2,3) -> (2,6,12)
    //
    // Rotate +90° around Y:
    //
    // -> (12,6,-2)
    //
    // Translate:
    //
    // -> (22,26,28)

    require_vec4_near(
            matrix * point,
            Vec4{22, 26, 28, 1}
            );
}

TEST_CASE("TRS equals explicit matrix composition") {
    constexpr float half_sqrt_two =
            0.70710678118f;

    constexpr Quaternion rotation{
        0,
        half_sqrt_two,
        0,
        half_sqrt_two
    };

    constexpr Vec3 translation{
        10,
        20,
        30
    };

    constexpr Vec3 scale{
        2,
        3,
        4
    };

    const Mat4 result =
            trs_matrix(
                    translation,
                    rotation,
                    scale
                    );

    const Mat4 expected =
            translation_matrix(translation)
            * rotation_matrix(rotation)
            * scale_matrix(scale);

    require_mat4_near(
            result,
            expected
            );
}


// ============================================================================
// look_at
// ============================================================================

TEST_CASE("Look at default OpenGL camera is identity") {
    const Mat4 result =
            look_at(
                    Vec3{0, 0, 0},
                    Vec3{0, 0, -1},
                    Vec3{0, 1, 0}
                    );

    require_mat4_near(
            result,
            Mat4::identity()
            );
}

TEST_CASE("Look at moves camera position to origin") {
    constexpr Vec3 eye{
        0,
        0,
        5
    };

    const Mat4 view =
            look_at(
                    eye,
                    Vec3{0, 0, 0},
                    Vec3{0, 1, 0}
                    );

    constexpr Vec4 eye_point{
        0,
        0,
        5,
        1
    };

    require_vec4_near(
            view * eye_point,
            Vec4{0, 0, 0, 1}
            );
}

TEST_CASE("Look at places target in front of camera") {
    const Mat4 view =
            look_at(
                    Vec3{0, 0, 5},
                    Vec3{0, 0, 0},
                    Vec3{0, 1, 0}
                    );

    constexpr Vec4 target{
        0,
        0,
        0,
        1
    };

    require_vec4_near(
            view * target,
            Vec4{0, 0, -5, 1}
            );
}

TEST_CASE("Look at rotates world into camera space") {
    const Mat4 view =
            look_at(
                    Vec3{0, 0, 0},
                    Vec3{1, 0, 0},
                    Vec3{0, 1, 0}
                    );

    constexpr Vec4 target{
        1,
        0,
        0,
        1
    };

    require_vec4_near(
            view * target,
            Vec4{0, 0, -1, 1}
            );

    constexpr Vec4 world_up{
        0,
        1,
        0,
        0
    };

    require_vec4_near(
            view * world_up,
            Vec4{0, 1, 0, 0}
            );
}

TEST_CASE("Look at throws when eye equals target") {
    REQUIRE_THROWS(
            look_at(
                Vec3{1, 2, 3},
                Vec3{1, 2, 3},
                Vec3{0, 1, 0})
            );
}

TEST_CASE("Look at throws for zero up vector") {
    REQUIRE_THROWS(
            look_at(
                Vec3{0, 0, 0},
                Vec3{0, 0, -1},
                Vec3{0, 0, 0})
            );
}

TEST_CASE("Look at throws when up is parallel to view direction") {
    REQUIRE_THROWS(
            look_at(
                Vec3{0, 0, 0},
                Vec3{0, 1, 0},
                Vec3{0, 1, 0})
            );
}


// ============================================================================
// perspective
// ============================================================================

TEST_CASE("Perspective matrix known values") {
    const Mat4 matrix =
            perspective(
                    Angle::from_degrees(90),
                    2.0f,
                    1.0f,
                    11.0f
                    );

    Mat4 expected{};

    expected(0, 0) = 0.5f;
    expected(1, 1) = 1.0f;

    expected(2, 2) = -1.2f;
    expected(2, 3) = -2.2f;

    expected(3, 2) = -1.0f;

    require_mat4_near(
            matrix,
            expected
            );
}

TEST_CASE("Perspective maps near plane to negative one NDC") {
    const Mat4 matrix =
            perspective(
                    Angle::from_degrees(90),
                    2.0f,
                    1.0f,
                    11.0f
                    );

    constexpr Vec4 point{
        0,
        0,
        -1,
        1
    };

    const Vec4 clip =
            matrix * point;

    const float ndc_z =
            clip.z / clip.w;

    REQUIRE(
            test_near(
                ndc_z,
                -1.0f)
            );
}

TEST_CASE("Perspective maps far plane to positive one NDC") {
    const Mat4 matrix =
            perspective(
                    Angle::from_degrees(90),
                    2.0f,
                    1.0f,
                    11.0f
                    );

    constexpr Vec4 point{
        0,
        0,
        -11,
        1
    };

    const Vec4 clip =
            matrix * point;

    const float ndc_z =
            clip.z / clip.w;

    REQUIRE(
            test_near(
                ndc_z,
                1.0f)
            );
}

TEST_CASE("Perspective respects vertical field of view") {
    const Mat4 matrix =
            perspective(
                    Angle::from_degrees(90),
                    2.0f,
                    1.0f,
                    11.0f
                    );

    constexpr Vec4 top_edge{
        0,
        1,
        -1,
        1
    };

    const Vec4 clip =
            matrix * top_edge;

    REQUIRE(
            test_near(
                clip.y / clip.w,
                1.0f)
            );
}

TEST_CASE("Perspective respects aspect ratio") {
    const Mat4 matrix =
            perspective(
                    Angle::from_degrees(90),
                    2.0f,
                    1.0f,
                    11.0f
                    );

    constexpr Vec4 right_edge{
        2,
        0,
        -1,
        1
    };

    const Vec4 clip =
            matrix * right_edge;

    REQUIRE(
            test_near(
                clip.x / clip.w,
                1.0f)
            );
}

TEST_CASE("Perspective throws for zero aspect ratio") {
    REQUIRE_THROWS(
            perspective(
                Angle::from_degrees(90),
                0.0f,
                0.1f,
                100.0f)
            );
}

TEST_CASE("Perspective throws for negative aspect ratio") {
    REQUIRE_THROWS(
            perspective(
                Angle::from_degrees(90),
                -1.0f,
                0.1f,
                100.0f)
            );
}

TEST_CASE("Perspective throws for invalid near plane") {
    REQUIRE_THROWS(
            perspective(
                Angle::from_degrees(90),
                1.0f,
                0.0f,
                100.0f)
            );

    REQUIRE_THROWS(
            perspective(
                Angle::from_degrees(90),
                1.0f,
                -1.0f,
                100.0f)
            );
}

TEST_CASE("Perspective throws when far plane is not beyond near plane") {
    REQUIRE_THROWS(
            perspective(
                Angle::from_degrees(90),
                1.0f,
                10.0f,
                10.0f)
            );

    REQUIRE_THROWS(
            perspective(
                Angle::from_degrees(90),
                1.0f,
                10.0f,
                5.0f)
            );
}

TEST_CASE("Perspective throws for invalid field of view") {
    REQUIRE_THROWS(
            perspective(
                Angle::from_degrees(0),
                1.0f,
                0.1f,
                100.0f)
            );

    REQUIRE_THROWS(
            perspective(
                Angle::from_degrees(180),
                1.0f,
                0.1f,
                100.0f)
            );
}


// ============================================================================
// orthographic
// ============================================================================

TEST_CASE("Orthographic matrix known values") {
    const Mat4 matrix =
            orthographic(
                    -2,
                    6,
                    -4,
                    4,
                    1,
                    11
                    );

    Mat4 expected{};

    expected(0, 0) = 0.25f;
    expected(1, 1) = 0.25f;
    expected(2, 2) = -0.2f;

    expected(0, 3) = -0.5f;
    expected(1, 3) = 0.0f;
    expected(2, 3) = -1.2f;

    expected(3, 3) = 1.0f;

    require_mat4_near(
            matrix,
            expected
            );
}

TEST_CASE("Orthographic maps minimum corner to negative one NDC") {
    const Mat4 matrix =
            orthographic(
                    -2,
                    6,
                    -4,
                    4,
                    1,
                    11
                    );

    constexpr Vec4 point{
        -2,
        -4,
        -1,
        1
    };

    require_vec4_near(
            matrix * point,
            Vec4{-1, -1, -1, 1}
            );
}

TEST_CASE("Orthographic maps maximum corner to positive one NDC") {
    const Mat4 matrix =
            orthographic(
                    -2,
                    6,
                    -4,
                    4,
                    1,
                    11
                    );

    constexpr Vec4 point{
        6,
        4,
        -11,
        1
    };

    require_vec4_near(
            matrix * point,
            Vec4{1, 1, 1, 1}
            );
}

TEST_CASE("Orthographic maps center to origin") {
    const Mat4 matrix =
            orthographic(
                    -2,
                    6,
                    -4,
                    4,
                    1,
                    11
                    );

    // Center:
    //
    // x = 2
    // y = 0
    // z = -6

    constexpr Vec4 point{
        2,
        0,
        -6,
        1
    };

    require_vec4_near(
            matrix * point,
            Vec4{0, 0, 0, 1}
            );
}

TEST_CASE("Orthographic throws for zero horizontal range") {
    REQUIRE_THROWS(
            orthographic(
                5,
                5,
                -1,
                1,
                0.1f,
                100.0f)
            );
}

TEST_CASE("Orthographic throws for zero vertical range") {
    REQUIRE_THROWS(
            orthographic(
                -1,
                1,
                5,
                5,
                0.1f,
                100.0f)
            );
}

TEST_CASE("Orthographic throws for zero depth range") {
    REQUIRE_THROWS(
            orthographic(
                -1,
                1,
                -1,
                1,
                10,
                10)
            );
}


// ============================================================================
// normal_matrix
// ============================================================================

TEST_CASE("Normal matrix identity") {
    const Mat3 result =
            normal_matrix(
                    Mat4::identity()
                    );

    require_mat3_near(
            result,
            Mat3::identity()
            );
}

TEST_CASE("Normal matrix ignores translation") {
    const Mat4 model =
            translation_matrix(
                    Vec3{10, 20, 30}
                    );

    const Mat3 result =
            normal_matrix(model);

    require_mat3_near(
            result,
            Mat3::identity()
            );
}

TEST_CASE("Normal matrix handles non uniform scale") {
    const Mat4 model =
            scale_matrix(
                    Vec3{2, 3, 4}
                    );

    const Mat3 result =
            normal_matrix(model);

    constexpr Mat3 expected{
        Vec3{0.5f, 0, 0},
        Vec3{0, 1.0f / 3.0f, 0},
        Vec3{0, 0, 0.25f}
    };

    require_mat3_near(
            result,
            expected
            );
}

TEST_CASE("Normal matrix handles rotation and non uniform scale") {
    constexpr float half_sqrt_two =
            0.70710678118f;

    constexpr Quaternion rotation{
        0,
        half_sqrt_two,
        0,
        half_sqrt_two
    };

    const Mat4 model =
            rotation_matrix(rotation)
            * scale_matrix(
                    Vec3{2, 3, 4}
                    );

    const Mat3 result =
            normal_matrix(model);

    constexpr Mat3 expected{
        Vec3{0, 0, -0.5f},
        Vec3{0, 1.0f / 3.0f, 0},
        Vec3{0.25f, 0, 0}
    };

    require_mat3_near(
            result,
            expected
            );
}

TEST_CASE("Normal matrix throws for non invertible model matrix") {
    const Mat4 model =
            scale_matrix(
                    Vec3{1, 0, 1}
                    );

    REQUIRE_THROWS(
            normal_matrix(model)
            );
}
