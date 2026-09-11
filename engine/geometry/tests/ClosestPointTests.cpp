#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/ClosestPoint.hpp"
#include <QuaternionMath.hpp>

#include <cmath>

using namespace yarep::geometry;
using namespace yarep::math;

namespace {
    constexpr float TEST_EPSILON = 1e-4f;

    void require_vec3_near(
            const Vec3& actual,
            const Vec3& expected
            ) {
        REQUIRE(std::abs(actual.x - expected.x) <= TEST_EPSILON);
        REQUIRE(std::abs(actual.y - expected.y) <= TEST_EPSILON);
        REQUIRE(std::abs(actual.z - expected.z) <= TEST_EPSILON);
    }
}


// ============================================================================
// AABB
// ============================================================================

TEST_CASE("Closest Point AABB - Point inside bounds returns itself") {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{1, -1, 2};

    require_vec3_near(
            closest_point(bounds, point),
            point
            );
}


TEST_CASE("Closest Point AABB - Point on boundary returns itself") {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{2, 1, -2};

    require_vec3_near(
            closest_point(bounds, point),
            point
            );
}


TEST_CASE("Closest Point AABB - Point outside one axis clamps to face") {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{5, 1, -2};

    require_vec3_near(
            closest_point(bounds, point),
            Vec3{2, 1, -2}
            );
}


TEST_CASE("Closest Point AABB - Point outside multiple axes clamps to edge") {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{5, 7, 2};

    require_vec3_near(
            closest_point(bounds, point),
            Vec3{2, 3, 2}
            );
}


TEST_CASE("Closest Point AABB - Point outside all axes clamps to corner") {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{-10, 8, 12};

    require_vec3_near(
            closest_point(bounds, point),
            Vec3{-2, 3, 4}
            );
}


TEST_CASE("Closest Point AABB - Supports negative world coordinates") {
    constexpr AABB bounds{
        Vec3{-10, -20, -30},
        Vec3{-5, -10, -15}
    };

    constexpr Vec3 point{-20, -15, -40};

    require_vec3_near(
            closest_point(bounds, point),
            Vec3{-10, -15, -30}
            );
}


TEST_CASE("Closest Point AABB - Zero sized bounds return their point") {
    constexpr AABB bounds{
        Vec3{2, -4, 7},
        Vec3{2, -4, 7}
    };

    require_vec3_near(
            closest_point(bounds, Vec3{100, 50, -30}),
            Vec3{2, -4, 7}
            );
}


// ============================================================================
// Sphere
// ============================================================================

TEST_CASE("Closest Point Sphere - Point inside sphere returns itself") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        5.0f
    };

    constexpr Vec3 point{1, 2, 1};

    require_vec3_near(
            closest_point(sphere, point),
            point
            );
}


TEST_CASE("Closest Point Sphere - Sphere center returns itself") {
    constexpr Sphere sphere{
        Vec3{1, 2, 3},
        5.0f
    };

    require_vec3_near(
            closest_point(sphere, sphere.center),
            sphere.center
            );
}


TEST_CASE("Closest Point Sphere - Point on surface returns itself") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        5.0f
    };

    constexpr Vec3 point{3, 4, 0};

    require_vec3_near(
            closest_point(sphere, point),
            point
            );
}


TEST_CASE("Closest Point Sphere - Outside point projects onto sphere surface") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        2.0f
    };

    constexpr Vec3 point{3, 4, 0};

    require_vec3_near(
            closest_point(sphere, point),
            Vec3{1.2f, 1.6f, 0}
            );
}


TEST_CASE("Closest Point Sphere - Offset sphere projects relative to center") {
    constexpr Sphere sphere{
        Vec3{10, -5, 3},
        2.0f
    };

    constexpr Vec3 point{15, -5, 3};

    require_vec3_near(
            closest_point(sphere, point),
            Vec3{12, -5, 3}
            );
}


TEST_CASE("Closest Point Sphere - Zero radius sphere always returns center") {
    constexpr Sphere sphere{
        Vec3{2, -4, 7},
        0.0f
    };

    require_vec3_near(
            closest_point(sphere, Vec3{100, 200, 300}),
            sphere.center
            );
}

