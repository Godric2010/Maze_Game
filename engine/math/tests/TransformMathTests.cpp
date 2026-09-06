#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include <cmath>

#include "../include/MatrixMath.hpp"
#include "../include/QuaternionMath.hpp"
#include "../include/TransformMath.hpp"
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

    void require_vec3_near(
            const Vec3& actual,
            const Vec3& expected,
            const float epsilon = TEST_EPSILON) {
        REQUIRE(test_near(actual.x, expected.x, epsilon));
        REQUIRE(test_near(actual.y, expected.y, epsilon));
        REQUIRE(test_near(actual.z, expected.z, epsilon));
    }

    void require_mat4_near(
            const Mat4& actual,
            const Mat4& expected,
            const float epsilon = TEST_EPSILON) {
        for (std::size_t row = 0; row < 4; ++row) {
            for (
                std::size_t column = 0;
                column < 4;
                ++column) {
                REQUIRE(
                        test_near(
                            actual(row, column),
                            expected(row, column),
                            epsilon)
                        );
            }
        }
    }
}


// ============================================================================
// to_matrix
// ============================================================================

TEST_CASE("Transform identity converts to identity matrix") {
    const Mat4 matrix =
            to_matrix(
                    Transform::identity()
                    );

    require_mat4_near(
            matrix,
            Mat4::identity()
            );
}


TEST_CASE("Transform translation converts to matrix") {
    const Transform transform{
        Vec3{5, -3, 8},
        Quaternion::identity(),
        Vec3{1, 1, 1}
    };

    const Mat4 matrix =
            to_matrix(transform);

    const Mat4 expected =
            translation_matrix(
                    Vec3{5, -3, 8}
                    );

    require_mat4_near(
            matrix,
            expected
            );
}


TEST_CASE("Transform scale converts to matrix") {
    const Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{2, 3, 4}
    };

    const Mat4 matrix =
            to_matrix(transform);

    const Mat4 expected =
            scale_matrix(
                    Vec3{2, 3, 4}
                    );

    require_mat4_near(
            matrix,
            expected
            );
}


TEST_CASE("Transform rotation converts to matrix") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Transform transform{
        Vec3{0, 0, 0},
        rotation,
        Vec3{1, 1, 1}
    };

    const Mat4 matrix =
            to_matrix(transform);

    const Mat4 expected =
            rotation_matrix(rotation);

    require_mat4_near(
            matrix,
            expected
            );
}


TEST_CASE("Transform converts to TRS matrix") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Transform transform{
        Vec3{10, 20, 30},
        rotation,
        Vec3{2, 3, 4}
    };

    const Mat4 matrix =
            to_matrix(transform);

    const Mat4 expected =
            trs_matrix(
                    transform.position,
                    transform.rotation,
                    transform.scale
                    );

    require_mat4_near(
            matrix,
            expected
            );
}


// ============================================================================
// transform_point
// ============================================================================

TEST_CASE("Transform point with identity leaves point unchanged") {
    constexpr Vec3 point{
        1,
        2,
        3
    };

    const Vec3 result =
            transform_point(
                    Transform::identity(),
                    point
                    );

    require_vec3_near(
            result,
            point
            );
}


TEST_CASE("Transform point applies translation") {
    const Transform transform{
        Vec3{5, -3, 8},
        Quaternion::identity(),
        Vec3{1, 1, 1}
    };

    const Vec3 result =
            transform_point(
                    transform,
                    Vec3{1, 2, 3}
                    );

    require_vec3_near(
            result,
            Vec3{6, -1, 11}
            );
}


TEST_CASE("Transform point applies scale") {
    const Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{2, 3, 4}
    };

    const Vec3 result =
            transform_point(
                    transform,
                    Vec3{1, -2, 3}
                    );

    require_vec3_near(
            result,
            Vec3{2, -6, 12}
            );
}


