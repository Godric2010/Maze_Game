#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
#else
    #include <catch2/catch_all.hpp>
#endif

#include <QuaternionMath.hpp>
#include "../include/RayIntersection.hpp"

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

    template<typename OptionalHit>
    void require_hit(
            const OptionalHit& result,
            const float expected_distance,
            const Vec3& expected_point,
            const Vec3& expected_normal
            ) {
        REQUIRE(result.has_value());

        REQUIRE(
                std::abs(result->distance - expected_distance)
                <= TEST_EPSILON
                );

        require_vec3_near(
                result->point,
                expected_point
                );

        require_vec3_near(
                result->normal,
                expected_normal
                );
    }
}


// ============================================================================
// Ray <-> Sphere
// ============================================================================

TEST_CASE("Ray Sphere - Ray hits sphere") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        1.0f
    };

    constexpr Ray ray{
        Vec3{-5, 0, 0},
        Vec3{1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, sphere),
            4.0f,
            Vec3{-1, 0, 0},
            Vec3{-1, 0, 0}
            );
}


TEST_CASE("Ray Sphere - Ray miss returns no hit") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        1.0f
    };

    constexpr Ray ray{
        Vec3{-5, 2, 0},
        Vec3{1, 0, 0}
    };

    REQUIRE_FALSE(
            ray_intersection(ray, sphere).has_value()
            );
}


TEST_CASE("Ray Sphere - Ray pointing away returns no hit") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        1.0f
    };

    constexpr Ray ray{
        Vec3{-5, 0, 0},
        Vec3{-1, 0, 0}
    };

    REQUIRE_FALSE(
            ray_intersection(ray, sphere).has_value()
            );
}


TEST_CASE("Ray Sphere - Tangent ray hits sphere") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        1.0f
    };

    constexpr Ray ray{
        Vec3{-5, 1, 0},
        Vec3{1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, sphere),
            5.0f,
            Vec3{0, 1, 0},
            Vec3{0, 1, 0}
            );
}


TEST_CASE("Ray Sphere - Ray starting inside hits exit surface") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        2.0f
    };

    constexpr Ray ray{
        Vec3{0, 0, 0},
        Vec3{1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, sphere),
            2.0f,
            Vec3{2, 0, 0},
            Vec3{1, 0, 0}
            );
}


TEST_CASE("Ray Sphere - Ray starting on surface hits at zero distance") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        2.0f
    };

    constexpr Ray ray{
        Vec3{-2, 0, 0},
        Vec3{-1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, sphere),
            0.0f,
            Vec3{-2, 0, 0},
            Vec3{-1, 0, 0}
            );
}


TEST_CASE("Ray Sphere - Non normalized direction reports world distance") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        1.0f
    };

    constexpr Ray ray{
        Vec3{-5, 0, 0},
        Vec3{2, 0, 0}
    };

    // Parametric t is 2, but actual world-space distance is 4.

    require_hit(
            ray_intersection(ray, sphere),
            4.0f,
            Vec3{-1, 0, 0},
            Vec3{-1, 0, 0}
            );
}


// ============================================================================
// Ray <-> Plane
// ============================================================================

TEST_CASE("Ray Plane - Ray hits plane") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Ray ray{
        Vec3{2, 5, -3},
        Vec3{0, -1, 0}
    };

    require_hit(
            ray_intersection(ray, plane),
            5.0f,
            Vec3{2, 0, -3},
            Vec3{0, 1, 0}
            );
}


TEST_CASE("Ray Plane - Offset plane is hit") {
    // y = 5

    constexpr Plane plane{
        Vec3{0, 1, 0},
        -5.0f
    };

    constexpr Ray ray{
        Vec3{2, 10, -3},
        Vec3{0, -1, 0}
    };

    require_hit(
            ray_intersection(ray, plane),
            5.0f,
            Vec3{2, 5, -3},
            Vec3{0, 1, 0}
            );
}


TEST_CASE("Ray Plane - Ray pointing away returns no hit") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Ray ray{
        Vec3{0, 5, 0},
        Vec3{0, 1, 0}
    };

    REQUIRE_FALSE(
            ray_intersection(ray, plane).has_value()
            );
}


TEST_CASE("Ray Plane - Parallel ray returns no hit") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Ray ray{
        Vec3{0, 5, 0},
        Vec3{1, 0, 0}
    };

    REQUIRE_FALSE(
            ray_intersection(ray, plane).has_value()
            );
}


TEST_CASE("Ray Plane - Coplanar ray has no unique intersection") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Ray ray{
        Vec3{2, 0, -3},
        Vec3{1, 0, 0}
    };

    REQUIRE_FALSE(
            ray_intersection(ray, plane).has_value()
            );
}


TEST_CASE("Ray Plane - Ray starting on plane hits at zero distance") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Ray ray{
        Vec3{2, 0, -3},
        Vec3{0, 1, 0}
    };

    require_hit(
            ray_intersection(ray, plane),
            0.0f,
            Vec3{2, 0, -3},
            Vec3{0, 1, 0}
            );
}


TEST_CASE("Ray Plane - Non normalized direction reports world distance") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Ray ray{
        Vec3{0, 6, 0},
        Vec3{0, -2, 0}
    };

    // Parametric t is 3, world-space distance is 6.

    require_hit(
            ray_intersection(ray, plane),
            6.0f,
            Vec3{0, 0, 0},
            Vec3{0, 1, 0}
            );
}


// ============================================================================
// Ray <-> AABB
// ============================================================================

