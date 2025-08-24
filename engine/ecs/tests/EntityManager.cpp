#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../src/EntityManager.hpp"

using namespace Engine::Ecs;

struct EntitymanagerFixture {
    EntityManager entityManager;
};

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::GenerateEntity: returns non-null & is alive", "[ecs][fast]") {
    const auto entity = entityManager.GenerateEntity();
    INFO("Entity: " << entity);
    REQUIRE(entityManager.IsEntityAlive(entity));
    entityManager.DestroyEntity(entity);
    INFO("Entity: " << entity);
    REQUIRE_FALSE(entityManager.IsEntityAlive(entity));
}

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::GenerateEntity: no two entities are equal", "[ecs][fast]") {
    const auto entity1 = entityManager.GenerateEntity();
    const auto entity2 = entityManager.GenerateEntity();
    INFO("Entity1: " << entity1);
    INFO("Entity_2: " << entity2);
    REQUIRE(entity1 != entity2);
}

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::DestroyEntity: invalidates old handle", "[ecs][fast]") {
    const auto entity = entityManager.GenerateEntity();
    REQUIRE(entityManager.IsEntityAlive(entity));
    entityManager.DestroyEntity(entity);
    INFO("Entity: " << entity);
    REQUIRE_FALSE(entityManager.IsEntityAlive(entity));
}

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::DestroyEntity: recycles index but bumps generation", "[ecs][fast]") {
    const auto entityA = entityManager.GenerateEntity();
    const auto indexA = GetEntityIndex(entityA);
    const auto genA = GetEntityGenration(entityA);

    entityManager.DestroyEntity(entityA);
    REQUIRE_FALSE(entityManager.IsEntityAlive(entityA));

    const auto entityB = entityManager.GenerateEntity();
    REQUIRE(GetEntityIndex(entityB) == indexA);
    REQUIRE(GetEntityGenration(entityB) != genA);
    REQUIRE(entityManager.IsEntityAlive(entityB));
}

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::IsEntityAlive: returns false for invalid/null", "[ecs][fast]") {
    REQUIRE_FALSE(entityManager.IsEntityAlive(0));
}
