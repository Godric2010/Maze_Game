#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include <cmath>

#include "../include/QuaternionMath.hpp"
#include "../include/VectorMath.hpp"

using namespace yarep::math;

namespace {
    constexpr float TEST_EPSILON = 0.0001f;

    bool test_near(
            const float lhs,
            const float rhs,
            const float epsilon = TEST_EPSILON) {
        return std::abs(lhs - rhs) <= epsilon;
    }

    void require_quaternion_components_near(
            const Quaternion& actual,
            const Quaternion& expected,
            const float epsilon = TEST_EPSILON) {
        REQUIRE(test_near(actual.x, expected.x, epsilon));
        REQUIRE(test_near(actual.y, expected.y, epsilon));
        REQUIRE(test_near(actual.z, expected.z, epsilon));
        REQUIRE(test_near(actual.w, expected.w, epsilon));
    }

    void require_vec3_near(
            const Vec3& actual,
            const Vec3& expected,
            const float epsilon = TEST_EPSILON) {
        REQUIRE(test_near(actual.x, expected.x, epsilon));
        REQUIRE(test_near(actual.y, expected.y, epsilon));
        REQUIRE(test_near(actual.z, expected.z, epsilon));
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

    Quaternion negated(const Quaternion& q) {
        return {
            -q.x,
            -q.y,
            -q.z,
            -q.w
        };
    }
}


// ============================================================================
// length_squared
// ============================================================================

TEST_CASE("Quaternion length squared identity") {
    REQUIRE(
            length_squared(
                Quaternion::identity())
            == 1.0f
            );
}

TEST_CASE("Quaternion length squared") {
    constexpr Quaternion quaternion{
        1,
        2,
        3,
        4
    };

    REQUIRE(
            length_squared(quaternion)
            == 30.0f
            );
}

TEST_CASE("Quaternion length squared ignores component signs") {
    constexpr Quaternion positive{
        1,
        2,
        3,
        4
    };

    constexpr Quaternion negative{
        -1,
        -2,
        -3,
        -4
    };

    REQUIRE(
            length_squared(positive)
            == length_squared(negative)
            );
}

TEST_CASE("Quaternion length squared zero quaternion") {
    constexpr Quaternion zero{
        0,
        0,
        0,
        0
    };

    REQUIRE(
            length_squared(zero)
            == 0.0f
            );
}


// ============================================================================
// length
// ============================================================================

TEST_CASE("Quaternion length identity") {
    REQUIRE(
            test_near(
                length(Quaternion::identity()),
                1.0f)
            );
}

TEST_CASE("Quaternion length") {
    constexpr Quaternion quaternion{
        1,
        2,
        3,
        4
    };

    REQUIRE(
            test_near(
                length(quaternion),
                std::sqrt(30.0f))
            );
}

TEST_CASE("Quaternion length zero quaternion") {
    constexpr Quaternion zero{
        0,
        0,
        0,
        0
    };

    REQUIRE(
            test_near(
                length(zero),
                0.0f)
            );
}


// ============================================================================
// normalize
// ============================================================================

TEST_CASE("Quaternion normalize identity") {
    const Quaternion result =
            normalize(
                    Quaternion::identity()
                    );

    require_quaternion_components_near(
            result,
            Quaternion::identity()
            );
}

TEST_CASE("Quaternion normalize") {
    constexpr Quaternion quaternion{
        1,
        2,
        3,
        4
    };

    const Quaternion result =
            normalize(quaternion);

    const float expected_length =
            std::sqrt(30.0f);

    require_quaternion_components_near(
            result,
            Quaternion{
                1.0f / expected_length,
                2.0f / expected_length,
                3.0f / expected_length,
                4.0f / expected_length
            }
            );

    REQUIRE(
            test_near(
                length(result),
                1.0f)
            );
}

TEST_CASE("Quaternion normalize negative quaternion") {
    constexpr Quaternion quaternion{
        -1,
        -2,
        -3,
        -4
    };

    const Quaternion result =
            normalize(quaternion);

    REQUIRE(
            test_near(
                length(result),
                1.0f)
            );

    REQUIRE(result.x < 0);
    REQUIRE(result.y < 0);
    REQUIRE(result.z < 0);
    REQUIRE(result.w < 0);
}

TEST_CASE("Quaternion normalize already normalized quaternion") {
    constexpr Quaternion quaternion{
        0,
        0,
        0,
        1
    };

    const Quaternion result =
            normalize(quaternion);

    require_quaternion_components_near(
            result,
            quaternion
            );
}

TEST_CASE("Quaternion normalize zero quaternion throws") {
    constexpr Quaternion zero{
        0,
        0,
        0,
        0
    };

    REQUIRE_THROWS(
            normalize(zero)
            );
}


// ============================================================================
// dot
// ============================================================================

TEST_CASE("Quaternion dot") {
    constexpr Quaternion a{
        1,
        2,
        3,
        4
    };

    constexpr Quaternion b{
        5,
        6,
        7,
        8
    };

    REQUIRE(
            dot(a, b)
            == 70.0f
            );
}

TEST_CASE("Quaternion dot is symmetric") {
    constexpr Quaternion a{
        1,
        -2,
        3,
        -4
    };

    constexpr Quaternion b{
        -5,
        6,
        -7,
        8
    };

    REQUIRE(
            dot(a, b)
            == dot(b, a)
            );
}

TEST_CASE("Quaternion dot with itself equals length squared") {
    constexpr Quaternion quaternion{
        1,
        2,
        3,
        4
    };

    REQUIRE(
            dot(quaternion, quaternion)
            == length_squared(quaternion)
            );
}

TEST_CASE("Quaternion dot orthogonal quaternions") {
    constexpr Quaternion identity{
        0,
        0,
        0,
        1
    };

    constexpr Quaternion x_rotation{
        1,
        0,
        0,
        0
    };

    REQUIRE(
            dot(identity, x_rotation)
            == 0.0f
            );
}


// ============================================================================
// conjugate
// ============================================================================

TEST_CASE("Quaternion conjugate identity") {
    const Quaternion result =
            conjugate(
                    Quaternion::identity()
                    );

    REQUIRE(
            result
            == Quaternion::identity()
            );
}

TEST_CASE("Quaternion conjugate") {
    constexpr Quaternion quaternion{
        1,
        2,
        3,
        4
    };

    const Quaternion result =
            conjugate(quaternion);

    REQUIRE(
            result
            == Quaternion{
            -1,
            -2,
            -3,
            4
            }
            );
}

TEST_CASE("Quaternion conjugate twice returns original") {
    constexpr Quaternion quaternion{
        1,
        -2,
        3,
        -4
    };

    REQUIRE(
            conjugate(conjugate(quaternion))
            == quaternion
            );
}


// ============================================================================
// inverse
// ============================================================================

TEST_CASE("Quaternion inverse identity") {
    const Quaternion result =
            inverse(
                    Quaternion::identity()
                    );

    require_quaternion_components_near(
            result,
            Quaternion::identity()
            );
}

TEST_CASE("Quaternion inverse") {
    constexpr Quaternion quaternion{
        1,
        2,
        3,
        4
    };

    const Quaternion result =
            inverse(quaternion);

    require_quaternion_components_near(
            result,
            Quaternion{
                -1.0f / 30.0f,
                -2.0f / 30.0f,
                -3.0f / 30.0f,
                4.0f / 30.0f
            }
            );
}

TEST_CASE("Quaternion multiplied by inverse returns identity") {
    constexpr Quaternion quaternion{
        1,
        2,
        3,
        4
    };

    const Quaternion quaternion_inverse =
            inverse(quaternion);

    require_quaternion_components_near(
            quaternion * quaternion_inverse,
            Quaternion::identity()
            );

    require_quaternion_components_near(
            quaternion_inverse * quaternion,
            Quaternion::identity()
            );
}

TEST_CASE("Quaternion inverse of unit quaternion equals conjugate") {
    constexpr float half_sqrt_two =
            0.70710678118f;

    constexpr Quaternion quaternion{
        0,
        half_sqrt_two,
        0,
        half_sqrt_two
    };

    require_quaternion_components_near(
            inverse(quaternion),
            conjugate(quaternion)
            );
}

TEST_CASE("Quaternion inverse zero quaternion throws") {
    constexpr Quaternion zero{
        0,
        0,
        0,
        0
    };

    REQUIRE_THROWS(
            inverse(zero)
            );
}


// ============================================================================
// from_axis_angle
// ============================================================================

TEST_CASE("Quaternion from axis angle zero degrees is identity") {
    const Quaternion result =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(0)
                    );