TEST_CASE("Ray AABB - Ray hits face") {
    constexpr AABB bounds{
        Vec3{-1, -2, -3},
        Vec3{1, 2, 3}
    };

    constexpr Ray ray{
        Vec3{-5, 0, 0},
        Vec3{1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, bounds),
            4.0f,
            Vec3{-1, 0, 0},
            Vec3{-1, 0, 0}
            );
}


TEST_CASE("Ray AABB - Ray misses bounds") {
    constexpr AABB bounds{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr Ray ray{
        Vec3{-5, 2, 0},
        Vec3{1, 0, 0}
    };

    REQUIRE_FALSE(
            ray_intersection(ray, bounds).has_value()
            );
}


TEST_CASE("Ray AABB - Parallel ray outside slab misses") {
    constexpr AABB bounds{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr Ray ray{
        Vec3{2, 0, -5},
        Vec3{0, 0, 1}
    };

    REQUIRE_FALSE(
            ray_intersection(ray, bounds).has_value()
            );
}


TEST_CASE("Ray AABB - Ray starting inside hits exit face") {
    constexpr AABB bounds{
        Vec3{-1, -2, -3},
        Vec3{1, 2, 3}
    };

    constexpr Ray ray{
        Vec3{0, 0, 0},
        Vec3{1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, bounds),
            1.0f,
            Vec3{1, 0, 0},
            Vec3{1, 0, 0}
            );
}


TEST_CASE("Ray AABB - Ray starting on surface hits at zero distance") {
    constexpr AABB bounds{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr Ray ray{
        Vec3{-1, 0, 0},
        Vec3{-1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, bounds),
            0.0f,
            Vec3{-1, 0, 0},
            Vec3{-1, 0, 0}
            );
}


TEST_CASE("Ray AABB - Ray from positive side returns positive face normal") {
    constexpr AABB bounds{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr Ray ray{
        Vec3{5, 0, 0},
        Vec3{-1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, bounds),
            4.0f,
            Vec3{1, 0, 0},
            Vec3{1, 0, 0}
            );
}


TEST_CASE("Ray AABB - Non normalized direction reports world distance") {
    constexpr AABB bounds{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr Ray ray{
        Vec3{-5, 0, 0},
        Vec3{2, 0, 0}
    };

    require_hit(
            ray_intersection(ray, bounds),
            4.0f,
            Vec3{-1, 0, 0},
            Vec3{-1, 0, 0}
            );
}

TEST_CASE("Ray AABB - Ray starting on surface and moving tangentially hits at zero distance") {
    constexpr AABB bounds{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr Ray ray{
        Vec3{-1, 0, 0},
        Vec3{0, 1, 0}
    };

    require_hit(
            ray_intersection(ray, bounds),
            0.0f,
            Vec3{-1, 0, 0},
            Vec3{-1, 0, 0}
            );
}

// ============================================================================
// Ray <-> OBB
// ============================================================================

TEST_CASE("Ray OBB - Ray hits identity OBB") {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{1, 2, 3},
        Quaternion::identity()
    };

    constexpr Ray ray{
        Vec3{-5, 0, 0},
        Vec3{1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, bounds),
            4.0f,
            Vec3{-1, 0, 0},
            Vec3{-1, 0, 0}
            );
}


TEST_CASE("Ray OBB - Translation and rotation affect hit") {
    const OBB bounds{
        Vec3{10, 0, 0},
        Vec3{2, 1, 4},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                )
    };

    constexpr Ray ray{
        Vec3{20, 0, 0},
        Vec3{-1, 0, 0}
    };

    // Local +Z becomes world +X.
    // World-space +X face therefore lies at x = 14.

    require_hit(
            ray_intersection(ray, bounds),
            6.0f,
            Vec3{14, 0, 0},
            Vec3{1, 0, 0}
            );
}


TEST_CASE("Ray OBB - Ray aligned with rotated local axis hits correctly") {
    constexpr float INV_SQRT_2 = 0.70710678118f;
    constexpr float SQRT_2 = 1.41421356237f;

    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 1, 1},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    const Ray ray{
        Vec3{
            -5.0f * INV_SQRT_2,
            -5.0f * INV_SQRT_2,
            0
        },
        Vec3{
            INV_SQRT_2,
            INV_SQRT_2,
            0
        }
    };

    require_hit(
            ray_intersection(ray, bounds),
            3.0f,
            Vec3{-SQRT_2, -SQRT_2, 0},
            Vec3{-INV_SQRT_2, -INV_SQRT_2, 0}
            );
}


TEST_CASE("Ray OBB - Ray inside enclosing AABB can still miss rotated OBB") {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 0.5f, 0.5f},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    constexpr Ray ray{
        Vec3{1.6f, 1.6f, -5},
        Vec3{0, 0, 1}
    };

    REQUIRE_FALSE(
            ray_intersection(ray, bounds).has_value()
            );
}


TEST_CASE("Ray OBB - Ray starting inside hits exit surface") {
    const OBB bounds{
        Vec3{10, 0, 0},
        Vec3{2, 1, 4},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                )
    };

    constexpr Ray ray{
        Vec3{10, 0, 0},
        Vec3{1, 0, 0}
    };

    require_hit(
            ray_intersection(ray, bounds),
            4.0f,
            Vec3{14, 0, 0},
            Vec3{1, 0, 0}
            );
}


TEST_CASE("Ray OBB - Non normalized direction reports world distance") {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{1, 2, 3},
        Quaternion::identity()
    };

    constexpr Ray ray{
        Vec3{-5, 0, 0},
        Vec3{2, 0, 0}
    };

    require_hit(
            ray_intersection(ray, bounds),
            4.0f,
            Vec3{-1, 0, 0},
            Vec3{-1, 0, 0}
            );
}
