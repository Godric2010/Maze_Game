#include "../src/ComponentManager.hpp"
#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch.hpp>
#endif

#include "../src/EntityManager.hpp"

using namespace Engine::Ecs;

struct EntitymanagerFixture {
    EntityManager entityManager;
};

TEST_CASE_METHOD(EntitymanagerFixture, "GenerateEntity: returns non-null & is alive", "[ecs][fast]") {
    const auto entity = entityManager.GenerateEntity();
    INFO("Entity: " << entity);
    REQUIRE(entityManager.IsEntityAlive(entity));
    entityManager.DestroyEntity(entity);
    INFO("Entity: " << entity);
    REQUIRE_FALSE(entityManager.IsEntityAlive(entity));
}
