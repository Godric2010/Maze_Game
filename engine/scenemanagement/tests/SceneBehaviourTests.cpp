#include <unordered_set>
#include <catch2/matchers/catch_matchers.hpp>
#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "MockedClasses.hpp"
#include "../src/SceneManager.hpp"

using namespace Engine::SceneManagement;
static uint8_t last_scene_instance_id = 0;

struct SceneProbe
{
    int instance_id;
    bool scene_is_active = false;
    bool scene_is_destroyed = false;
    int entities_added = 0;
};

void RunSingleUpdatePhase(SceneManager* scene_manager)
{
    scene_manager->PreFixed(1.0f);
    scene_manager->FixedUpdate(1.0f);
    scene_manager->Update(1.0f);
}

class BehaviourTestScene : public IScene
{
private:
    std::unordered_set<Engine::Ecs::EntityId> m_entities;
    std::shared_ptr<SceneProbe> m_scene_probe;

public:
    explicit BehaviourTestScene(const std::shared_ptr<SceneProbe>& scene_probe)
    {
        last_scene_instance_id++;
        m_scene_probe = scene_probe;
        m_scene_probe->instance_id = last_scene_instance_id;
    }

    ~BehaviourTestScene() override
    {
        if (m_scene_probe)
        {
            m_scene_probe->scene_is_destroyed = true;
        }
    }

    void OnStart() override
    {
        m_scene_probe->scene_is_active = true;
    }

    void OnExit() override
    {
        m_scene_probe->scene_is_active = false;
    }

    void AddEntity(const std::string& name)
    {
        const auto entity_id = World().CreateEntity(name);
        m_entities.emplace(entity_id);
    }

    void TestApplication() const
    {
        // ReSharper disable once CppNoDiscardExpression
        Application();
    }

    void TestInput() const
    {
        Input();
    }

    void TestSceneWorld() const
    {
        World();
    }

    void TestSceneManager() const
    {
        SceneManager();
    }
};


TEST_CASE("SceneBehaviourTests - Scene Loading - World instance gets rebuild clean")
{
    FakeApplication app{};
    FakeInput input{};
    FakeSystemManager system_manager{};
    FakeRenderer renderer{};
    last_scene_instance_id = 0;

    auto scene_manager = SceneManager(
        app,
        system_manager,
        input,
        renderer,
        1280,
        720
    );

    auto probe1 = std::make_shared<SceneProbe>();
    auto probe2 = std::make_shared<SceneProbe>();
    const SceneFactory f1 = [probe1](const SceneArgs&)
    {
        return std::make_unique<BehaviourTestScene>(probe1);
    };

    const SceneFactory f2 = [probe2](const SceneArgs&)
    {
        return std::make_unique<BehaviourTestScene>(probe2);
    };

    scene_manager.RegisterScene("Scene01", f1);
    scene_manager.RegisterScene("Scene02", f2);
    scene_manager.LoadScene("Scene01", SceneArgs{});
    RunSingleUpdatePhase(&scene_manager);

    // Adding entities to an active scene
    REQUIRE(probe1->scene_is_active);
    RunSingleUpdatePhase(&scene_manager);

    // Switch active scene
    scene_manager.LoadScene("Scene02", SceneArgs{});
    RunSingleUpdatePhase(&scene_manager);

    REQUIRE_FALSE(probe1->scene_is_active);
    REQUIRE(probe1->scene_is_destroyed);
    REQUIRE(probe2->scene_is_active);
}

TEST_CASE("ISceneTests - Uninitialized Scene throws exceptions")
{
    const auto test_scene = std::make_unique<BehaviourTestScene>(std::make_shared<SceneProbe>());
    REQUIRE_THROWS_AS(test_scene->TestApplication(), SceneRuntimeException);
    REQUIRE_THROWS_AS(test_scene->TestInput(), SceneRuntimeException);
    REQUIRE_THROWS_AS(test_scene->TestSceneWorld(), SceneRuntimeException);
    REQUIRE_THROWS_AS(test_scene->TestSceneManager(), SceneRuntimeException);
}

TEST_CASE("ISceneTests - Initialized Scene has context")
{
    FakeApplication app{};
    FakeInput input{};
    FakeSystemManager system_manager{};
    FakeRenderer renderer{};
    auto scene_manager = SceneManager(
        app,
        system_manager,
        input,
        renderer,
        1280,
        720
    );
    const auto test_scene = std::make_unique<BehaviourTestScene>(std::make_shared<SceneProbe>());
    const auto world = std::make_unique<Engine::Ecs::World>();
    const auto world_adapter = std::make_unique<SceneWorld>(*world);
    const SceneContext ctx{
        .app = app,
        .scene_manager = scene_manager,
        .world = *world,
        .game_world = *world_adapter,
        .system_manager = system_manager,
        .input = input,
        .renderer = renderer,
        .screen_width = 1280,
        .screen_height = 720,
    };
    test_scene->Initialize(ctx);

    REQUIRE_NOTHROW(test_scene->TestApplication());
    REQUIRE_NOTHROW(test_scene->TestInput());
    REQUIRE_NOTHROW(test_scene->TestSceneWorld());
    REQUIRE_NOTHROW(test_scene->TestSceneManager());
}
