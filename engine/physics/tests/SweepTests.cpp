//
// Created by Sebastian Borsch on 06.10.25.
//

#include "../include/math/Types.hpp"
#include "math/Sweep.hpp"
#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch_all.hpp>
#endif

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp> // length2
#include <cmath>
using namespace Engine::Physics::Math;

// ---- Helpers ----
static bool ApproxVec3(const glm::vec3& a, const glm::vec3& b, float eps = 1e-5f)
{
    return length2(a - b) <= eps * eps;
}

static bool ApproxFloat(float a, float b, float eps = 1e-5f)
{
    return std::fabs(a - b) <= eps;
}

static bool IsUnitOrZero(const glm::vec3& v, float eps = 1e-4f)
{
    const float len2 = length2(v);
    if (len2 <= eps * eps) return true;
    return std::fabs(std::sqrt(len2) - 1.0f) <= eps;
}

// ------------------------------ Sweep Sphere vs AABB ------------------------------

TEST_CASE("math.Sweep(Sphere,AABB) - zero motion returns no hit")
{
    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 0.5f };
    constexpr glm::vec3 motion_vector{ 0, 0, 0 };
    constexpr AABB bounding_box{ glm::vec3(5, -1, -1), glm::vec3(7, 1, 1) };

    const CollisionHit hit = Sweep(sphere, motion_vector, bounding_box);

    REQUIRE_FALSE(hit.hit);
}

TEST_CASE("math.Sweep(Sphere,AABB) - miss returns no hit")
{
    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 0.5f };
    constexpr glm::vec3 motion_vector{ 0, 5, 0 }; // moves up, box is to the right
    constexpr AABB bounding_box{ glm::vec3(5, -1, -1), glm::vec3(7, 1, 1) };

    const CollisionHit hit = Sweep(sphere, motion_vector, bounding_box);

    REQUIRE_FALSE(hit.hit);
}

TEST_CASE("math.Sweep(Sphere,AABB) - hits expanded box on x axis with correct time of impact and normal")
{
    // Box spans x=[5..7], expanded by radius 1 => x=[4..8]
    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 1.0f };
    constexpr AABB bounding_box{ glm::vec3(5, -1, -1), glm::vec3(7, 1, 1) };

    // Ray from x=0 along +X hits expanded min x=4 at t=4
    constexpr glm::vec3 motion_vector{ 10, 0, 0 };

    const CollisionHit hit = Sweep(sphere, motion_vector, bounding_box);

    REQUIRE(hit.hit);
    REQUIRE(ApproxFloat(hit.time_of_impact, 4.0f));
    REQUIRE(ApproxVec3(hit.point, glm::vec3(4, 0, 0)));

    // Entry at min x face => normal should be (-1,0,0)
    REQUIRE(ApproxVec3(hit.normal, glm::vec3(-1, 0, 0)));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.0f));
    REQUIRE(IsUnitOrZero(hit.normal));
}

TEST_CASE("math.Sweep(Sphere,AABB) - starting inside expanded box clamps time of impact to zero")
{
    // Box spans x=[5..7], expanded by radius 1 => x=[4..8]
    constexpr Sphere sphere{ glm::vec3(4.5f, 0, 0), 1.0f }; // already inside expanded range (x in [4..8])
    constexpr AABB bounding_box{ glm::vec3(5, -1, -1), glm::vec3(7, 1, 1) };
    constexpr glm::vec3 motion_vector{ 10, 0, 0 };

    const CollisionHit hit = Sweep(sphere, motion_vector, bounding_box);

    REQUIRE(hit.hit);
    REQUIRE(ApproxFloat(hit.time_of_impact, 0.0f));
    REQUIRE(ApproxVec3(hit.point, sphere.center)); // point = center + dir * 0

    // The normal depends on which face NormalFromEntryPoint chooses at that point.
    // We only assert its axis-aligned unit vector.
    REQUIRE(IsUnitOrZero(hit.normal));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.0f));
}

TEST_CASE("math.Sweep(Sphere,AABB) - motion shorter than needed to reach box returns no hit")
{
    // Expanded min x=4, start at 0, need length >= 4
    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 1.0f };
    constexpr AABB bounding_box{ glm::vec3(5, -1, -1), glm::vec3(7, 1, 1) };
    constexpr glm::vec3 motion_vector{ 3.0f, 0, 0 }; // too short

    const CollisionHit hit = Sweep(sphere, motion_vector, bounding_box);

    REQUIRE_FALSE(hit.hit);
}

TEST_CASE("math.Sweep(Sphere,AABB) - parallel ray outside slab returns no hit")
{
    // Direction along +Y, but x coordinate outside expanded x-range, so RayAABB_tEnter should early-out
    constexpr Sphere sphere{ glm::vec3(100, 0, 0), 1.0f };
    constexpr AABB bounding_box{ glm::vec3(5, -1, -1), glm::vec3(7, 1, 1) };
    constexpr glm::vec3 motion_vector{ 0, 10, 0 };

    const CollisionHit hit = Sweep(sphere, motion_vector, bounding_box);

    REQUIRE_FALSE(hit.hit);
}