    require_quaternion_components_near(
            result,
            Quaternion::identity()
            );
}

TEST_CASE("Quaternion from axis angle ninety degrees around Y") {
    constexpr float half_sqrt_two =
            0.70710678118f;

    const Quaternion result =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    require_quaternion_components_near(
            result,
            Quaternion{
                0,
                half_sqrt_two,
                0,
                half_sqrt_two
            }
            );
}

TEST_CASE("Quaternion from axis angle ninety degrees around X") {
    constexpr float half_sqrt_two =
            0.70710678118f;

    const Quaternion result =
            from_axis_angle(
                    Vec3{1, 0, 0},
                    Angle::from_degrees(90)
                    );

    require_quaternion_components_near(
            result,
            Quaternion{
                half_sqrt_two,
                0,
                0,
                half_sqrt_two
            }
            );
}

TEST_CASE("Quaternion from axis angle negative angle") {
    constexpr float half_sqrt_two =
            0.70710678118f;

    const Quaternion result =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(-90)
                    );

    require_quaternion_components_near(
            result,
            Quaternion{
                0,
                -half_sqrt_two,
                0,
                half_sqrt_two
            }
            );
}

TEST_CASE("Quaternion from axis angle normalizes axis") {
    const Quaternion unit_axis =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Quaternion non_unit_axis =
            from_axis_angle(
                    Vec3{0, 5, 0},
                    Angle::from_degrees(90)
                    );

    require_quaternion_components_near(
            non_unit_axis,
            unit_axis
            );
}

