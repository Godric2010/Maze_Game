#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../include/math/TypeUtils.hpp"

void AssertVec3IsAsExpected(const glm::vec3 &expected, const glm::vec3 &actual) {
    REQUIRE(std::abs(expected.x - actual.x) < 1E-5f);
    REQUIRE(std::abs(expected.y - actual.y) < 1E-5f);
    REQUIRE(std::abs(expected.z - actual.z) < 1E-5f);
}

TEST_CASE("Sphere Utilities - Resulting bounding box is as expected") {
    SECTION(
        "Sphere with valid radius - Bounding box center is sphere position") {
        constexpr auto s = Engine::Physics::Math::Sphere{
            .center = glm::vec3{4, 2, 1},
            .radius = 5
        };

        const auto [min, max] = Engine::Physics::Math::Util::FromSphere(s);
        const auto result_center = min + ((max - min) * 0.5f);
        AssertVec3IsAsExpected(s.center, result_center);
    }

    SECTION("Sphere with negative radius - No Bounding box gets build") {
        constexpr auto s = Engine::Physics::Math::Sphere{
            .center = glm::vec3{4, 2, 1},
            .radius = -5
        };

        REQUIRE_THROWS(Engine::Physics::Math::Util::FromSphere(s));
    }

    SECTION("Sphere with no radius - Exception gets thrown") {
        constexpr auto s = Engine::Physics::Math::Sphere{
            .center = glm::vec3{4, 2, 1},
            .radius = 0,
        };

        REQUIRE_THROWS(Engine::Physics::Math::Util::FromSphere(s));
    }
}

TEST_CASE("Orientated Bounding Box") {
    constexpr float width = 2.0f;
    constexpr float height = 1.0f;
    constexpr float depth = 1.0f;

    constexpr auto expected_half_extents = glm::vec3{1.0f, 0.5f, 0.5f};

    SECTION("Create OBB with no rotation - All values are as expected") {
        constexpr auto position = glm::vec3{1, 1, 1};
        auto obb = Engine::Physics::Math::Util::BuildWorldObb(position, glm::vec3(0), width, height, depth);
        AssertVec3IsAsExpected(position, obb.center);
        AssertVec3IsAsExpected(expected_half_extents, obb.half_extents);
    }




}