// ------------------------------ Sweep Sphere vs OBB ------------------------------

TEST_CASE("math.Sweep(Sphere,OBB) - identity orientation matches AABB sweep result")
{
    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 1.0f };
    constexpr glm::vec3 motion_vector{ 10, 0, 0 };

    constexpr AABB aabb_box{ glm::vec3(5, -1, -1), glm::vec3(7, 1, 1) };

    OBB obb_box{};
    obb_box.center = (aabb_box.min + aabb_box.max) * 0.5f;
    obb_box.half_extents = (aabb_box.max - aabb_box.min) * 0.5f;
    obb_box.orientation = glm::mat3(1.0f);

    const CollisionHit hit_aabb = Sweep(sphere, motion_vector, aabb_box);
    const CollisionHit hit_obb = Sweep(sphere, motion_vector, obb_box);

    REQUIRE(hit_aabb.hit);
    REQUIRE(hit_obb.hit);

    REQUIRE(ApproxFloat(hit_obb.time_of_impact, hit_aabb.time_of_impact));
    REQUIRE(ApproxVec3(hit_obb.point, hit_aabb.point));
    REQUIRE(ApproxVec3(hit_obb.normal, hit_aabb.normal));
    REQUIRE(ApproxFloat(hit_obb.penetration_depth, hit_aabb.penetration_depth));
}

TEST_CASE("math.Sweep(Sphere,OBB) - rotated OBB is hit and normal is unit length")
{
    // Make an OBB centered at (6,0,0) roughly like the AABB but rotated around Z.
    OBB obb_box{};
    obb_box.center = { 6, 0, 0 };
    obb_box.half_extents = { 1, 1, 1 };

    constexpr float angle = glm::quarter_pi<float>(); // 45 degrees
    const glm::mat4 rotation_matrix = rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
    obb_box.orientation = glm::mat3(rotation_matrix);

    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 0.5f };
    constexpr glm::vec3 motion_vector{ 20, 0, 0 };

    const CollisionHit hit = Sweep(sphere, motion_vector, obb_box);

    REQUIRE(hit.hit);
    REQUIRE(hit.time_of_impact >= 0.0f);
    REQUIRE(hit.time_of_impact <= glm::length(motion_vector));
    REQUIRE(IsUnitOrZero(hit.normal));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.0f));
}

TEST_CASE("math.Sweep(Sphere,OBB) - miss rotated OBB returns no hit")
{
    OBB obb_box{};
    obb_box.center = { 6, 10, 0 };      // shifted up so we miss
    obb_box.half_extents = { 1, 1, 1 };

    constexpr float angle = 0.9f;
    const glm::mat4 rotation_matrix = rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
    obb_box.orientation = glm::mat3(rotation_matrix);

    constexpr Sphere sphere{ glm::vec3(0, 0, 0), 0.5f };
    constexpr glm::vec3 motion_vector{ 20, 0, 0 };

    const CollisionHit hit = Sweep(sphere, motion_vector, obb_box);

    REQUIRE_FALSE(hit.hit);
}

TEST_CASE("math.Sweep(Sphere,OBB) - comparing world result with local space sweep transformation")
{
    // This test checks the "ToLocalSphereAndMotion + Sweep(local) + transform back" logic indirectly.
    // We construct a scenario and verify that transforming into local space and back preserves the hit time,
    // and that world point is consistent with motion.

    OBB obb_box{};
    obb_box.center = { 5, -2, 1 };
    obb_box.half_extents = { 2, 1, 1 };

    constexpr float angle = 0.7f;
    const glm::mat4 rotation_matrix = rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
    obb_box.orientation = glm::mat3(rotation_matrix);

    constexpr Sphere sphere{ glm::vec3(-10, -2, 1), 0.5f };
    constexpr glm::vec3 motion_vector{ 30, 0, 0 };
    const float motion_length = length(motion_vector);
    const glm::vec3 motion_direction = motion_vector / motion_length;

    const CollisionHit hit = Sweep(sphere, motion_vector, obb_box);

    REQUIRE(hit.hit);
    REQUIRE(hit.time_of_impact >= 0.0f);
    REQUIRE(hit.time_of_impact <= motion_length);

    // point must lie on the swept segment: sphere.center + dir * toi
    const glm::vec3 expected_point_on_path = sphere.center + motion_direction * hit.time_of_impact;
    REQUIRE(ApproxVec3(hit.point, expected_point_on_path));

    REQUIRE(IsUnitOrZero(hit.normal));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.0f));
}