TEST_CASE("Transform point applies rotation") {
    const Transform transform{
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{1, 1, 1}
    };

    const Vec3 result =
            transform_point(
                    transform,
                    Vec3{1, 0, 0}
                    );

    require_vec3_near(
            result,
            Vec3{0, 0, -1}
            );
}


TEST_CASE("Transform point applies scale rotation translation") {
    const Transform transform{
        Vec3{10, 20, 30},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{2, 3, 4}
    };

    const Vec3 result =
            transform_point(
                    transform,
                    Vec3{1, 2, 3}
                    );

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

    require_vec3_near(
            result,
            Vec3{22, 26, 28}
            );
}


TEST_CASE("Transform point matches matrix transformation") {
    const Transform transform{
        Vec3{7, -2, 5},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(63)
                ),

        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{
        4,
        -2,
        7
    };

    const Vec3 transform_result =
            transform_point(
                    transform,
                    point
                    );

    const Vec4 matrix_result =
            to_matrix(transform)
            * Vec4{
                point.x,
                point.y,
                point.z,
                1.0f
            };

    require_vec3_near(
            transform_result,
            Vec3{
                matrix_result.x,
                matrix_result.y,
                matrix_result.z
            }
            );
}


TEST_CASE("Transform zero point becomes transform position") {
    const Transform transform{
        Vec3{5, -3, 8},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(45)
                ),

        Vec3{2, 3, 4}
    };

    const Vec3 result =
            transform_point(
                    transform,
                    Vec3{0, 0, 0}
                    );

    require_vec3_near(
            result,
            transform.position
            );
}


// ============================================================================
// transform_vector
// ============================================================================

TEST_CASE("Transform vector with identity leaves vector unchanged") {
    constexpr Vec3 vector{
        1,
        2,
        3
    };

    const Vec3 result =
            transform_vector(
                    Transform::identity(),
                    vector
                    );

    require_vec3_near(
            result,
            vector
            );
}


TEST_CASE("Transform vector ignores translation") {
    const Transform transform{
        Vec3{100, -200, 300},
        Quaternion::identity(),
        Vec3{1, 1, 1}
    };

    constexpr Vec3 vector{
        1,
        2,
        3
    };

    const Vec3 result =
            transform_vector(
                    transform,
                    vector
                    );

    require_vec3_near(
            result,
            vector
            );
}


TEST_CASE("Transform vector applies scale") {
    const Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{2, 3, 4}
    };

    const Vec3 result =
            transform_vector(
                    transform,
                    Vec3{1, -2, 3}
                    );

    require_vec3_near(
            result,
            Vec3{2, -6, 12}
            );
}


TEST_CASE("Transform vector applies rotation") {
    const Transform transform{
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{1, 1, 1}
    };

    const Vec3 result =
            transform_vector(
                    transform,
                    Vec3{1, 0, 0}
                    );

    require_vec3_near(
            result,
            Vec3{0, 0, -1}
            );
}


TEST_CASE("Transform vector applies scale then rotation") {
    const Transform transform{
        Vec3{100, 200, 300},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{2, 3, 4}
    };

    const Vec3 result =
            transform_vector(
                    transform,
                    Vec3{1, 2, 3}
                    );

    require_vec3_near(
            result,
            Vec3{12, 6, -2}
            );
}


TEST_CASE("Transform vector matches matrix with zero W") {
    const Transform transform{
        Vec3{50, 60, 70},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(42)
                ),

        Vec3{2, 3, 4}
    };

    constexpr Vec3 vector{
        4,
        -2,
        7
    };

    const Vec3 transform_result =
            transform_vector(
                    transform,
                    vector
                    );

    const Vec4 matrix_result =
            to_matrix(transform)
            * Vec4{
                vector.x,
                vector.y,
                vector.z,
                0.0f
            };

    require_vec3_near(
            transform_result,
            Vec3{
                matrix_result.x,
                matrix_result.y,
                matrix_result.z
            }
            );
}


// ============================================================================
// transform_direction
// ============================================================================