TEST_CASE("Quaternion from axis angle returns unit quaternion") {
    const Quaternion result =
            from_axis_angle(
                    Vec3{2, -4, 6},
                    Angle::from_degrees(73)
                    );

    REQUIRE(
            test_near(
                length(result),
                1.0f)
            );
}

TEST_CASE("Quaternion from axis angle full rotation represents identity") {
    const Quaternion result =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(360)
                    );

    REQUIRE(
            nearly_equal(
                result,
                Quaternion::identity(),
                TEST_EPSILON)
            );
}

TEST_CASE("Quaternion from axis angle zero axis throws") {
    REQUIRE_THROWS(
            from_axis_angle(
                Vec3{0, 0, 0},
                Angle::from_degrees(90))
            );
}


// ============================================================================
// rotate
// ============================================================================

TEST_CASE("Quaternion identity rotation leaves vector unchanged") {
    constexpr Vec3 vector{
        1,
        2,
        3
    };

    const Vec3 result =
            rotate(
                    Quaternion::identity(),
                    vector
                    );

    require_vec3_near(
            result,
            vector
            );
}

TEST_CASE("Quaternion rotates X axis ninety degrees around Y") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Vec3 result =
            rotate(
                    rotation,
                    Vec3{1, 0, 0}
                    );

    // Right-handed:
    //
    // +90° around Y
    //
    // +X -> -Z

    require_vec3_near(
            result,
            Vec3{0, 0, -1}
            );
}

TEST_CASE("Quaternion rotates Z axis ninety degrees around Y") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Vec3 result =
            rotate(
                    rotation,
                    Vec3{0, 0, 1}
                    );

    require_vec3_near(
            result,
            Vec3{1, 0, 0}
            );
}

TEST_CASE("Quaternion rotation followed by inverse restores vector") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{1, 2, 3},
                    Angle::from_degrees(73)
                    );

    constexpr Vec3 vector{
        4,
        -2,
        7
    };

    const Vec3 rotated =
            rotate(
                    rotation,
                    vector
                    );

    const Vec3 restored =
            rotate(
                    inverse(rotation),
                    rotated
                    );

    require_vec3_near(
            restored,
            vector
            );
}

