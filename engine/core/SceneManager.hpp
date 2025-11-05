#pragma once
#include <unordered_map>
#include "IScene.hpp"

namespace Engine::Core {
    class SceneManager {
    public:
        explicit SceneManager(const SceneContext &scene_context);

        ~SceneManager();

        void LoadScene(std::unique_ptr<IScene> scene);

        void Update(float delta_time);

    private:
        const SceneContext* m_context;
        std::unique_ptr<IScene> m_current_scene;
        std::unique_ptr<IScene> m_pending_scene;

        void ApplyTransitionToPendingScene();
    };
} // namespace
