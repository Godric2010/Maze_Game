#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "MockedClasses.hpp"
#include "../src/SceneManager.hpp"

class DemoScene : public Engine::SceneManagement::IScene {
public:
    bool scene_is_active = false;
    float scene_width = 0;
    float scene_height = 0;

    void OnStart() override {
        scene_is_active = true;
        scene_width = Screen().width;
        scene_height = Screen().height;
    }

    void OnExit() override {
        scene_is_active = false;
    }
};


static Engine::SceneManagement::SceneManager BuildSceneManager() {
    FakeApplication app{};
    FakeInput input{};
    FakeSystemManager system_manager{};

    return Engine::SceneManagement::SceneManager(
            app,
            system_manager,
            input,
            1280,
            720
            );
}


TEST_CASE("SceneManagerTests - Register single scene") {
    auto scene_manager = BuildSceneManager();

    const DemoScene* demo_scene = nullptr;
    const Engine::SceneManagement::SceneFactory scene_factory = [&](const Engine::SceneManagement::SceneArgs&) {
        auto scene = std::make_unique<DemoScene>();
        demo_scene = scene.get();
        return scene;
    };
    scene_manager.RegisterScene("MyScene", scene_factory);
    scene_manager.LoadScene("MyScene", Engine::SceneManagement::SceneArgs{});
    REQUIRE(demo_scene != nullptr);
}

TEST_CASE("SceneManagerTests - Register scene with same identifier twice") {
    const auto scene_manager = BuildSceneManager();
    const Engine::SceneManagement::SceneFactory scene_factory = [&](const Engine::SceneManagement::SceneArgs&) {
        auto scene = std::make_unique<DemoScene>();
        return scene;
    };
    scene_manager.RegisterScene("MyScene", scene_factory);
    REQUIRE_THROWS(scene_manager.RegisterScene("MyScene", scene_factory));
}

TEST_CASE("SceneManagerTests - Register same scene with two identifiers") {
    auto scene_manager = BuildSceneManager();
    std::vector<DemoScene*> scenes;
    const Engine::SceneManagement::SceneFactory scene_factory = [&](const Engine::SceneManagement::SceneArgs&) {
        auto scene = std::make_unique<DemoScene>();
        scenes.push_back(scene.get());
        return scene;
    };
    scene_manager.RegisterScene("MyScene01", scene_factory);
    scene_manager.RegisterScene("MyScene02", scene_factory);

    scene_manager.LoadScene("MyScene01", Engine::SceneManagement::SceneArgs{});
    REQUIRE(scenes.size() == 1);
    scene_manager.LoadScene("MyScene02", Engine::SceneManagement::SceneArgs{});
    REQUIRE(scenes.size() == 2);
    REQUIRE(scenes[0] != scenes[1]);
}

TEST_CASE("SceneManagerTests - Load Scene with unknown identifier") {
    auto scene_manager = BuildSceneManager();
    REQUIRE_THROWS(scene_manager.LoadScene("MyScene", Engine::SceneManagement::SceneArgs{}));
}

TEST_CASE("SceneManagerTests - Switch Scene") {
    FakeApplication app{};
    FakeInput input{};
    FakeSystemManager system_manager{};

    constexpr float screen_width = 1280;
    constexpr float screen_height = 720;
    auto scene_manager = Engine::SceneManagement::SceneManager(
            app,
            system_manager,
            input,
            screen_width,
            screen_height
            );

    std::vector<DemoScene*> scenes;
    const Engine::SceneManagement::SceneFactory scene_factory = [&](const Engine::SceneManagement::SceneArgs&) {
        auto scene = std::make_unique<DemoScene>();
        scenes.push_back(scene.get());
        return scene;
    };
    scene_manager.RegisterScene("MyScene01", scene_factory);
    scene_manager.RegisterScene("MyScene02", scene_factory);

    scene_manager.LoadScene("MyScene01", Engine::SceneManagement::SceneArgs{});
    scene_manager.Update(1.0f);
    REQUIRE(scenes.size() == 1);
    REQUIRE(scenes[0]->scene_is_active);
    REQUIRE(scenes[0]->scene_width == screen_width);
    REQUIRE(scenes[0]->scene_height == screen_height);

    scene_manager.LoadScene("MyScene02", Engine::SceneManagement::SceneArgs{});
    scene_manager.Update(1.0f);
    REQUIRE(scenes.size() == 2);
    REQUIRE_FALSE(scenes[0]->scene_is_active);
    REQUIRE(scenes[1]->scene_is_active);
}

TEST_CASE("SceneManagerTests - Switch Scene but loaded two scenes before updating") {
    FakeApplication app{};
    FakeInput input{};
    FakeSystemManager system_manager{};

    auto scene_manager = Engine::SceneManagement::SceneManager(
            app,
            system_manager,
            input,
            1280,
            720
            );

    std::vector<DemoScene*> scenes;
    const Engine::SceneManagement::SceneFactory scene_factory = [&](const Engine::SceneManagement::SceneArgs&) {
        auto scene = std::make_unique<DemoScene>();
        scenes.push_back(scene.get());
        return scene;
    };
    scene_manager.RegisterScene("MyScene01", scene_factory);
    scene_manager.RegisterScene("MyScene02", scene_factory);

    scene_manager.LoadScene("MyScene01", Engine::SceneManagement::SceneArgs{});
    REQUIRE(scenes.size() == 1);
    REQUIRE_FALSE(scenes[0]->scene_is_active);

    scene_manager.LoadScene("MyScene02", Engine::SceneManagement::SceneArgs{});
    REQUIRE(scenes.size() == 2);
    REQUIRE_FALSE(scenes[1]->scene_is_active);

    scene_manager.Update(1.0f);
    REQUIRE_FALSE(scenes[0]->scene_is_active);
    REQUIRE(scenes[1]->scene_is_active);
}

TEST_CASE("SceneManagerTests - Update but no scene is loaded") {
    FakeApplication app{};
    FakeInput input{};
    FakeSystemManager system_manager{};

    auto scene_manager = Engine::SceneManagement::SceneManager(
            app,
            system_manager,
            input,
            1280,
            720
            );

    std::vector<DemoScene*> scenes;
    const Engine::SceneManagement::SceneFactory scene_factory = [&](const Engine::SceneManagement::SceneArgs&) {
        auto scene = std::make_unique<DemoScene>();
        scenes.push_back(scene.get());
        return scene;
    };
    scene_manager.RegisterScene("MyScene01", scene_factory);

    scene_manager.Update(1.0f);
    REQUIRE(scenes.empty());
}
