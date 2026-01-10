#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/math/Overlap.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp> // length2
#include <cmath>
using namespace Engine::Physics::Math;

static bool ApproxVec3(const glm::vec3& a, const glm::vec3& b, float eps = 1e-5f) {
    return length2(a - b) <= eps * eps;
}
// --------------------------- AABB vs AABB ---------------------------

TEST_CASE("math.Overlap(AABB,AABB) - overlapping volumes returns true")
{
    constexpr AABB bounds_a{ glm::vec3(0, 0, 0), glm::vec3(2, 2, 2) };
    constexpr AABB bounds_b{ glm::vec3(1, 1, 1), glm::vec3(3, 3, 3) };

    const bool overlaps = Overlap(bounds_a, bounds_b);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(AABB,AABB) - separated on x axis returns false")
{
    constexpr AABB bounds_a{ glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) };
    constexpr AABB bounds_b{ glm::vec3(2, 0, 0), glm::vec3(3, 1, 1) };

    const bool overlaps = Overlap(bounds_a, bounds_b);

    REQUIRE_FALSE(overlaps);
}

TEST_CASE("math.Overlap(AABB,AABB) - separated on y axis returns false")
{
    constexpr AABB bounds_a{ glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) };
    constexpr AABB bounds_b{ glm::vec3(0, 2, 0), glm::vec3(1, 3, 1) };

    const bool overlaps = Overlap(bounds_a, bounds_b);

    REQUIRE_FALSE(overlaps);
}

TEST_CASE("math.Overlap(AABB,AABB) - separated on z axis returns false")
{
    constexpr AABB bounds_a{ glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) };
    constexpr AABB bounds_b{ glm::vec3(0, 0, 2), glm::vec3(1, 1, 3) };

    const bool overlaps = Overlap(bounds_a, bounds_b);

    REQUIRE_FALSE(overlaps);
}

TEST_CASE("math.Overlap(AABB,AABB) - touching at face counts as overlap")
{
    // Your implementation uses < and >, so equality on faces should still overlap.
    constexpr AABB bounds_a{ glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) };
    constexpr AABB bounds_b{ glm::vec3(1, 0, 0), glm::vec3(2, 1, 1) };

    const bool overlaps = Overlap(bounds_a, bounds_b);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(AABB,AABB) - one box fully inside the other returns true")
{
    constexpr AABB outer_bounds{ glm::vec3(-5, -5, -5), glm::vec3(5, 5, 5) };
    constexpr AABB inner_bounds{ glm::vec3(-1, -2, -3), glm::vec3(1, 2, 3) };

    const bool overlaps = Overlap(outer_bounds, inner_bounds);

    REQUIRE(overlaps);
}

// ------------------------ Sphere vs Sphere --------------------------

TEST_CASE("math.Overlap(Sphere,Sphere) - overlapping spheres returns true")
{
    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 2.0f };
    constexpr Sphere other_sphere{ glm::vec3(3, 0, 0), 2.0f }; // distance 3, radii sum 4

    const bool overlaps = Overlap(sphere, other_sphere);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,Sphere) - separated spheres returns false")
{
    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 1.0f };
    constexpr Sphere other_sphere{ glm::vec3(3, 0, 0), 1.0f }; // distance 3, radii sum 2

    const bool overlaps = Overlap(sphere, other_sphere);

    REQUIRE_FALSE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,Sphere) - touching spheres counts as overlap")
{
    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 1.0f };
    constexpr Sphere other_sphere{ glm::vec3(2, 0, 0), 1.0f }; // distance 2, radii sum 2

    const bool overlaps = Overlap(sphere, other_sphere);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,Sphere) - same center always overlaps if radii are non-negative")
{
    constexpr Sphere sphere{ glm::vec3(1, 2, 3), 0.5f };
    constexpr Sphere other_sphere{ glm::vec3(1, 2, 3), 0.0f };

    const bool overlaps = Overlap(sphere, other_sphere);

    REQUIRE(overlaps);
}

// ------------------------- Sphere vs AABB ---------------------------