TEST_CASE("Quaternion rotation preserves vector length") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{2, 3, -1},
                    Angle::from_degrees(123)
                    );

    constexpr Vec3 vector{
        4,
        -7,
        2
    };

    const Vec3 result =
            rotate(
                    rotation,
                    vector
                    );

    REQUIRE(
            test_near(
                length(result),
                length(vector))
            );
}

TEST_CASE("Quaternion rotation leaves zero vector unchanged") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{1, 1, 1},
                    Angle::from_degrees(42)
                    );

    const Vec3 result =
            rotate(
                    rotation,
                    Vec3{0, 0, 0}
                    );

    REQUIRE(
            result
            == Vec3{0,
            0,
            0}
            );
}


// ============================================================================
// to_mat3
// ============================================================================

TEST_CASE("Quaternion identity converts to Mat3 identity") {
    const Mat3 result =
            to_mat3(
                    Quaternion::identity()
                    );

    require_mat3_near(
            result,
            Mat3::identity()
            );
}

TEST_CASE("Quaternion ninety degrees around Y converts to Mat3") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Mat3 result =
            to_mat3(rotation);

    constexpr Mat3 expected{
        Vec3{0, 0, -1},
        Vec3{0, 1, 0},
        Vec3{1, 0, 0}
    };

    require_mat3_near(
            result,
            expected
            );
}

TEST_CASE("Quaternion Mat3 rotation matches quaternion rotation") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{1, 2, 3},
                    Angle::from_degrees(57)
                    );

    constexpr Vec3 vector{
        4,
        -2,
        7
    };

    const Vec3 quaternion_result =
            rotate(
                    rotation,
                    vector
                    );

    const Vec3 matrix_result =
            to_mat3(rotation) * vector;

    require_vec3_near(
            matrix_result,
            quaternion_result
            );
}


// ============================================================================
// to_mat4
// ============================================================================

TEST_CASE("Quaternion identity converts to Mat4 identity") {
    const Mat4 result =
            to_mat4(
                    Quaternion::identity()
                    );

    require_mat4_near(
            result,
            Mat4::identity()
            );
}

TEST_CASE("Quaternion ninety degrees around Y converts to Mat4") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Mat4 result =
            to_mat4(rotation);

    constexpr Mat4 expected{
        Vec4{0, 0, -1, 0},
        Vec4{0, 1, 0, 0},
        Vec4{1, 0, 0, 0},
        Vec4{0, 0, 0, 1}
    };

    require_mat4_near(
            result,
            expected
            );
}

TEST_CASE("Quaternion Mat4 does not introduce translation") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Mat4 matrix =
            to_mat4(rotation);

    REQUIRE(
            test_near(
                matrix(0, 3),
                0.0f)
            );

    REQUIRE(
            test_near(
                matrix(1, 3),
                0.0f)
            );

    REQUIRE(
            test_near(
                matrix(2, 3),
                0.0f)
            );

    REQUIRE(
            test_near(
                matrix(3, 3),
                1.0f)
            );
}


// ============================================================================
// nearly_equal
// ============================================================================

TEST_CASE("Quaternion nearly equal recognizes identical quaternions") {
    const Quaternion quaternion =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    REQUIRE(
            nearly_equal(
                quaternion,
                quaternion,
                TEST_EPSILON)
            );
}

TEST_CASE("Quaternion nearly equal accepts components within epsilon") {
    constexpr Quaternion lhs{
        0,
        0,
        0,
        1
    };

    constexpr Quaternion rhs{
        0.00005f,
        -0.00005f,
        0.00005f,
        0.99995f
    };

    REQUIRE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}

TEST_CASE("Quaternion nearly equal rejects components outside epsilon") {
    constexpr Quaternion lhs{
        0,
        0,
        0,
        1
    };

    constexpr Quaternion rhs{
        0.001f,
        0,
        0,
        1
    };

    REQUIRE_FALSE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}

TEST_CASE("Quaternion nearly equal recognizes negated quaternion as same rotation") {
    const Quaternion quaternion =
            from_axis_angle(
                    Vec3{1, 2, 3},
                    Angle::from_degrees(73)
                    );

    const Quaternion negative =
            negated(quaternion);

    REQUIRE(
            nearly_equal(
                quaternion,
                negative,
                TEST_EPSILON)
            );
}

