#pragma once
#include <optional>

#include "SceneRegistry.hpp"
#include "../include/IScene.hpp"
#include "Scene/ISceneManager.hpp"
#include "Scene/SceneArgs.hpp"

namespace Engine::SceneManagement {
    class SceneManager : public ISceneManager {
    public:
        explicit SceneManager(IApplication &app,
                              Ecs::SystemManager &system_manager, Input::IInput &input_manager,
                              float screen_width, float screen_height);

        ~SceneManager() override;

        void RegisterScene(const std::string &name, const SceneFactory &scene_factory) const;

        void LoadScene(const std::string &name, const SceneArgs &args) override;

        void Update(float delta_time);

    private:
        std::optional<SceneContext> m_context;
        std::unique_ptr<IScene> m_current_scene;
        std::unique_ptr<IScene> m_pending_scene;

        std::unique_ptr<Ecs::World> m_active_world;
        std::unique_ptr<SceneWorld> m_world_adapter;

        std::unique_ptr<SceneRegistry> m_registry;

        void ApplyTransitionToPendingScene();
    };
} // namespace
