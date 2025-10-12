#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../src/ComponentPool.hpp"
#include "../src/Entity.hpp"

using namespace Engine::Ecs;

class TestClass {
public:
    uint32_t testValue;
};

TEST_CASE("ComponentPool::Add - Add new entity and component", "[ecs][fast]") {
    constexpr EntityId entityA = 1u;
    constexpr EntityId entityB = 2u;
    ComponentPool<TestClass> pool;

    pool.Add(entityA, TestClass{.testValue = 1});
    pool.Add(entityB, TestClass{.testValue = 2});

    REQUIRE(pool.Count() == 2);
    REQUIRE(pool.Get(entityA)->testValue == 1);
    REQUIRE(pool.Get(entityB)->testValue == 2);
}

TEST_CASE("ComponentPool::Add - Add entity, that already exists", "[ecs][fast]") {
    constexpr EntityId entityA = 1u;
    ComponentPool<TestClass> pool;
    pool.Add(entityA, TestClass{.testValue = 1});

    pool.Add(entityA, TestClass{.testValue = 2});
    REQUIRE(pool.Count() == 1);
    REQUIRE(pool.Get(entityA)->testValue == 1);
}

TEST_CASE("ComponentPool::Add - Add invalid Entity", "[ecs][fast]") {
    constexpr EntityId entityA = 0u;
    ComponentPool<TestClass> pool;
    REQUIRE_THROWS(pool.Add(entityA, TestClass{.testValue = 1}));
}

TEST_CASE("ComponentPool::Remove - Remove one Entity", "[ecs][fast]") {
    constexpr EntityId entityA = 1u;
    ComponentPool<TestClass> pool;
    pool.Add(entityA, TestClass{.testValue = 1});
    pool.Remove(entityA);
    REQUIRE(pool.Count() == 0);
    REQUIRE_FALSE(pool.Contains(entityA));
}

TEST_CASE("ComponentPool::Remove - Remove entity in the middle", "[ecs][fast]") {
    constexpr EntityId entityA = 1u;
    constexpr EntityId entityB = 2u;
    constexpr EntityId entityC = 3u;

    ComponentPool<TestClass> pool;
    pool.Add(entityA, TestClass{.testValue = 1});
    pool.Add(entityB, TestClass{.testValue = 2});
    pool.Add(entityC, TestClass{.testValue = 3});

    REQUIRE(pool.Contains(entityA));
    REQUIRE(pool.Contains(entityB));
    REQUIRE(pool.Contains(entityC));

    pool.Remove(entityB);

    REQUIRE(pool.Count() == 2);
    REQUIRE_FALSE(pool.Contains(entityB));
    REQUIRE(pool.Contains(entityC));
    REQUIRE(pool.Contains(entityA));

    REQUIRE(pool.Get(entityC)->testValue == 3);
    REQUIRE(pool.Get(entityA)->testValue == 1);
    REQUIRE_THROWS(pool.Get(entityB));
}

TEST_CASE("ComponentPool::ForEach - Iterate over each component and increase it by one", "[ecs][fast]") {
    constexpr EntityId entityA = 1u;
    constexpr EntityId entityB = 2u;
    constexpr EntityId entityC = 3u;

    ComponentPool<TestClass> pool;
    pool.Add(entityA, TestClass{.testValue = 1});
    pool.Add(entityB, TestClass{.testValue = 2});
    pool.Add(entityC, TestClass{.testValue = 3});

    pool.ForEach([](EntityId entity, TestClass &component) {
        component.testValue++;
    });

    REQUIRE(pool.Get(entityA)->testValue == 2);
    REQUIRE(pool.Get(entityB)->testValue == 3);
    REQUIRE(pool.Get(entityC)->testValue == 4);
}
