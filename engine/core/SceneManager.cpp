#include "SceneManager.hpp"

namespace Engine::Core {
    SceneManager::SceneManager(const SceneContext &scene_context) {
        m_context = &scene_context;
    }

    SceneManager::~SceneManager() {
        if (m_current_scene) {
            m_current_scene->OnExit();
            m_current_scene.reset();
        }
    }

    void SceneManager::LoadScene(std::unique_ptr<IScene> scene) {
        m_pending_scene = std::move(scene);
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