TEST_CASE("math.Overlap(Sphere,AABB) - sphere center inside box returns true")
{
    constexpr Sphere sphere{ glm::vec3(0.2f, -0.3f, 0.4f), 0.1f };
    constexpr AABB bounding_box{ glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1) };

    const bool overlaps = Overlap(sphere, bounding_box);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,AABB) - sphere far away returns false")
{
    constexpr Sphere sphere{ glm::vec3(10, 0, 0), 0.9f };
    constexpr AABB bounding_box{ glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1) };

    const bool overlaps = Overlap(sphere, bounding_box);

    REQUIRE_FALSE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,AABB) - sphere touching box face counts as overlap")
{
    constexpr AABB bounding_box{ glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1) };

    // Closest point will be (1,0,0), distance from center to closest is exactly radius.
    constexpr Sphere sphere{ glm::vec3(2, 0, 0), 1.0f };

    const bool overlaps = Overlap(sphere, bounding_box);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,AABB) - sphere overlapping near box corner returns true")
{
    constexpr AABB bounding_box{ glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1) };

    // Nearest corner is (1,1,1). Center is at (2,2,2). Distance^2 = 3, so radius > sqrt(3) overlaps.
    constexpr Sphere sphere{ glm::vec3(2, 2, 2), 2.0f };

    const bool overlaps = Overlap(sphere, bounding_box);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,AABB) - sphere just outside corner returns false")
{
    constexpr AABB bounding_box{ glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1) };

    // Center (2,2,2) -> distance to corner (1,1,1) is sqrt(3) ~ 1.732
    constexpr Sphere sphere{ glm::vec3(2, 2, 2), 1.7f };

    const bool overlaps = Overlap(sphere, bounding_box);

    REQUIRE_FALSE(overlaps);
}

// -------------------------- Sphere vs OBB ---------------------------

TEST_CASE("math.Overlap(Sphere,OBB) - identity orientation matches AABB behavior")
{
    OBB obb{};
    obb.center = { 0, 0, 0 };
    obb.half_extents = { 1, 2, 3 };
    obb.orientation = glm::mat3(1.0f);

    constexpr Sphere sphere{ glm::vec3(2, 0, 0), 1.0f }; // touches at local x = 1

    const bool overlaps = Overlap(sphere, obb);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,OBB) - rotated box and sphere center inside returns true")
{
    OBB obb{};
    obb.center = { 5, -2, 1 };
    obb.half_extents = { 2, 1, 3 };

    constexpr float angle = 0.7f;
    const glm::mat4 r4 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
    obb.orientation = glm::mat3(r4);

    constexpr glm::vec3 local_inside{ 1.0f, 0.5f, -2.0f };
    const glm::vec3 sphere_center = obb.center + obb.orientation * local_inside;

    const Sphere sphere{ sphere_center, 0.25f };

    const bool overlaps = Overlap(sphere, obb);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,OBB) - rotated box and sphere far away returns false")
{
    OBB obb{};
    obb.center = { 0, 0, 0 };
    obb.half_extents = { 1, 1, 1 };

    constexpr float angle = 1.1f;
    const glm::mat4 r4 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
    obb.orientation = glm::mat3(r4);

    constexpr Sphere sphere{ glm::vec3(10, 0, 0), 0.5f };

    const bool overlaps = Overlap(sphere, obb);

    REQUIRE_FALSE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,OBB) - sphere touching rotated box face counts as overlap")
{
    OBB obb{};
    obb.center = { 0, 0, 0 };
    obb.half_extents = { 2, 1, 1 };

    constexpr float angle = glm::half_pi<float>(); // 90 degrees
    const glm::mat4 r4 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
    obb.orientation = glm::mat3(r4);

    // Choose a point that is exactly on the local +X face, then move outward along local +X by radius.
    // local face point: ( +2, 0, 0 )
    // sphere center in local: ( +2 + radius, 0, 0 )
    constexpr float radius = 0.5f;
    constexpr glm::vec3 local_sphere_center{ 2.0f + radius, 0.0f, 0.0f };

    const glm::vec3 sphere_center = obb.center + obb.orientation * local_sphere_center;
    const Sphere sphere{ sphere_center, radius };

    const bool overlaps = Overlap(sphere, obb);

    REQUIRE(overlaps);
}

TEST_CASE("math.Overlap(Sphere,OBB) - sphere just outside rotated box face returns false")
{
    OBB obb{};
    obb.center = { 0, 0, 0 };
    obb.half_extents = { 2, 1, 1 };

    constexpr float angle = glm::half_pi<float>(); // 90 degrees
    const glm::mat4 r4 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
    obb.orientation = glm::mat3(r4);

    // local x face at +2, choose sphere center at +2 + radius + epsilon => no overlap
    constexpr float radius = 0.5f;
    constexpr float epsilon = 1e-3f;
    constexpr glm::vec3 local_sphere_center{ 2.0f + radius + epsilon, 0.0f, 0.0f };

    const glm::vec3 sphere_center = obb.center + obb.orientation * local_sphere_center;
    const Sphere sphere{ sphere_center, radius };

    const bool overlaps = Overlap(sphere, obb);

    REQUIRE_FALSE(overlaps);
}