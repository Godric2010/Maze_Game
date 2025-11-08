#pragma once
#include <unordered_map>
#include "IScene.hpp"
#include "SceneRegistry.hpp"

namespace Engine::Core {
    class ISceneManager {
    public:
        virtual ~ISceneManager() = default;

        virtual void LoadScene(const std::string &name, const SceneArgs &args) = 0;
    };

    class SceneManager : public ISceneManager {
    public:
        explicit SceneManager(const SceneContext &scene_context);

        ~SceneManager() override;

        void RegisterScene(const std::string &name, const SceneFactory &scene_factory) const;

        void LoadScene(const std::string &name, const SceneArgs &args) override;

        void Update(float delta_time);

    private:
        const SceneContext *m_context;
        std::unique_ptr<IScene> m_current_scene;
        std::unique_ptr<IScene> m_pending_scene;

        std::unique_ptr<SceneRegistry> m_registry;

        void ApplyTransitionToPendingScene();
    };
} // namespace
