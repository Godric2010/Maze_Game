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

using namespace Engine::Physics;

TEST_CASE("Sweep - Sphere Tests", "[Physics]") {
    Math::Sphere test_sphere{.center = glm::vec3(1.0f, 1.0f, 0.0f), .radius = 1.0f};

    SECTION("Box overlapping with sphere") {
        constexpr auto box_center = glm::vec3(2.1f, 1.0f, 0.0f);
        constexpr float box_radius = 1.0f;
        Math::AABB test_box{.min = box_center - box_radius, .max = box_center + box_radius};

        auto direction = glm::vec3(1.0f, 0.0f, 0.0f);

        auto collision_hit = Sweep(test_sphere, direction, test_box);
        REQUIRE(collision_hit.hit);
        REQUIRE(collision_hit.normal == glm::vec3(-1.0f, 0.0f, 0.0f));
        REQUIRE(collision_hit.time_of_impact == 0.0f);
    }

    SECTION("Box is close to sphere and in movement direction") {
        constexpr auto box_center = glm::vec3(2.5f, 1.0f, 0.0f);
        constexpr float box_radius = 1.0f;
        Math::AABB test_box{.min = box_center, .max = box_center + box_radius};
        auto direction = glm::vec3(1.0f, 0.0f, 0.0f);
        auto collision_hit = Sweep(test_sphere, direction, test_box);
        REQUIRE(collision_hit.hit);
        REQUIRE(collision_hit.normal == glm::vec3(-1.0f, 0.0f, 0.0f));
        REQUIRE(collision_hit.time_of_impact == 0.5f);
    }

    SECTION("Box not yet overlapping with sphere") {
        constexpr auto box_center = glm::vec3(3.1f, 1.0f, 0.0f);
        constexpr float box_radius = 1.0f;
        Math::AABB test_box{.min = box_center, .max = box_center + box_radius};
        auto direction = glm::vec3(1.0f, 0.0f, 0.0f);
        auto collision_hit = Sweep(test_sphere, direction, test_box);
        REQUIRE_FALSE(collision_hit.hit);
    }

    SECTION("Box is not in the movement path") {
        constexpr auto box_center = glm::vec3(3.0f, 1.0f, 0.0f);
        constexpr float box_radius = 1.0f;
        Math::AABB test_box{.min = box_center, .max = box_center + box_radius};

        auto direction = glm::vec3(0.0f, 0.0f, 1.0f);
        auto collision_hit = Sweep(test_sphere, direction, test_box);
        REQUIRE_FALSE(collision_hit.hit);
    }

    SECTION("Box is in opposite movement path") {
        constexpr auto box_center = glm::vec3(3.0f, 1.0f, 0.0f);
        constexpr float box_radius = 1.0f;
        Math::AABB test_box{.min = box_center, .max = box_center + box_radius};
        auto direction = glm::vec3(-1.0f, 0.0f, 0.0f);
        auto collision_hit = Sweep(test_sphere, direction, test_box);
        REQUIRE_FALSE(collision_hit.hit);
    }
}