TEST_CASE("Transform direction with identity leaves direction unchanged") {
    constexpr Vec3 direction{
        1,
        2,
        3
    };

    const Vec3 result =
            transform_direction(
                    Transform::identity(),
                    direction
                    );

    require_vec3_near(
            result,
            direction
            );
}


TEST_CASE("Transform direction ignores translation") {
    const Transform transform{
        Vec3{100, 200, 300},
        Quaternion::identity(),
        Vec3{1, 1, 1}
    };

    constexpr Vec3 direction{
        1,
        2,
        3
    };

    const Vec3 result =
            transform_direction(
                    transform,
                    direction
                    );

    require_vec3_near(
            result,
            direction
            );
}


TEST_CASE("Transform direction ignores scale") {
    const Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{2, 5, 10}
    };

    constexpr Vec3 direction{
        1,
        2,
        3
    };

    const Vec3 result =
            transform_direction(
                    transform,
                    direction
                    );

    require_vec3_near(
            result,
            direction
            );
}


TEST_CASE("Transform direction applies rotation") {
    const Transform transform{
        Vec3{50, 60, 70},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{2, 3, 4}
    };

    const Vec3 result =
            transform_direction(
                    transform,
                    Vec3{1, 0, 0}
                    );

    require_vec3_near(
            result,
            Vec3{0, 0, -1}
            );
}


TEST_CASE("Transform direction preserves length") {
    const Transform transform{
        Vec3{50, 60, 70},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(73)
                ),

        Vec3{2, 10, 0.5f}
    };

    constexpr Vec3 direction{
        4,
        -2,
        7
    };

    const Vec3 result =
            transform_direction(
                    transform,
                    direction
                    );

    REQUIRE(
            test_near(
                length(result),
                length(direction))
            );
}


TEST_CASE("Transform zero direction remains zero") {
    const Transform transform{
        Vec3{10, 20, 30},

        from_axis_angle(
                Vec3{1, 1, 1},
                Angle::from_degrees(42)
                ),

        Vec3{2, 3, 4}
    };

    const Vec3 result =
            transform_direction(
                    transform,
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
// forward / right / up
// ============================================================================

TEST_CASE("Transform identity basis directions") {
    const Transform transform =
            Transform::identity();

    require_vec3_near(
            forward(transform),
            Vec3{0, 0, -1}
            );

    require_vec3_near(
            right(transform),
            Vec3{1, 0, 0}
            );

    require_vec3_near(
            up(transform),
            Vec3{0, 1, 0}
            );
}

TEST_CASE("Transform direction ignores negative scale") {
    const Transform transform{
        Vec3{10, 20, 30},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{-2, -3, -4}
    };

    const Vec3 result =
            transform_direction(
                    transform,
                    Vec3{1, 0, 0}
                    );

    require_vec3_near(
            result,
            Vec3{0, 0, -1}
            );
}

TEST_CASE("Transform basis directions ignore zero scale") {
    const Transform transform{
        Vec3{10, 20, 30},
        Quaternion::identity(),
        Vec3{0, 0, 0}
    };

    require_vec3_near(
            forward(transform),
            Vec3{0, 0, -1}
            );

    require_vec3_near(
            right(transform),
            Vec3{1, 0, 0}
            );

    require_vec3_near(
            up(transform),
            Vec3{0, 1, 0}
            );
}

TEST_CASE("Transform basis directions respect rotation") {
    const Transform transform{
        Vec3{100, 200, 300},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{2, 3, 4}
    };

    require_vec3_near(
            forward(transform),
            Vec3{-1, 0, 0}
            );

    require_vec3_near(
            right(transform),
            Vec3{0, 0, -1}
            );

    require_vec3_near(
            up(transform),
            Vec3{0, 1, 0}
            );
}


TEST_CASE("Transform basis directions ignore position and scale") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{1, 2, 3},
                    Angle::from_degrees(57)
                    );

    const Transform a{
        Vec3{0, 0, 0},
        rotation,
        Vec3{1, 1, 1}
    };

    const Transform b{
        Vec3{100, -500, 30},
        rotation,
        Vec3{7, 0.5f, 20}
    };

    require_vec3_near(
            forward(a),
            forward(b)
            );

    require_vec3_near(
            right(a),
            right(b)
            );

    require_vec3_near(
            up(a),
            up(b)
            );
}


