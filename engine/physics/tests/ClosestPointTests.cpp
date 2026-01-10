#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/math/ClosestPoint.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

using namespace Engine::Physics::Math;

static bool ApproxVec3(const glm::vec3& a, const glm::vec3& b, float eps = 1e-5f) {
    return glm::length2(a - b) <= eps * eps;
}

TEST_CASE("math.ClosestPointTests(AABB) - Point inside returns same point") {
    constexpr AABB box{{-1, -1, -1}, {1, 1, 1}};
    constexpr glm::vec3 test_point{0.5, 0.5, 0.5};
    const glm::vec3 closest_point = ClosestPoint(test_point, box);
    REQUIRE(ApproxVec3(test_point, closest_point));
}

TEST_CASE("math.ClosestPoint(AABB) - Clamps outside on single axis") {
    constexpr AABB box{{-1, -1, -1}, {1, 1, 1}};
    constexpr glm::vec3 point{5, 0.2f, -0.3f};
    constexpr glm::vec3 expected{1, 0.2f, -0.3f};

    const glm::vec3 closest_point = ClosestPoint(point, box);

    REQUIRE(ApproxVec3(closest_point, expected));
}

TEST_CASE("math.ClosestPoint(AABB) - clamps to corner") {
    constexpr AABB box{{-1, -1, -1}, {1, 1, 1}};

    constexpr glm::vec3 point{5, 6, 7};
    constexpr glm::vec3 expected{1, 1, 1};

    const glm::vec3 closest_point = ClosestPoint(point, box);

    REQUIRE(ApproxVec3(closest_point, expected));
}

TEST_CASE("math.ClosestPoint(AABB) - point on face stays on face") {
    constexpr AABB box{{-1, -1, -1}, {1, 1, 1}};

    constexpr glm::vec3 point{1, 0.4f, -0.9f}; // x genau auf max-face
    constexpr glm::vec3 expected = point;

    const glm::vec3 closest_point = ClosestPoint(point, box);

    REQUIRE(ApproxVec3(closest_point, expected));
}

TEST_CASE("math.ClosestPoint(OBB) - identity orientation behaves like AABB around center") {
    OBB box{};
    box.center = {10, 0, -2};
    box.half_extents = {1, 2, 3};
    box.orientation = glm::mat3(1.0f); // identity

    glm::vec3 point{20, 1, -10};

    glm::vec3 local = point - box.center;
    glm::vec3 clamped{
        glm::clamp(local.x, -box.half_extents.x, box.half_extents.x),
        glm::clamp(local.y, -box.half_extents.y, box.half_extents.y),
        glm::clamp(local.z, -box.half_extents.z, box.half_extents.z),
    };
    glm::vec3 expected = box.center + clamped;

    glm::vec3 closest_point = ClosestPoint(point, box);

    REQUIRE(ApproxVec3(closest_point, expected));
}

TEST_CASE("math.ClosestPoint(OBB) - rotated box clamps in local space") {
    OBB box{};
    box.center = {0, 0, 0};
    box.half_extents = {1, 2, 1};

    auto angle = glm::half_pi<float>();
    glm::mat4 r4 = rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
    box.orientation = glm::mat3(r4);

    // local = (3, 1, -4) -> clamp -> (1, 1, -1)
    glm::vec3 local_wanted{3, 1, -4};
    glm::vec3 point = box.center + box.orientation * local_wanted;

    glm::vec3 expected_world = box.center + box.orientation * glm::vec3(1, 1, -1);

    glm::vec3 closest_point = ClosestPoint(point, box);

    REQUIRE(ApproxVec3(closest_point, expected_world));
}

TEST_CASE("math.ClosestPoint(OBB) - point inside returns the point (any rotation)") {
    OBB box{};
    box.center = {5, -2, 1};
    box.half_extents = {2, 2, 2};

    constexpr float angle = 0.7f;
    const glm::mat4 r4 = rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
    box.orientation = glm::mat3(r4);

    constexpr glm::vec3 local_inside{0.5f, -1.5f, 1.0f};
    const glm::vec3 point = box.center + box.orientation * local_inside;

    const glm::vec3 closest_point = ClosestPoint(point, box);

    REQUIRE(ApproxVec3(closest_point, point));
}
