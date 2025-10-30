#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../src/EntityManager.hpp"

using namespace Engine::Ecs;

struct EntitymanagerFixture {
    EntityManager entity_manager;
};

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::GenerateEntity: returns non-null & is alive", "[ecs][fast]") {
    const auto entity = entity_manager.ReserveEntity("T1");
    INFO("Entity: " << entity);
    REQUIRE(entity_manager.IsEntityPending(entity));
    REQUIRE_FALSE(entity_manager.IsEntityAlive(entity));
    REQUIRE(entity_manager.GetEntityByName("T1") == entity);

    entity_manager.CommitEntity(entity);
    REQUIRE_FALSE(entity_manager.IsEntityPending(entity));
    REQUIRE(entity_manager.IsEntityAlive(entity));

    entity_manager.DestroyEntity(entity);
    INFO("Entity: " << entity);
    REQUIRE_FALSE(entity_manager.IsEntityAlive(entity));
    REQUIRE_FALSE(entity_manager.IsEntityPending(entity));
    REQUIRE(entity_manager.GetEntityByName("T1") == INVALID_ENTITY_ID);
}

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::GenerateEntity: no two entities are equal", "[ecs][fast]") {
    const auto entity1 = entity_manager.ReserveEntity("E1");
    const auto entity2 = entity_manager.ReserveEntity("E2");
    INFO("Entity1: " << entity1);
    INFO("Entity_2: " << entity2);
    REQUIRE(entity1 != entity2);
    REQUIRE(entity_manager.GetEntityByName("E1") == entity1);
    REQUIRE(entity_manager.GetEntityByName("E2") == entity2);
}

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::DestroyEntity: invalidates old handle", "[ecs][fast]") {
    const auto entity = entity_manager.ReserveEntity("E1");
    entity_manager.CommitEntity(entity);
    REQUIRE(entity_manager.IsEntityAlive(entity));

    entity_manager.DestroyEntity(entity);
    INFO("Entity: " << entity);
    REQUIRE_FALSE(entity_manager.IsEntityAlive(entity));
    REQUIRE_FALSE(entity_manager.IsEntityPending(entity));
    REQUIRE(entity_manager.GetEntityByName("E1") == INVALID_ENTITY_ID);
}

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::DestroyEntity: recycles index but bumps generation",
                 "[ecs][fast]") {
    const auto entity_a = entity_manager.ReserveEntity("E1");
    entity_manager.CommitEntity(entity_a);
    const auto index_a = GetEntityIndex(entity_a);
    const auto gen_a = GetEntityGenration(entity_a);

    entity_manager.DestroyEntity(entity_a);
    REQUIRE_FALSE(entity_manager.IsEntityAlive(entity_a));

    const auto entity_b = entity_manager.ReserveEntity("E2");
    entity_manager.CommitEntity(entity_b);
    REQUIRE(GetEntityIndex(entity_b) == index_a);
    REQUIRE(GetEntityGenration(entity_b) != gen_a);
    REQUIRE(entity_manager.IsEntityAlive(entity_b));
}

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::IsEntityAlive: returns false for invalid/null", "[ecs][fast]") {
    REQUIRE_FALSE(entity_manager.IsEntityAlive(0));
}

TEST_CASE_METHOD(EntitymanagerFixture, "EntityManager::IsEntityPending: returns false for invalid/null",
                 "[ecs][fast]") {
    REQUIRE_FALSE(entity_manager.IsEntityPending(0));
}
