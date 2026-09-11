#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
    #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Intersects.hpp"
#include <QuaternionMath.hpp>

using namespace yarep::geometry;
using namespace yarep::math;


// ============================================================================
// AABB <-> AABB
// ============================================================================

TEST_CASE("Intersects AABB AABB - Overlapping bounds intersect") {
    constexpr AABB a{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    constexpr AABB b{
        Vec3{1, 1, 1},
        Vec3{4, 4, 4}
    };

    REQUIRE(intersects(a, b));
}


TEST_CASE("Intersects AABB AABB - Identical bounds intersect") {
    constexpr AABB bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    REQUIRE(intersects(bounds, bounds));
}


TEST_CASE("Intersects AABB AABB - Contained bounds intersect") {
    constexpr AABB outer{
        Vec3{-5, -5, -5},
        Vec3{5, 5, 5}
    };

    constexpr AABB inner{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    REQUIRE(intersects(outer, inner));
    REQUIRE(intersects(inner, outer));
}


TEST_CASE("Intersects AABB AABB - Separation on X does not intersect") {
    constexpr AABB a{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr AABB b{
        Vec3{2, -1, -1},
        Vec3{4, 1, 1}
    };

    REQUIRE_FALSE(intersects(a, b));
}


TEST_CASE("Intersects AABB AABB - Separation on Y does not intersect") {
    constexpr AABB a{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr AABB b{
        Vec3{-1, 2, -1},
        Vec3{1, 4, 1}
    };

    REQUIRE_FALSE(intersects(a, b));
}


TEST_CASE("Intersects AABB AABB - Separation on Z does not intersect") {
    constexpr AABB a{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr AABB b{
        Vec3{-1, -1, 2},
        Vec3{1, 1, 4}
    };

    REQUIRE_FALSE(intersects(a, b));
}


TEST_CASE("Intersects AABB AABB - Touching faces intersect") {
    constexpr AABB a{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr AABB b{
        Vec3{1, -1, -1},
        Vec3{3, 1, 1}
    };

    REQUIRE(intersects(a, b));
}


TEST_CASE("Intersects AABB AABB - Touching edges intersect") {
    constexpr AABB a{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr AABB b{
        Vec3{1, 1, -1},
        Vec3{3, 3, 1}
    };

    REQUIRE(intersects(a, b));
}


TEST_CASE("Intersects AABB AABB - Touching corners intersect") {
    constexpr AABB a{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr AABB b{
        Vec3{1, 1, 1},
        Vec3{3, 3, 3}
    };

    REQUIRE(intersects(a, b));
}


TEST_CASE("Intersects AABB AABB - Point AABB inside bounds intersects") {
    constexpr AABB bounds{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    constexpr AABB point{
        Vec3{1, 1, 1},
        Vec3{1, 1, 1}
    };

    REQUIRE(intersects(bounds, point));
}


TEST_CASE("Intersects AABB AABB - Separated point AABB does not intersect") {
    constexpr AABB bounds{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    constexpr AABB point{
        Vec3{3, 0, 0},
        Vec3{3, 0, 0}
    };

    REQUIRE_FALSE(intersects(bounds, point));
}


// ============================================================================
// Sphere <-> Sphere
// ============================================================================

TEST_CASE("Intersects Sphere Sphere - Overlapping spheres intersect") {
    constexpr Sphere a{
        Vec3{0, 0, 0},
        2.0f
    };

    constexpr Sphere b{
        Vec3{3, 0, 0},
        2.0f
    };

    REQUIRE(intersects(a, b));
}


TEST_CASE("Intersects Sphere Sphere - Touching spheres intersect") {
    constexpr Sphere a{
        Vec3{0, 0, 0},
        2.0f
    };

    constexpr Sphere b{
        Vec3{4, 0, 0},
        2.0f
    };

    REQUIRE(intersects(a, b));
}


TEST_CASE("Intersects Sphere Sphere - Separated spheres do not intersect") {
    constexpr Sphere a{
        Vec3{0, 0, 0},
        2.0f
    };

    constexpr Sphere b{
        Vec3{4.1f, 0, 0},
        2.0f
    };

    REQUIRE_FALSE(intersects(a, b));
}


TEST_CASE("Intersects Sphere Sphere - Contained sphere intersects") {
    constexpr Sphere outer{
        Vec3{0, 0, 0},
        10.0f
    };

    constexpr Sphere inner{
        Vec3{2, 0, 0},
        1.0f
    };

    REQUIRE(intersects(outer, inner));
    REQUIRE(intersects(inner, outer));
}


TEST_CASE("Intersects Sphere Sphere - Identical spheres intersect") {
    constexpr Sphere sphere{
        Vec3{2, -4, 7},
        3.0f
    };

    REQUIRE(intersects(sphere, sphere));
}


TEST_CASE("Intersects Sphere Sphere - Two zero radius spheres at same point intersect") {
    constexpr Sphere a{
        Vec3{1, 2, 3},
        0.0f
    };

    constexpr Sphere b{
        Vec3{1, 2, 3},
        0.0f
    };

    REQUIRE(intersects(a, b));
}


TEST_CASE("Intersects Sphere Sphere - Separated zero radius spheres do not intersect") {
    constexpr Sphere a{
        Vec3{1, 2, 3},
        0.0f
    };

    constexpr Sphere b{
        Vec3{1.1f, 2, 3},
        0.0f
    };

    REQUIRE_FALSE(intersects(a, b));
}


// ============================================================================
// Sphere <-> AABB
// ============================================================================

TEST_CASE("Intersects Sphere AABB - Sphere center inside AABB intersects") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        1.0f
    };

    constexpr AABB bounds{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere AABB - Sphere overlapping face intersects") {
    constexpr Sphere sphere{
        Vec3{2.5f, 0, 0},
        1.0f
    };

    constexpr AABB bounds{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere AABB - Sphere touching face intersects") {
    constexpr Sphere sphere{
        Vec3{3, 0, 0},
        1.0f
    };

    constexpr AABB bounds{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere AABB - Sphere touching corner intersects") {
    constexpr AABB bounds{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr Sphere sphere{
        Vec3{2, 1, 1},
        1.0f
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere AABB - Separated sphere does not intersect") {
    constexpr Sphere sphere{
        Vec3{3.1f, 0, 0},
        1.0f
    };

    constexpr AABB bounds{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    REQUIRE_FALSE(intersects(sphere, bounds));
    REQUIRE_FALSE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere AABB - AABB completely inside sphere intersects") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        10.0f
    };

    constexpr AABB bounds{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere AABB - Zero radius sphere inside bounds intersects") {
    constexpr Sphere sphere{
        Vec3{1, 0, 0},
        0.0f
    };

    constexpr AABB bounds{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere AABB - Zero radius sphere outside bounds does not intersect") {
    constexpr Sphere sphere{
        Vec3{3, 0, 0},
        0.0f
    };

    constexpr AABB bounds{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    REQUIRE_FALSE(intersects(sphere, bounds));
    REQUIRE_FALSE(intersects(bounds, sphere));
}


// ============================================================================
// Sphere <-> OBB
// ============================================================================

TEST_CASE("Intersects Sphere OBB - Sphere inside identity OBB intersects") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        1.0f
    };

    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere OBB - Sphere touching identity OBB intersects") {
    constexpr Sphere sphere{
        Vec3{3, 0, 0},
        1.0f
    };

    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere OBB - Separated sphere does not intersect") {
    constexpr Sphere sphere{
        Vec3{3.1f, 0, 0},
        1.0f
    };

    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    REQUIRE_FALSE(intersects(sphere, bounds));
    REQUIRE_FALSE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere OBB - Rotation affects intersection") {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 1, 4},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                )
    };

    const Sphere sphere{
        Vec3{4.5f, 0, 0},
        0.5f
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere OBB - Sphere inside enclosing AABB but outside rotated OBB does not intersect") {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 0.5f, 0.5f},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    const Sphere sphere{
        Vec3{1.7f, 1.7f, 0},
        0.2f
    };

    REQUIRE_FALSE(intersects(sphere, bounds));
    REQUIRE_FALSE(intersects(bounds, sphere));
}


TEST_CASE("Intersects Sphere OBB - Zero radius sphere inside rotated OBB intersects") {
    const OBB bounds{
        Vec3{4, -2, 7},
        Vec3{2, 3, 4},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(57)
                )
    };

    constexpr Sphere sphere{
        Vec3{4, -2, 7},
        0.0f
    };

    REQUIRE(intersects(sphere, bounds));
    REQUIRE(intersects(bounds, sphere));
}


// ============================================================================
// AABB <-> OBB
// ============================================================================

TEST_CASE("Intersects AABB OBB - Identity bounds overlap") {
    constexpr AABB aabb{
        Vec3{-2, -2, -2},
        Vec3{2, 2, 2}
    };

    constexpr OBB obb{
        Vec3{1, 0, 0},
        Vec3{2, 2, 2},
        Quaternion::identity()
    };

    REQUIRE(intersects(aabb, obb));
    REQUIRE(intersects(obb, aabb));
}


TEST_CASE("Intersects AABB OBB - Identity bounds separated") {
    constexpr AABB aabb{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr OBB obb{
        Vec3{4, 0, 0},
        Vec3{1, 1, 1},
        Quaternion::identity()
    };

    REQUIRE_FALSE(intersects(aabb, obb));
    REQUIRE_FALSE(intersects(obb, aabb));
}


TEST_CASE("Intersects AABB OBB - Touching faces intersect") {
    constexpr AABB aabb{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    constexpr OBB obb{
        Vec3{2, 0, 0},
        Vec3{1, 1, 1},
        Quaternion::identity()
    };

    REQUIRE(intersects(aabb, obb));
    REQUIRE(intersects(obb, aabb));
}


TEST_CASE("Intersects AABB OBB - Rotated OBB overlaps AABB") {
    constexpr AABB aabb{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    const OBB obb{
        Vec3{1.5f, 0, 0},
        Vec3{1, 1, 1},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    REQUIRE(intersects(aabb, obb));
    REQUIRE(intersects(obb, aabb));
}


TEST_CASE("Intersects AABB OBB - Rotated OBB can be separated") {
    constexpr AABB aabb{
        Vec3{-1, -1, -1},
        Vec3{1, 1, 1}
    };

    const OBB obb{
        Vec3{4, 0, 0},
        Vec3{1, 1, 1},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    REQUIRE_FALSE(intersects(aabb, obb));
    REQUIRE_FALSE(intersects(obb, aabb));
}


TEST_CASE("Intersects AABB OBB - Overlapping enclosing AABBs do not imply OBB intersection") {
    constexpr AABB aabb{
        Vec3{1.95f, 1.95f, -0.1f},
        Vec3{2.05f, 2.05f, 0.1f}
    };

    const OBB obb{
        Vec3{0, 0, 0},
        Vec3{2, 1, 1},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    REQUIRE_FALSE(intersects(aabb, obb));
    REQUIRE_FALSE(intersects(obb, aabb));
}


TEST_CASE("Intersects AABB OBB - Point AABB inside OBB intersects") {
    constexpr AABB aabb{
        Vec3{0, 0, 0},
        Vec3{0, 0, 0}
    };

    const OBB obb{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(57)
                )
    };

    REQUIRE(intersects(aabb, obb));
    REQUIRE(intersects(obb, aabb));
}


// ============================================================================
// OBB <-> OBB
// ============================================================================

TEST_CASE("Intersects OBB OBB - Identical bounds intersect") {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    REQUIRE(intersects(bounds, bounds));
}


TEST_CASE("Intersects OBB OBB - Identity bounds overlap") {
    constexpr OBB a{
        Vec3{0, 0, 0},
        Vec3{2, 2, 2},
        Quaternion::identity()
    };

    constexpr OBB b{
        Vec3{3, 0, 0},
        Vec3{2, 2, 2},
        Quaternion::identity()
    };

    REQUIRE(intersects(a, b));
    REQUIRE(intersects(b, a));
}


TEST_CASE("Intersects OBB OBB - Identity bounds separated") {
    constexpr OBB a{
        Vec3{0, 0, 0},
        Vec3{1, 1, 1},
        Quaternion::identity()
    };

    constexpr OBB b{
        Vec3{3, 0, 0},
        Vec3{1, 1, 1},
        Quaternion::identity()
    };

    REQUIRE_FALSE(intersects(a, b));
    REQUIRE_FALSE(intersects(b, a));
}


TEST_CASE("Intersects OBB OBB - Touching faces intersect") {
    constexpr OBB a{
        Vec3{0, 0, 0},
        Vec3{1, 1, 1},
        Quaternion::identity()
    };

    constexpr OBB b{
        Vec3{2, 0, 0},
        Vec3{1, 1, 1},
        Quaternion::identity()
    };

    REQUIRE(intersects(a, b));
    REQUIRE(intersects(b, a));
}


TEST_CASE("Intersects OBB OBB - Contained OBB intersects") {
    constexpr OBB outer{
        Vec3{0, 0, 0},
        Vec3{5, 5, 5},
        Quaternion::identity()
    };

    const OBB inner{
        Vec3{1, 1, 1},
        Vec3{1, 1, 1},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(45)
                )
    };

    REQUIRE(intersects(outer, inner));
    REQUIRE(intersects(inner, outer));
}


TEST_CASE("Intersects OBB OBB - Rotated bounds overlap") {
    const OBB a{
        Vec3{0, 0, 0},
        Vec3{2, 1, 1},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    const OBB b{
        Vec3{2, 0, 0},
        Vec3{2, 1, 1},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(-30)
                )
    };

    REQUIRE(intersects(a, b));
    REQUIRE(intersects(b, a));
}


TEST_CASE("Intersects OBB OBB - Rotated bounds separated") {
    const OBB a{
        Vec3{0, 0, 0},
        Vec3{2, 1, 1},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    const OBB b{
        Vec3{6, 0, 0},
        Vec3{2, 1, 1},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(-30)
                )
    };

    REQUIRE_FALSE(intersects(a, b));
    REQUIRE_FALSE(intersects(b, a));
}


/*
 * This case is deliberately 3D.
 *
 * Testing only the three local axes of each OBB is not sufficient.
 * The separating axis exists on a cross-product axis between the boxes.
 */
TEST_CASE("Intersects OBB OBB - Cross product separating axis detects separation") {
    const OBB a{
        Vec3{0, 0, 0},
        Vec3{2, 0.5f, 0.5f},

        from_axis_angle(
                Vec3{1, 0, 0},
                Angle::from_degrees(30)
                )
    };

    const OBB b{
        Vec3{0, 0, 2.25f},
        Vec3{2, 0.5f, 0.5f},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(30)
                )
    };

    REQUIRE_FALSE(intersects(a, b));
    REQUIRE_FALSE(intersects(b, a));
}


TEST_CASE("Intersects OBB OBB - Point OBB inside bounds intersects") {
    constexpr OBB outer{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    constexpr OBB point{
        Vec3{1, 1, 1},
        Vec3{0, 0, 0},
        Quaternion::identity()
    };

    REQUIRE(intersects(outer, point));
    REQUIRE(intersects(point, outer));
}


// ============================================================================
// Plane <-> Sphere
// ============================================================================

TEST_CASE("Intersects Plane Sphere - Sphere crossing plane intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Sphere sphere{
        Vec3{0, 0.5f, 0},
        1.0f
    };

    REQUIRE(intersects(plane, sphere));
    REQUIRE(intersects(sphere, plane));
}


TEST_CASE("Intersects Plane Sphere - Sphere touching plane intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Sphere sphere{
        Vec3{0, 1, 0},
        1.0f
    };

    REQUIRE(intersects(plane, sphere));
    REQUIRE(intersects(sphere, plane));
}


TEST_CASE("Intersects Plane Sphere - Sphere above plane does not intersect") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Sphere sphere{
        Vec3{0, 2, 0},
        1.0f
    };

    REQUIRE_FALSE(intersects(plane, sphere));
    REQUIRE_FALSE(intersects(sphere, plane));
}


TEST_CASE("Intersects Plane Sphere - Sphere below plane does not intersect") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Sphere sphere{
        Vec3{0, -2, 0},
        1.0f
    };

    REQUIRE_FALSE(intersects(plane, sphere));
    REQUIRE_FALSE(intersects(sphere, plane));
}


TEST_CASE("Intersects Plane Sphere - Offset plane respects distance") {
    // y = 5

    constexpr Plane plane{
        Vec3{0, 1, 0},
        -5.0f
    };

    constexpr Sphere sphere{
        Vec3{0, 7, 0},
        2.0f
    };

    REQUIRE(intersects(plane, sphere));
    REQUIRE(intersects(sphere, plane));
}


TEST_CASE("Intersects Plane Sphere - Zero radius sphere on plane intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr Sphere sphere{
        Vec3{3, 0, -7},
        0.0f
    };

    REQUIRE(intersects(plane, sphere));
    REQUIRE(intersects(sphere, plane));
}


// ============================================================================
// Plane <-> AABB
// ============================================================================

TEST_CASE("Intersects Plane AABB - Plane crossing AABB intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr AABB bounds{
        Vec3{-1, -2, -1},
        Vec3{1, 2, 1}
    };

    REQUIRE(intersects(plane, bounds));
    REQUIRE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane AABB - AABB touching plane intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr AABB bounds{
        Vec3{-1, 0, -1},
        Vec3{1, 2, 1}
    };

    REQUIRE(intersects(plane, bounds));
    REQUIRE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane AABB - AABB above plane does not intersect") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr AABB bounds{
        Vec3{-1, 1, -1},
        Vec3{1, 3, 1}
    };

    REQUIRE_FALSE(intersects(plane, bounds));
    REQUIRE_FALSE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane AABB - AABB below plane does not intersect") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr AABB bounds{
        Vec3{-1, -3, -1},
        Vec3{1, -1, 1}
    };

    REQUIRE_FALSE(intersects(plane, bounds));
    REQUIRE_FALSE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane AABB - Offset plane intersects AABB") {
    // y = 5

    constexpr Plane plane{
        Vec3{0, 1, 0},
        -5.0f
    };

    constexpr AABB bounds{
        Vec3{-1, 4, -1},
        Vec3{1, 6, 1}
    };

    REQUIRE(intersects(plane, bounds));
    REQUIRE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane AABB - Point AABB on plane intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr AABB point{
        Vec3{2, 0, -4},
        Vec3{2, 0, -4}
    };

    REQUIRE(intersects(plane, point));
    REQUIRE(intersects(point, plane));
}


// ============================================================================
// Plane <-> OBB
// ============================================================================

TEST_CASE("Intersects Plane OBB - Plane crossing identity OBB intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 1, 3},
        Quaternion::identity()
    };

    REQUIRE(intersects(plane, bounds));
    REQUIRE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane OBB - Identity OBB touching plane intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr OBB bounds{
        Vec3{0, 1, 0},
        Vec3{2, 1, 3},
        Quaternion::identity()
    };

    REQUIRE(intersects(plane, bounds));
    REQUIRE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane OBB - Identity OBB separated from plane") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    constexpr OBB bounds{
        Vec3{0, 2, 0},
        Vec3{2, 0.5f, 3},
        Quaternion::identity()
    };

    REQUIRE_FALSE(intersects(plane, bounds));
    REQUIRE_FALSE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane OBB - Rotation can cause OBB to intersect plane") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    const OBB bounds{
        Vec3{0, 1.5f, 0},
        Vec3{2, 0.5f, 0.5f},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(90)
                )
    };

    // The long local X axis rotates onto world Y.
    // World Y extent therefore becomes 2.

    REQUIRE(intersects(plane, bounds));
    REQUIRE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane OBB - Rotated OBB touching plane intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    const OBB bounds{
        Vec3{0, 2, 0},
        Vec3{2, 0.5f, 0.5f},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(90)
                )
    };

    REQUIRE(intersects(plane, bounds));
    REQUIRE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane OBB - Rotated OBB separated from plane") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    const OBB bounds{
        Vec3{0, 2.1f, 0},
        Vec3{2, 0.5f, 0.5f},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(90)
                )
    };

    REQUIRE_FALSE(intersects(plane, bounds));
    REQUIRE_FALSE(intersects(bounds, plane));
}


TEST_CASE("Intersects Plane OBB - Point OBB on plane intersects") {
    constexpr Plane plane{
        Vec3{0, 1, 0},
        0.0f
    };

    const OBB point{
        Vec3{4, 0, -7},
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(57)
                )
    };

    REQUIRE(intersects(plane, point));
    REQUIRE(intersects(point, plane));
}
