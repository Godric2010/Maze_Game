#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "SceneWorld.hpp"
using namespace Engine;
using namespace Engine::SceneManagement;

TEST_CASE("SceneWorldTests - Test Scene World Facade") {
    auto* world = new Ecs::World();
    const auto scene_world = std::make_unique<SceneWorld>(*world);

    const auto entity_id = scene_world->CreateEntity("TestEntity");
    REQUIRE(world->GetEntityByName("TestEntity") == entity_id);
    REQUIRE(scene_world->GetEntityByName("TestEntity") == entity_id);

    scene_world->AddComponent<int>(entity_id, 42);
    REQUIRE(world->GetComponent<int>(entity_id) == scene_world->GetComponent<int>(entity_id));

    world->ApplyEngineEvents();

    auto test_components = scene_world->GetComponentsOfType<int>();
    auto world_components = world->GetComponentsOfType<int>();
    REQUIRE(world_components.size() == test_components.size());
    REQUIRE(world_components[0].first == test_components[0].first);
    REQUIRE(world_components[0].second == test_components[0].second);

    scene_world->RemoveComponent<int>(entity_id);
    world->ApplyEngineEvents();
    REQUIRE(world->GetComponent<int>(entity_id) == nullptr);

    scene_world->DestroyEntity(entity_id);
    world->ApplyEngineEvents();

    auto invalid_entity = world->GetEntityByName("TestEntity");
    REQUIRE(invalid_entity == Ecs::INVALID_ENTITY_ID);
    delete world;
}
