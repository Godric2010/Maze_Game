// ReSharper disable CppUseStructuredBinding
#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/math/Resolve.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>          // length2
#include <glm/gtc/epsilon.hpp>       // epsilonEqual
#include <cmath>

using namespace Engine::Physics::Math;
// ---- Helpers ----
static bool ApproxVec3(const glm::vec3& a, const glm::vec3& b, const float eps = 1e-5f) {
    return length2(a - b) <= eps * eps;
}

static bool ApproxFloat(const float a, const float b, const float eps = 1e-5f) {
    return std::fabs(a - b) <= eps;
}

// -------------------- Penetration(Sphere, AABB) --------------------

TEST_CASE("math.Penetration(Sphere,AABB) - no overlap returns hit false") {
    constexpr AABB bounding_box{glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)};
    constexpr Sphere sphere{glm::vec3(10, 0, 0), 0.25f};

    const CollisionHit hit = Penetration(sphere, bounding_box);

    REQUIRE_FALSE(hit.hit);
    REQUIRE(ApproxVec3(hit.normal, glm::vec3(0.0f)));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.0f));
    REQUIRE(ApproxVec3(hit.point, glm::vec3(0.0f)));
    REQUIRE(ApproxFloat(hit.time_of_impact, 0.0f));
}

TEST_CASE("math.Penetration(Sphere,AABB) - touching box face returns hit true and expected penetration") {
    constexpr AABB bounding_box{glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)};
    constexpr Sphere sphere{glm::vec3(2, 0, 0), 1.0f};

    const CollisionHit hit = Penetration(sphere, bounding_box);

    REQUIRE(hit.hit);
    REQUIRE(ApproxVec3(hit.point, glm::vec3(1,0,0)));
    REQUIRE(ApproxVec3(hit.normal, glm::vec3(1,0,0)));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.0f));
    REQUIRE(ApproxFloat(hit.time_of_impact, 0.0f));
}

TEST_CASE("math.Penetration(Sphere,AABB) - sphere overlaps near face returns correct normal and penetration depth") {
    constexpr AABB bounding_box{glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)};
    constexpr Sphere sphere{glm::vec3(1.5f, 0, 0), 1.0f};

    const CollisionHit hit = Penetration(sphere, bounding_box);

    REQUIRE(hit.hit);
    REQUIRE(ApproxVec3(hit.point, glm::vec3(1,0,0)));
    REQUIRE(ApproxVec3(hit.normal, glm::vec3(1,0,0)));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.5f));
}

TEST_CASE("math.Penetration(Sphere,AABB) - sphere center inside chooses nearest face normal") {
    constexpr AABB bounding_box{glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)};

    // Center closer to +X face than others
    constexpr Sphere sphere{glm::vec3(0.9f, 0.0f, 0.0f), 1.0f};

    const CollisionHit hit = Penetration(sphere, bounding_box);

    REQUIRE(hit.hit);
    REQUIRE(ApproxVec3(hit.normal, glm::vec3(1, 0, 0)));
    REQUIRE(ApproxFloat(hit.penetration_depth, 1.0f)); // with the chosen convention in the inside-case
}

// -------------------- Penetration(AABB, AABB) --------------------

TEST_CASE("math.Penetration(AABB,AABB) - no overlap returns hit false") {
    constexpr AABB bounds_a{glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)};
    constexpr AABB bounds_b{glm::vec3(3, 0, 0), glm::vec3(4, 1, 1)};

    const CollisionHit hit = Penetration(bounds_a, bounds_b);

    REQUIRE_FALSE(hit.hit);
    REQUIRE(ApproxVec3(hit.normal, glm::vec3(0.0f)));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.0f));
    REQUIRE(ApproxVec3(hit.point, glm::vec3(0.0f)));
    REQUIRE(ApproxFloat(hit.time_of_impact, 0.0f));
}

TEST_CASE("math.Penetration(AABB,AABB) - overlap picks x axis when overlap_x is smallest") {
    constexpr AABB bounds_a{glm::vec3(0, 0, 0), glm::vec3(2, 2, 2)};
    constexpr AABB bounds_b{glm::vec3(1.5f, 0, 0), glm::vec3(3.0f, 2, 2)};

    // overlap_x = min(2,3) - max(0,1.5) = 2 - 1.5 = 0.5
    const CollisionHit hit = Penetration(bounds_a, bounds_b);

    REQUIRE(hit.hit);
    REQUIRE(ApproxVec3(hit.normal, glm::vec3(-1, 0, 0))); // center_a.x < center_b.x
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.5f));
}

TEST_CASE("math.Penetration(AABB,AABB) - overlap picks y axis when overlap_y is smallest") {
    constexpr AABB bounds_a{glm::vec3(0, 0, 0), glm::vec3(2, 2, 2)};
    constexpr AABB bounds_b{glm::vec3(0, 1.75f, 0), glm::vec3(2, 3.0f, 2)};

    // overlap_y = min(2,3) - max(0,1.75) = 0.25
    const CollisionHit hit = Penetration(bounds_a, bounds_b);

    REQUIRE(hit.hit);
    REQUIRE(ApproxVec3(hit.normal, glm::vec3(0, -1, 0)));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.25f));
}

TEST_CASE("math.Penetration(AABB,AABB) - overlap picks z axis when overlap_z is smallest") {
    constexpr AABB bounds_a{glm::vec3(0, 0, 0), glm::vec3(2, 2, 2)};
    constexpr AABB bounds_b{glm::vec3(0, 0, 1.9f), glm::vec3(2, 2, 3.0f)};

    // overlap_z = min(2,3) - max(0,1.9) = 0.1
    const CollisionHit hit = Penetration(bounds_a, bounds_b);

    REQUIRE(hit.hit);
    REQUIRE(ApproxVec3(hit.normal, glm::vec3(0, 0, -1)));
    REQUIRE(ApproxFloat(hit.penetration_depth, 0.1f));
}

// ------------------------------- Slide -------------------------------

TEST_CASE("math.Slide(vec,normal) - removes the normal component") {
    constexpr glm::vec3 vec{1.0f, 1.0f, 0.0f};
    constexpr glm::vec3 normal{0.0f, 1.0f, 0.0f};

    const glm::vec3 slid_vector = Slide(vec, normal);

    REQUIRE(ApproxVec3(slid_vector, glm::vec3(1.0f, 0.0f, 0.0f)));
}

TEST_CASE("math.Slide(vec,normal) - parallel vector becomes zero") {
    constexpr glm::vec3 vec{0.0f, 2.0f, 0.0f};
    constexpr glm::vec3 normal{0.0f, 1.0f, 0.0f};

    const glm::vec3 slid_vector = Slide(vec, normal);

    REQUIRE(ApproxVec3(slid_vector, glm::vec3(0.0f)));
}
