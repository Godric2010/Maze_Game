#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
    #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Contains.hpp"
#include <QuaternionMath.hpp>

using namespace yarep::geometry;
using namespace yarep::math;


// ============================================================================
// AABB
// ============================================================================

TEST_CASE(
        "Contains AABB - Point inside bounds"
        ) {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE(
            contains(
                bounds,
                Vec3{1, -1, 2})
            );
}


TEST_CASE(
        "Contains AABB - Center is contained"
        ) {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE(
            contains(
                bounds,
                Vec3{0, 0, 0})
            );
}


TEST_CASE(
        "Contains AABB - Minimum corner is contained"
        ) {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE(
            contains(
                bounds,
                bounds.min)
            );
}


TEST_CASE(
        "Contains AABB - Maximum corner is contained"
        ) {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE(
            contains(
                bounds,
                bounds.max)
            );
}


TEST_CASE(
        "Contains AABB - Point on face is contained"
        ) {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE(
            contains(
                bounds,
                Vec3{2, 0, 0})
            );
}


TEST_CASE(
        "Contains AABB - Point outside positive X is not contained"
        ) {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{2.1f, 0, 0})
            );
}


TEST_CASE(
        "Contains AABB - Point outside negative X is not contained"
        ) {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{-2.1f, 0, 0})
            );
}


TEST_CASE(
        "Contains AABB - Point outside Y is not contained"
        ) {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{0, 3.1f, 0})
            );
}


TEST_CASE(
        "Contains AABB - Point outside Z is not contained"
        ) {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{0, 0, -4.1f})
            );
}


TEST_CASE(
        "Contains AABB - Supports negative world coordinates"
        ) {
    constexpr AABB bounds{
        Vec3{-10, -20, -30},
        Vec3{-5, -10, -15}
    };

    REQUIRE(
            contains(
                bounds,
                Vec3{-7, -15, -20})
            );

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{-4, -15, -20})
            );
}


TEST_CASE(
        "Contains AABB - Zero sized bounds contain their point"
        ) {
    constexpr AABB bounds{
        Vec3{2, -4, 7},
        Vec3{2, -4, 7}
    };

    REQUIRE(
            contains(
                bounds,
                Vec3{2, -4, 7})
            );
}


TEST_CASE(
        "Contains AABB - Zero sized bounds reject other points"
        ) {
    constexpr AABB bounds{
        Vec3{2, -4, 7},
        Vec3{2, -4, 7}
    };

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{2.01f, -4, 7})
            );
}


// ============================================================================
// Sphere
// ============================================================================

TEST_CASE(
        "Contains Sphere - Center is contained"
        ) {
    constexpr Sphere sphere{
        Vec3{1, 2, 3},
        5.0f
    };

    REQUIRE(
            contains(
                sphere,
                sphere.center)
            );
}


TEST_CASE(
        "Contains Sphere - Point inside sphere is contained"
        ) {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        5.0f
    };

    REQUIRE(
            contains(
                sphere,
                Vec3{1, 2, 3})
            );
}


TEST_CASE(
        "Contains Sphere - Point on surface is contained"
        ) {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        5.0f
    };

    REQUIRE(
            contains(
                sphere,
                Vec3{3, 4, 0})
            );
}


TEST_CASE(
        "Contains Sphere - Point outside sphere is not contained"
        ) {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        5.0f
    };

    REQUIRE_FALSE(
            contains(
                sphere,
                Vec3{3, 4.1f, 0})
            );
}


TEST_CASE(
        "Contains Sphere - Offset sphere contains relative point"
        ) {
    constexpr Sphere sphere{
        Vec3{10, -5, 3},
        2.0f
    };

    REQUIRE(
            contains(
                sphere,
                Vec3{11, -5, 3})
            );

    REQUIRE_FALSE(
            contains(
                sphere,
                Vec3{13, -5, 3})
            );
}


TEST_CASE(
        "Contains Sphere - Supports negative world coordinates"
        ) {
    constexpr Sphere sphere{
        Vec3{-10, -20, -30},
        4.0f
    };

    REQUIRE(
            contains(
                sphere,
                Vec3{-12, -20, -30})
            );

    REQUIRE_FALSE(
            contains(
                sphere,
                Vec3{-15, -20, -30})
            );
}


TEST_CASE(
        "Contains Sphere - Zero radius sphere contains its center"
        ) {
    constexpr Sphere sphere{
        Vec3{2, -4, 7},
        0.0f
    };

    REQUIRE(
            contains(
                sphere,
                Vec3{2, -4, 7})
            );
}


TEST_CASE(
        "Contains Sphere - Zero radius sphere rejects other points"
        ) {
    constexpr Sphere sphere{
        Vec3{2, -4, 7},
        0.0f
    };

    REQUIRE_FALSE(
            contains(
                sphere,
                Vec3{2.01f, -4, 7})
            );
}


// ============================================================================
// OBB
// ============================================================================

TEST_CASE(
        "Contains OBB - Center is contained"
        ) {
    constexpr OBB bounds{
        Vec3{1, 2, 3},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    REQUIRE(
            contains(
                bounds,
                bounds.center)
            );
}


TEST_CASE(
        "Contains OBB - Point inside identity OBB is contained"
        ) {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    REQUIRE(
            contains(
                bounds,
                Vec3{1, -2, 3})
            );
}


TEST_CASE(
        "Contains OBB - Point on identity OBB boundary is contained"
        ) {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    REQUIRE(
            contains(
                bounds,
                Vec3{2, 3, 4})
            );
}


TEST_CASE(
        "Contains OBB - Point outside identity OBB is not contained"
        ) {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{2.1f, 0, 0})
            );
}


TEST_CASE(
        "Contains OBB - Translation affects containment"
        ) {
    constexpr OBB bounds{
        Vec3{10, -5, 7},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    REQUIRE(
            contains(
                bounds,
                Vec3{11, -4, 8})
            );

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{0, 0, 0})
            );
}


TEST_CASE(
        "Contains OBB - Ninety degree Y rotation affects containment"
        ) {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 1, 4},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                )
    };

    // Local Z extent is 4 and rotates onto world X.

    REQUIRE(
            contains(
                bounds,
                Vec3{3.5f, 0, 0})
            );

    // Local X extent is 2 and rotates onto world Z.

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{0, 0, 2.5f})
            );
}


TEST_CASE(
        "Contains OBB - Rotated OBB rejects point inside enclosing AABB but outside OBB"
        ) {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 1, 1},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    // This point lies inside the enclosing AABB of the rotated box,
    // but outside the actual OBB.

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{2.0f, 2.0f, 0})
            );
}


TEST_CASE(
        "Contains OBB - Rotation around translated center"
        ) {
    const OBB bounds{
        Vec3{10, 5, -3},
        Vec3{2, 1, 4},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                )
    };

    REQUIRE(
            contains(
                bounds,
                Vec3{13, 5, -3})
            );

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{10, 5, 0})
            );
}


TEST_CASE(
        "Contains OBB - Zero sized OBB contains its center"
        ) {
    const OBB bounds{
        Vec3{4, -2, 7},
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(57)
                )
    };

    REQUIRE(
            contains(
                bounds,
                bounds.center)
            );
}


TEST_CASE(
        "Contains OBB - Zero sized OBB rejects other points"
        ) {
    const OBB bounds{
        Vec3{4, -2, 7},
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(57)
                )
    };

    REQUIRE_FALSE(
            contains(
                bounds,
                Vec3{4.1f, -2, 7})
            );
}