TEST_CASE("Closest Point Sphere - Outside point in negative direction projects onto surface") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        2.0f
    };

    constexpr Vec3 point{-5, 0, 0};

    require_vec3_near(
            closest_point(sphere, point),
            Vec3{-2, 0, 0}
            );
}

// ============================================================================
// OBB
// ============================================================================

TEST_CASE("Closest Point OBB - Point inside identity OBB returns itself") {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    constexpr Vec3 point{1, -2, 3};

    require_vec3_near(
            closest_point(bounds, point),
            point
            );
}


TEST_CASE("Closest Point OBB - Point outside identity OBB clamps to face") {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    constexpr Vec3 point{5, 1, -2};

    require_vec3_near(
            closest_point(bounds, point),
            Vec3{2, 1, -2}
            );
}


TEST_CASE("Closest Point OBB - Translation affects closest point") {
    constexpr OBB bounds{
        Vec3{10, -5, 7},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    constexpr Vec3 point{15, -4, 8};

    require_vec3_near(
            closest_point(bounds, point),
            Vec3{12, -4, 8}
            );
}


TEST_CASE("Closest Point OBB - Rotation affects closest point") {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 1, 4},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                )
    };

    require_vec3_near(
            closest_point(bounds, Vec3{6, 0, 0}),
            Vec3{4, 0, 0}
            );
}


TEST_CASE("Closest Point OBB - Rotated OBB uses actual oriented bounds") {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 0.5f, 0.5f},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    constexpr float SQRT_2 = 1.41421356237f;

    require_vec3_near(
            closest_point(bounds, Vec3{1.7f, 1.7f, 0}),
            Vec3{SQRT_2, SQRT_2, 0}
            );
}


TEST_CASE("Closest Point OBB - Point on rotated boundary returns itself") {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 1, 4},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                )
    };

    constexpr Vec3 point{4, 0, 0};

    require_vec3_near(
            closest_point(bounds, point),
            point
            );
}


TEST_CASE("Closest Point OBB - Zero sized rotated OBB returns center") {
    const OBB bounds{
        Vec3{4, -2, 7},
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(57)
                )
    };

    require_vec3_near(
            closest_point(bounds, Vec3{100, 200, 300}),
            bounds.center
            );
}

TEST_CASE("Closest Point OBB - Translation and rotation are applied around OBB center") {
    const OBB bounds{
        Vec3{10, 0, 0},
        Vec3{2, 1, 4},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                )
    };

    /*
     * Local OBB X/Z extents:
     * x = +/-2
     * z = +/-4
     *
     * After 90 degree Y rotation:
     * local Z becomes world X.
     *
     * The point lies 6 units along world +X from the OBB center,
     * so the closest point is 4 units along world +X from the center.
     */

    require_vec3_near(
            closest_point(bounds, Vec3{16, 0, 0}),
            Vec3{14, 0, 0}
            );
}

// ============================================================================
// Plane
// ============================================================================

TEST_CASE("Closest Point Plane - Point above plane projects orthogonally") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    require_vec3_near(
            closest_point(plane, Vec3{2, 5, -3}),
            Vec3{2, 0, -3}
            );
}


TEST_CASE("Closest Point Plane - Point below plane projects orthogonally") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    require_vec3_near(
            closest_point(plane, Vec3{2, -5, -3}),
            Vec3{2, 0, -3}
            );
}


TEST_CASE("Closest Point Plane - Point on plane returns itself") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Vec3 point{7, 0, -4};

    require_vec3_near(
            closest_point(plane, point),
            point
            );
}


TEST_CASE("Closest Point Plane - Offset plane projects correctly") {
    // y = 5

    constexpr Plane plane{
        Vec3{0, 1, 0},
        -5.0f
    };

    require_vec3_near(
            closest_point(plane, Vec3{2, 9, -3}),
            Vec3{2, 5, -3}
            );
}


TEST_CASE("Closest Point Plane - Diagonal plane projects orthogonally") {
    constexpr float INV_SQRT_2 = 0.70710678118f;

    constexpr Plane plane{
        Vec3{INV_SQRT_2, INV_SQRT_2, 0},
        0.0f
    };

    require_vec3_near(
            closest_point(plane, Vec3{2, 0, 3}),
            Vec3{1, -1, 3}
            );
}