TEST_CASE("Quaternion nearly equal rejects different rotations") {
    const Quaternion a =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(45)
                    );

    const Quaternion b =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(50)
                    );

    REQUIRE_FALSE(
            nearly_equal(
                a,
                b,
                TEST_EPSILON)
            );
}

TEST_CASE("Quaternion nearly equal rejects mixed signs") {
    constexpr Quaternion lhs{
        0.5f,
        0.5f,
        0.5f,
        0.5f
    };

    constexpr Quaternion rhs{
        0.5f,
        -0.5f,
        0.5f,
        -0.5f
    };

    REQUIRE_FALSE(
        nearly_equal(
            lhs,
            rhs,
            TEST_EPSILON));
}
// ============================================================================
// slerp
// ============================================================================

TEST_CASE("Quaternion slerp returns start at zero") {
    const Quaternion from =
            Quaternion::identity();

    const Quaternion to =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Quaternion result =
            slerp(
                    from,
                    to,
                    0.0f
                    );

    REQUIRE(
            nearly_equal(
                result,
                from,
                TEST_EPSILON)
            );
}

TEST_CASE("Quaternion slerp returns target at one") {
    const Quaternion from =
            Quaternion::identity();

    const Quaternion to =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Quaternion result =
            slerp(
                    from,
                    to,
                    1.0f
                    );

    REQUIRE(
            nearly_equal(
                result,
                to,
                TEST_EPSILON)
            );
}

TEST_CASE("Quaternion slerp halfway between identity and 180 degrees") {
    constexpr Quaternion from{
        0,
        0,
        0,
        1
    };

    constexpr Quaternion to{
        0,
        1,
        0,
        0
    };

    constexpr float half_sqrt_two =
            0.70710678118f;

    const Quaternion result =
            slerp(
                    from,
                    to,
                    0.5f
                    );

    require_quaternion_components_near(
            result,
            Quaternion{
                0,
                half_sqrt_two,
                0,
                half_sqrt_two
            }
            );
}

TEST_CASE("Quaternion slerp produces unit quaternion") {
    const Quaternion from =
            from_axis_angle(
                    Vec3{1, 0, 0},
                    Angle::from_degrees(15)
                    );

    const Quaternion to =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(130)
                    );

    const Quaternion result =
            slerp(
                    from,
                    to,
                    0.37f
                    );

    REQUIRE(
            test_near(
                length(result),
                1.0f)
            );
}

TEST_CASE("Quaternion slerp uses shortest path for negated quaternion") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Quaternion same_rotation =
            negated(rotation);

    const Quaternion result =
            slerp(
                    rotation,
                    same_rotation,
                    0.5f
                    );

    REQUIRE(
            nearly_equal(
                result,
                rotation,
                TEST_EPSILON)
            );
}

TEST_CASE("Quaternion slerp between equal rotations does not change rotation") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{2, 3, 4},
                    Angle::from_degrees(45)
                    );

    const Quaternion result =
            slerp(
                    rotation,
                    rotation,
                    0.5f
                    );

    REQUIRE(
            nearly_equal(
                result,
                rotation,
                TEST_EPSILON)
            );
}

TEST_CASE("Quaternion slerp midpoint is symmetric") {
    const Quaternion a =
            from_axis_angle(
                    Vec3{1, 0, 0},
                    Angle::from_degrees(20)
                    );

    const Quaternion b =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(70)
                    );

    const Quaternion ab =
            slerp(
                    a,
                    b,
                    0.5f
                    );

    const Quaternion ba =
            slerp(
                    b,
                    a,
                    0.5f
                    );

    REQUIRE(
            nearly_equal(
                ab,
                ba,
                TEST_EPSILON)
            );
}

TEST_CASE("Quaternion slerp supports extrapolation") {
    const Quaternion from =
            Quaternion::identity();

    const Quaternion to =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Quaternion expected =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(135)
                    );

    const Quaternion result =
            slerp(
                    from,
                    to,
                    1.5f
                    );

    REQUIRE(
            nearly_equal(
                result,
                expected,
                TEST_EPSILON)
            );
}
