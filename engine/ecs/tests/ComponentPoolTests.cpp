#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../src/ComponentPool.hpp"
#include "../src/Entity.hpp"

using namespace Engine::Ecs;

class TestClass {
public:
    uint32_t test_value;
};

TEST_CASE("ComponentPool::Add - Add new entity and component", "[ecs][fast]") {
    constexpr EntityId entity_a = 1u;
    constexpr EntityId entity_b = 2u;
    auto pool = ComponentPool<TestClass>(0);

    pool.Add(entity_a, TestClass{.test_value = 1});
    pool.Add(entity_b, TestClass{.test_value = 2});

    REQUIRE(pool.Count() == 2);
    REQUIRE(pool.Get(entity_a)->test_value == 1);
    REQUIRE(pool.Get(entity_b)->test_value == 2);
}

TEST_CASE("ComponentPool::Add - Add entity, that already exists", "[ecs][fast]") {
    constexpr EntityId entity_a = 1u;
    auto pool = ComponentPool<TestClass>(0);
    pool.Add(entity_a, TestClass{.test_value = 1});

    pool.Add(entity_a, TestClass{.test_value = 2});
    REQUIRE(pool.Count() == 1);
    REQUIRE(pool.Get(entity_a)->test_value == 1);
}

TEST_CASE("ComponentPool::Add - Add invalid Entity", "[ecs][fast]") {
    constexpr EntityId entity_a = 0u;
    auto pool = ComponentPool<TestClass>(0);
    REQUIRE_THROWS(pool.Add(entity_a, TestClass{.test_value = 1}));
}

TEST_CASE("ComponentPool::Remove - Remove one Entity", "[ecs][fast]") {
    constexpr EntityId entity_a = 1u;
    auto pool = ComponentPool<TestClass>(0);
    pool.Add(entity_a, TestClass{.test_value = 1});
    pool.Remove(entity_a);
    REQUIRE(pool.Count() == 0);
    REQUIRE_FALSE(pool.Contains(entity_a));
}

TEST_CASE("ComponentPool::Remove - Remove entity in the middle", "[ecs][fast]") {
    constexpr EntityId entity_a = 1u;
    constexpr EntityId entity_b = 2u;
    constexpr EntityId entity_c = 3u;

    ComponentPool<TestClass> pool = ComponentPool<TestClass>(0);
    pool.Add(entity_a, TestClass{.test_value = 1});
    pool.Add(entity_b, TestClass{.test_value = 2});
    pool.Add(entity_c, TestClass{.test_value = 3});

    REQUIRE(pool.Contains(entity_a));
    REQUIRE(pool.Contains(entity_b));
    REQUIRE(pool.Contains(entity_c));

    pool.Remove(entity_b);

    REQUIRE(pool.Count() == 2);
    REQUIRE_FALSE(pool.Contains(entity_b));
    REQUIRE(pool.Contains(entity_c));
    REQUIRE(pool.Contains(entity_a));

    REQUIRE(pool.Get(entity_c)->test_value == 3);
    REQUIRE(pool.Get(entity_a)->test_value == 1);
    REQUIRE_THROWS(pool.Get(entity_b));
}

TEST_CASE("ComponentPool::ForEach - Iterate over each component and increase it by one", "[ecs][fast]") {
    constexpr EntityId entity_a = 1u;
    constexpr EntityId entity_b = 2u;
    constexpr EntityId entity_c = 3u;

    auto pool = ComponentPool<TestClass>(0);
    pool.Add(entity_a, TestClass{.test_value = 1});
    pool.Add(entity_b, TestClass{.test_value = 2});
    pool.Add(entity_c, TestClass{.test_value = 3});

    pool.ForEach([](EntityId entity, TestClass &component) {
        component.test_value++;
    });

    REQUIRE(pool.Get(entity_a)->test_value == 2);
    REQUIRE(pool.Get(entity_b)->test_value == 3);
    REQUIRE(pool.Get(entity_c)->test_value == 4);
}
