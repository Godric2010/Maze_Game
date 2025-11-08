#include "SceneManager.hpp"

namespace Engine::Core {
    SceneManager::SceneManager(IApplication &app,
                               Ecs::SystemManager &system_manager, InputManager &input_manager,
                               const float screen_width,
                               const float screen_height) {
        m_active_world = std::make_unique<Ecs::World>();
        m_active_game_world = std::make_unique<GameWorld>(m_active_world.get(), &input_manager);
        m_context.emplace(SceneContext{
            .app = app,
            .scene_manager = *this,
            .world = *m_active_world,
            .game_world = *m_active_game_world,
            .system_manager = system_manager,
            .input = input_manager,
            .screen_width = screen_width,
            .screen_height = screen_height,
        });

        m_registry = std::make_unique<SceneRegistry>();
    }

    SceneManager::~SceneManager() {
        if (m_current_scene) {
            m_current_scene->OnExit();
            m_current_scene.reset();
        }
        m_registry.reset();
    }

    void SceneManager::RegisterScene(const std::string &name, const SceneFactory &scene_factory) const {
        m_registry->RegisterScene(name, scene_factory);
    }

    void SceneManager::LoadScene(const std::string &name, const SceneArgs &args) {
        if (auto scene = m_registry->CreateScene(name, args)) {
            m_pending_scene = std::move(scene);
            return;
        }

        throw std::runtime_error("Failed to load scene: " + name);
    }

    void SceneManager::Update(const float delta_time) {
        if (m_pending_scene) {
            ApplyTransitionToPendingScene();
        }
        if (m_current_scene) {
            m_current_scene->Update(delta_time);
        }
    }

    void SceneManager::ApplyTransitionToPendingScene() {
        if (m_current_scene) {
            m_current_scene->OnExit();
            m_current_scene->UnloadScene();
            m_current_scene.reset();
        }

        m_active_game_world.reset();
        m_active_world.reset();

        m_active_world = std::make_unique<Ecs::World>();
        m_active_game_world = std::make_unique<GameWorld>(m_active_world.get(), &m_context->input);

        const auto old_context = m_context.value();
        auto new_context = SceneContext{\
            .app = old_context.app,
            .scene_manager = *this,
            .world = *m_active_world,
            .game_world = *m_active_game_world,
            .system_manager = old_context.system_manager,
            .input = old_context.input,
            .screen_width = old_context.screen_width,
            .screen_height = old_context.screen_height
        };
        new_context.system_manager.RegisterSystems(m_active_world.get(), m_active_game_world.get());
        m_context.emplace(new_context);

        m_current_scene = std::move(m_pending_scene);
        m_current_scene->Initialize(*m_context);
        m_current_scene->OnStart();
    }
} // namespace
