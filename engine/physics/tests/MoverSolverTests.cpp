//
// Created by Sebastian Borsch on 06.10.25.
//
#include "FakeCollisionQueryService.hpp"
#include "../include/math/Types.hpp"
#include "collision/CollisionQueryService.hpp"
#include "collision/MoverSolver.hpp"
#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

using namespace Engine::Physics::Collision;
using namespace Engine::Physics;

TEST_CASE("MoverSolver stops at wall and slides", "[Physics]") {
    FakeCollisionQueryService query_service;
    Engine::Ecs::EntityId entity = 1ull;
    query_service.aabbs.emplace(entity, Math::AABB{{-1, -1, 0}, {1, 1, 2}});
    std::vector candidates = {entity};

    SECTION("Move Z-Axis negative") {
        MoverInput input;
        input.position = {0, 0, 5};
        input.radius = 0.5f;
        input.delta = {0, 0, -10};

        const auto res = MoverSolver::Solve(input, query_service, candidates);
        REQUIRE(res.collided);
        REQUIRE(std::abs(res.new_position.z - 2.5f) < 1e-3f);
        REQUIRE(std::abs(res.last_normal.z - -1.0f) < 1e-4f);
    }

    SECTION("Move Z-Axis positive") {
        MoverInput input;
        input.position = {0, 0, -5};
        input.radius = 0.5f;
        input.delta = {0, 0, 10};

        const auto res = MoverSolver::Solve(input, query_service, candidates);
        REQUIRE(res.collided);
        REQUIRE(std::abs(res.new_position.z - -0.5f) < 1e-3f);
        REQUIRE(std::abs(res.last_normal.z - -1.0f) < 1e-4f);
    }

    SECTION("Move X-Axis negative") {
        MoverInput input;
        input.position = {5, 0, 0};
        input.radius = 0.5f;
        input.delta = {-5, 0, 0};

        const auto res = MoverSolver::Solve(input, query_service, candidates);
        REQUIRE(res.collided);
        REQUIRE(std::abs(res.new_position.x - 1.5f) < 1e-3f);
        REQUIRE(std::abs(res.last_normal.x - -1.0f) < 1e-4f);
    }
    SECTION("Move X-Axis positive") {
        MoverInput input;
        input.position = {-5, 0, 0};
        input.radius = 0.5f;
        input.delta = {5, 0, 0};

        const auto res = MoverSolver::Solve(input, query_service, candidates);
        REQUIRE(res.collided);
        REQUIRE(std::abs(res.new_position.x - -1.5f) < 1e-3f);
        REQUIRE(std::abs(res.last_normal.x - 1.0f) < 1e-4f);
    }
}