TEST_CASE("Transform basis directions are orthonormal") {
    const Transform transform{
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(73)
                ),

        Vec3{5, 10, 20}
    };

    const Vec3 f = forward(transform);
    const Vec3 r = right(transform);
    const Vec3 u = up(transform);

    REQUIRE(test_near(length(f), 1.0f));
    REQUIRE(test_near(length(r), 1.0f));
    REQUIRE(test_near(length(u), 1.0f));

    REQUIRE(test_near(dot(f, r), 0.0f));
    REQUIRE(test_near(dot(f, u), 0.0f));
    REQUIRE(test_near(dot(r, u), 0.0f));
}


// ============================================================================
// nearly_equal
// ============================================================================

TEST_CASE("Transform nearly equal recognizes identical transforms") {
    const Transform transform{
        Vec3{1, 2, 3},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(45)
                ),

        Vec3{2, 3, 4}
    };

    REQUIRE(
            nearly_equal(
                transform,
                transform,
                TEST_EPSILON)
            );
}


TEST_CASE("Transform nearly equal accepts values within epsilon") {
    constexpr Transform lhs{
        Vec3{1, 2, 3},
        Quaternion{0, 0, 0, 1},
        Vec3{2, 3, 4}
    };

    constexpr Transform rhs{
        Vec3{
            1.00005f,
            2.00005f,
            2.99995f
        },

        Quaternion{
            0.00005f,
            0,
            0,
            0.99995f
        },

        Vec3{
            2.00005f,
            2.99995f,
            4.00005f
        }
    };

    REQUIRE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}


TEST_CASE("Transform nearly equal rejects different position") {
    constexpr Transform lhs{
        Vec3{1, 2, 3},
        Quaternion{0, 0, 0, 1},
        Vec3{1, 1, 1}
    };

    constexpr Transform rhs{
        Vec3{1.01f, 2, 3},
        Quaternion{0, 0, 0, 1},
        Vec3{1, 1, 1}
    };

    REQUIRE_FALSE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}


TEST_CASE("Transform nearly equal rejects different rotation") {
    const Transform lhs{
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(45)
                ),

        Vec3{1, 1, 1}
    };

    const Transform rhs{
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(50)
                ),

        Vec3{1, 1, 1}
    };

    REQUIRE_FALSE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}


TEST_CASE("Transform nearly equal rejects different scale") {
    constexpr Transform lhs{
        Vec3{0, 0, 0},
        Quaternion{0, 0, 0, 1},
        Vec3{1, 1, 1}
    };

    constexpr Transform rhs{
        Vec3{0, 0, 0},
        Quaternion{0, 0, 0, 1},
        Vec3{1, 1.01f, 1}
    };

    REQUIRE_FALSE(
            nearly_equal(
                lhs,
                rhs,
                0.0001f)
            );
}


TEST_CASE("Transform nearly equal recognizes negated rotation quaternion") {
    const Quaternion rotation =
            from_axis_angle(
                    Vec3{1, 2, 3},
                    Angle::from_degrees(73)
                    );

    const Quaternion negated_rotation{
        -rotation.x,
        -rotation.y,
        -rotation.z,
        -rotation.w
    };

    const Transform lhs{
        Vec3{1, 2, 3},
        rotation,
        Vec3{2, 3, 4}
    };

    const Transform rhs{
        Vec3{1, 2, 3},
        negated_rotation,
        Vec3{2, 3, 4}
    };

    REQUIRE(
            nearly_equal(
                lhs,
                rhs,
                TEST_EPSILON)
            );
}
