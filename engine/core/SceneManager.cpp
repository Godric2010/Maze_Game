#include "SceneManager.hpp"

namespace Engine::Core {
    SceneManager::SceneManager(const SceneContext &scene_context) {
        m_context = &scene_context;
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
            m_current_scene.reset();
        }

        m_current_scene = std::move(m_pending_scene);
        m_current_scene->Initialize(*m_context);
        m_current_scene->OnStart();
    }
} // namespace
