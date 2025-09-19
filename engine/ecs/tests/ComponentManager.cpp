#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../src/ComponentManager.hpp"

using namespace Engine::Ecs;

struct ComponentManagerFixture {
    ComponentManager component_manager;
};

struct TestClassA {
    uint32_t test_val;
};

struct TestClassB {
    int test_val;
};

struct TestClassC {
    float test_val;
};

TEST_CASE_METHOD(ComponentManagerFixture,
                 "ComponentManager::AddComponent - Add Components to manager with various entities", "[ecs][fast]") {
    EntityId entity_a = 1u;
    EntityId entity_b = 2u;
    EntityId entity_c = 3u;


    component_manager.AddComponent(entity_a, TestClassA{.test_val = 1});
    REQUIRE(component_manager.HasComponent<TestClassA>(entity_a));

    component_manager.AddComponent(entity_b, TestClassB{.test_val = -2});
    REQUIRE(component_manager.HasComponent<TestClassB>(entity_b));
    REQUIRE_FALSE(component_manager.HasComponent<TestClassA>(entity_b));
    REQUIRE_FALSE(component_manager.HasComponent<TestClassB>(entity_a));

    component_manager.AddComponent(entity_c, TestClassC{.test_val = 3.3f});
    REQUIRE(component_manager.HasComponent<TestClassC>(entity_c));
    REQUIRE(component_manager.GetComponent<TestClassC>(entity_c).test_val == 3.3f);
}

TEST_CASE_METHOD(ComponentManagerFixture, "ComponentManger::AddComponent - Add multiple components to one entity", "[ecs][fast]") {
    EntityId entity_a = 1u;

    component_manager.AddComponent(entity_a, TestClassA{.test_val = 1});
    component_manager.AddComponent(entity_a, TestClassB{.test_val = -2});
    component_manager.AddComponent(entity_a, TestClassC{.test_val = 3.3f});

    REQUIRE(component_manager.HasComponent<TestClassA>(entity_a));
    REQUIRE(component_manager.HasComponent<TestClassB>(entity_a));
    REQUIRE(component_manager.HasComponent<TestClassC>(entity_a));

    REQUIRE(component_manager.GetComponent<TestClassA>(entity_a).test_val == 1);
    REQUIRE(component_manager.GetComponent<TestClassB>(entity_a).test_val == -2);
    REQUIRE(component_manager.GetComponent<TestClassC>(entity_a).test_val == 3.3f);
}

TEST_CASE_METHOD(ComponentManagerFixture, "ComponentManger::RemoveComponent - Remove Component", "[ecs][fast]") {
    constexpr EntityId entity_a = 1u;
    constexpr EntityId entity_b = 2u;

    component_manager.AddComponent(entity_a, TestClassA{.test_val = 1});
    component_manager.AddComponent(entity_b, TestClassA{.test_val = 2});

    component_manager.RemoveComponent<TestClassA>(entity_a);
    REQUIRE_FALSE(component_manager.HasComponent<TestClassA>(entity_a));
    REQUIRE(component_manager.GetComponent<TestClassA>(entity_b).test_val == 2);
}

TEST_CASE_METHOD(ComponentManagerFixture,
                 "ComponentManager::GetEntitiesWithComponent - Get all entities that have one component",
                 "[ecs][fast]") {
    constexpr EntityId entity_a = 1u;
    constexpr EntityId entity_b = 2u;
    constexpr EntityId entity_c = 3u;

    component_manager.AddComponent(entity_a, TestClassA{.test_val = 1});
    component_manager.AddComponent(entity_b, TestClassA{.test_val = 42});
    component_manager.AddComponent(entity_c, TestClassC{.test_val = 43.33f});

    const auto entities = component_manager.GetEntitiesWithComponent<TestClassA>();
    REQUIRE(entities.size() == 2);
    REQUIRE(entities[0] == entity_a);
    REQUIRE(entities[1] == entity_b);
}

TEST_CASE_METHOD(ComponentManagerFixture, "ComponentManger::OnEntityDestroy - All components that have the entity are removed", "[ecs][fast]") {
    EntityId entity_a = 1u;
    EntityId entity_b = 2u;

    component_manager.AddComponent(entity_a, TestClassA{.test_val = 1});
    component_manager.AddComponent(entity_a, TestClassB{.test_val = -2});
    component_manager.AddComponent(entity_a, TestClassC{.test_val = 3.3f});

    component_manager.AddComponent(entity_b, TestClassA{.test_val = 4});
    component_manager.AddComponent(entity_b, TestClassB{.test_val = -3});
    component_manager.AddComponent(entity_b, TestClassC{.test_val = 4.3f});
    REQUIRE(component_manager.HasComponent<TestClassA>(entity_a));
    REQUIRE(component_manager.HasComponent<TestClassB>(entity_b));

    component_manager.OnDestroyEntity(entity_a);

    auto result_a = component_manager.GetEntitiesWithComponent<TestClassA>();
    auto result_b = component_manager.GetEntitiesWithComponent<TestClassB>();
    auto result_c = component_manager.GetEntitiesWithComponent<TestClassC>();

    REQUIRE(result_a.size() == 1);
    REQUIRE(result_a[0] == entity_b);

    REQUIRE(result_b.size() == 1);
    REQUIRE(result_b[0] == entity_b);

    REQUIRE(result_c.size() == 1);
    REQUIRE(result_c[0] == entity_b);
}
