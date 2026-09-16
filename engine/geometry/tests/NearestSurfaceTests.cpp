#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "Math.hpp"
#include "../include/NearestSurface.hpp"

#include <cmath>

using namespace yarep::geometry;
using namespace yarep::math;

namespace {
    constexpr float TEST_EPSILON = 1e-4f;

    void require_vec3_near(
            const Vec3& actual,
            const Vec3& expected
            ) {
        REQUIRE(
                std::abs(actual.x - expected.x)
                <= TEST_EPSILON
                );

        REQUIRE(
                std::abs(actual.y - expected.y)
                <= TEST_EPSILON
                );

        REQUIRE(
                std::abs(actual.z - expected.z)
                <= TEST_EPSILON
                );
    }

    void require_surface_near(
            const NearestSurface& actual,
            const Vec3& expected_normal,
            const float expected_distance
            ) {
        require_vec3_near(
                actual.normal,
                expected_normal
                );

        REQUIRE(
                std::abs(actual.distance - expected_distance)
                <= TEST_EPSILON
                );
    }
}


// ============================================================================
// AABB
// ============================================================================

TEST_CASE (
"Nearest Surface AABB - Finds negative X face"
)
 {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{
        -1.5f,
        0,
        0
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{-1, 0, 0},
        0.5f
    );
}


TEST_CASE (
"Nearest Surface AABB - Finds positive X face"
)
 {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{
        1.25f,
        0,
        0
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{1, 0, 0},
        0.75f
    );
}


TEST_CASE (
"Nearest Surface AABB - Finds negative Y face"
)
 {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{
        0,
        -2.75f,
        0
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{0, -1, 0},
        0.25f
    );
}


TEST_CASE (
"Nearest Surface AABB - Finds positive Y face"
)
 {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{
        0,
        2.5f,
        0
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{0, 1, 0},
        0.5f
    );
}


TEST_CASE (
"Nearest Surface AABB - Finds negative Z face"
)
 {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{
        0,
        0,
        -3.75f
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{0, 0, -1},
        0.25f
    );
}


TEST_CASE (
"Nearest Surface AABB - Finds positive Z face"
)
 {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{
        0,
        0,
        3.25f
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{0, 0, 1},
        0.75f
    );
}


TEST_CASE (
"Nearest Surface AABB - Point on face has zero distance"
)
 {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Vec3 point{
        2,
        1,
        -1
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{1, 0, 0},
        0.0f
    );
}


TEST_CASE (
"Nearest Surface AABB - Supports asymmetric negative world bounds"
)
 {
    constexpr AABB bounds{
        Vec3{-10, -20, -30},
        Vec3{-5, -10, -15}
    };

    constexpr Vec3 point{
        -9.5f,
        -15,
        -20
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{-1, 0, 0},
        0.5f
    );
}


// ============================================================================
// OBB
// ============================================================================

TEST_CASE (
"Nearest Surface OBB - Identity rotation behaves like local AABB"
)
 {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    constexpr Vec3 point{
        1.5f,
        0,
        0
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{1, 0, 0},
        0.5f
    );
}


TEST_CASE (
"Nearest Surface OBB - Translation affects query position"
)
 {
    constexpr OBB bounds{
        Vec3{10, -5, 7},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    constexpr Vec3 point{
        8.25f,
        -5,
        7
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{-1, 0, 0},
        0.25f
    );
}


TEST_CASE (
"Nearest Surface OBB - Ninety degree Y rotation rotates surface normal"
)
 {
    const OBB bounds{
        Vec3{10, 5, -3},
        Vec3{2, 3, 4},

        from_axis_angle(
            Vec3{0, 1, 0},
            Angle::from_degrees(90)
        )
    };

    // Local point is (0, 0, 3.5).
    // Nearest local face is +Z with distance 0.5.
    // +Z rotated 90 degrees around Y becomes world +X.

    constexpr Vec3 point{
        13.5f,
        5,
        -3
    };

    require_surface_near(
        nearest_surface(bounds, point),
        Vec3{1, 0, 0},
        0.5f
    );
}


TEST_CASE (
"Nearest Surface OBB - Arbitrary rotation rotates surface normal"
)
 {
    constexpr float INV_SQRT_2 =
        0.70710678118f;

    const auto rotation =
        from_axis_angle(
            Vec3{0, 0, 1},
            Angle::from_degrees(45)
        );

    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 4, 1},
        rotation
    };

    // Local point lies 0.25 from the +X face.

    constexpr Vec3 local_point{
        1.75f,
        0,
        0
    };

    const Vec3 world_point =
        rotate(
            rotation,
            local_point
        );

    require_surface_near(
        nearest_surface(bounds, world_point),
        Vec3{
            INV_SQRT_2,
            INV_SQRT_2,
            0
        },
        0.25f
    );
}


TEST_CASE (
"Nearest Surface OBB - Rotation and translation work together"
)
 {
    const auto rotation =
        from_axis_angle(
            Vec3{0, 1, 0},
            Angle::from_degrees(90)
        );

    const OBB bounds{
        Vec3{10, -4, 7},
        Vec3{2, 3, 4},
        rotation
    };

    // Local point lies 0.5 from the +Z face.

    constexpr Vec3 local_point{
        0,
        0,
        3.5f
    };

    const Vec3 world_point =
        bounds.center
        + rotate(rotation, local_point);

    require_surface_near(
        nearest_surface(bounds, world_point),
        Vec3{1, 0, 0},
        0.5f
    );
}


TEST_CASE (
"Nearest Surface OBB - Point on rotated face has zero distance"
)
 {
    const auto rotation =
        from_axis_angle(
            Vec3{0, 0, 1},
            Angle::from_degrees(45)
        );

    const OBB bounds{
        Vec3{4, -2, 7},
        Vec3{2, 3, 4},
        rotation
    };

    constexpr Vec3 local_point{
        2,
        0,
        0
    };

    const Vec3 world_point =
        bounds.center
        + rotate(rotation, local_point);

    const Vec3 expected_normal =
        rotate(
            rotation,
            Vec3{1, 0, 0}
        );

    require_surface_near(
        nearest_surface(bounds, world_point),
        expected_normal,
        0.0f
    );
}
