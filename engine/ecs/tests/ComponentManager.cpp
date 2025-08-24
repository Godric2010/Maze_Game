#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../src/ComponentManager.hpp"

using namespace Engine::Ecs;

struct ComponentManagerFixture {
    ComponentManager m_componentManager;
};

struct TestClassA {
    uint32_t testVal;
};

struct TestClassB {
    int testVal;
};

struct TestClassC {
    float testVal;
};

TEST_CASE_METHOD(ComponentManagerFixture,
                 "ComponentManager::AddComponent - Add Components to manager with various entities", "[ecs][fast]") {
    EntityId entityA = 1u;
    EntityId entityB = 2u;
    EntityId entityC = 3u;


    m_componentManager.AddComponent(entityA, TestClassA{.testVal = 1});
    REQUIRE(m_componentManager.HasComponent<TestClassA>(entityA));

    m_componentManager.AddComponent(entityB, TestClassB{.testVal = -2});
    REQUIRE(m_componentManager.HasComponent<TestClassB>(entityB));
    REQUIRE_FALSE(m_componentManager.HasComponent<TestClassA>(entityB));
    REQUIRE_FALSE(m_componentManager.HasComponent<TestClassB>(entityA));

    m_componentManager.AddComponent(entityC, TestClassC{.testVal = 3.3f});
    REQUIRE(m_componentManager.HasComponent<TestClassC>(entityC));
    REQUIRE(m_componentManager.GetComponent<TestClassC>(entityC).testVal == 3.3f);
}

TEST_CASE_METHOD(ComponentManagerFixture, "ComponentManger::AddComponent - Add multiple components to one entity", "[ecs][fast]") {
    EntityId entityA = 1u;

    m_componentManager.AddComponent(entityA, TestClassA{.testVal = 1});
    m_componentManager.AddComponent(entityA, TestClassB{.testVal = -2});
    m_componentManager.AddComponent(entityA, TestClassC{.testVal = 3.3f});

    REQUIRE(m_componentManager.HasComponent<TestClassA>(entityA));
    REQUIRE(m_componentManager.HasComponent<TestClassB>(entityA));
    REQUIRE(m_componentManager.HasComponent<TestClassC>(entityA));

    REQUIRE(m_componentManager.GetComponent<TestClassA>(entityA).testVal == 1);
    REQUIRE(m_componentManager.GetComponent<TestClassB>(entityA).testVal == -2);
    REQUIRE(m_componentManager.GetComponent<TestClassC>(entityA).testVal == 3.3f);
}

TEST_CASE_METHOD(ComponentManagerFixture, "ComponentManger::RemoveComponent - Remove Component", "[ecs][fast]") {
    EntityId entityA = 1u;
    EntityId entityB = 2u;

    m_componentManager.AddComponent(entityA, TestClassA{.testVal = 1});
    m_componentManager.AddComponent(entityB, TestClassA{.testVal = 2});

    m_componentManager.RemoveComponent<TestClassA>(entityA);
    REQUIRE_FALSE(m_componentManager.HasComponent<TestClassA>(entityA));
    REQUIRE(m_componentManager.GetComponent<TestClassA>(entityB).testVal == 2);
}

TEST_CASE_METHOD(ComponentManagerFixture,
                 "ComponentManager::GetEntitiesWithComponent - Get all entities that have one component",
                 "[ecs][fast]") {
    EntityId entityA = 1u;
    EntityId entityB = 2u;
    EntityId entityC = 3u;

    m_componentManager.AddComponent(entityA, TestClassA{.testVal = 1});
    m_componentManager.AddComponent(entityB, TestClassA{.testVal = 42});
    m_componentManager.AddComponent(entityC, TestClassC{.testVal = 43.33f});

    const auto entities = m_componentManager.GetEntitiesWithComponent<TestClassA>();
    REQUIRE(entities.size() == 2);
    REQUIRE(entities[0] == entityA);
    REQUIRE(entities[1] == entityB);
}

TEST_CASE_METHOD(ComponentManagerFixture, "ComponentManger::OnEntityDestroy - All components that have the entity are removed", "[ecs][fast]") {
    EntityId entityA = 1u;
    EntityId entityB = 2u;

    m_componentManager.AddComponent(entityA, TestClassA{.testVal = 1});
    m_componentManager.AddComponent(entityA, TestClassB{.testVal = -2});
    m_componentManager.AddComponent(entityA, TestClassC{.testVal = 3.3f});

    m_componentManager.AddComponent(entityB, TestClassA{.testVal = 4});
    m_componentManager.AddComponent(entityB, TestClassB{.testVal = -3});
    m_componentManager.AddComponent(entityB, TestClassC{.testVal = 4.3f});
    REQUIRE(m_componentManager.HasComponent<TestClassA>(entityA));
    REQUIRE(m_componentManager.HasComponent<TestClassB>(entityB));

    m_componentManager.OnDestroyEntity(entityA);

    auto resultA = m_componentManager.GetEntitiesWithComponent<TestClassA>();
    auto resultB = m_componentManager.GetEntitiesWithComponent<TestClassB>();
    auto resultC = m_componentManager.GetEntitiesWithComponent<TestClassC>();

    REQUIRE(resultA.size() == 1);
    REQUIRE(resultA[0] == entityB);

    REQUIRE(resultB.size() == 1);
    REQUIRE(resultB[0] == entityB);

    REQUIRE(resultC.size() == 1);
    REQUIRE(resultC[0] == entityB);
}
