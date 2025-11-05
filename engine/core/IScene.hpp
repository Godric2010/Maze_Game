//
// Created by Sebastian Borsch on 04.11.25.
//

#pragma once
#include "GameWorld.hpp"
#include "SceneContext.hpp"
#include "InputManager.hpp"
#include "IInput.hpp"

namespace Engine::Core {
    class IScene {
    public:
        virtual ~IScene() = default;

        IScene(const IScene &) = delete;

        IScene(IScene &&) = delete;

        IScene &operator=(const IScene &) = default;

        IScene &operator=(IScene &&) = default;

        void Initialize(const SceneContext &scene_context) {
            assert(!m_initialized && "Initialize of scene should be called only once!");
            m_context = &scene_context;
            m_initialized = true;
        }

        virtual void OnStart(){}

        void Update(const float delta_time) const {
            m_context->system_manager.RunSystems(delta_time);
        }

        virtual void OnExit(){}

    protected:
        [[nodiscard]] GameWorld &World() const { return m_context->game_world; }
        [[nodiscard]] IInput &Input() const { return m_context->input; }

    private:
        const SceneContext *m_context;
        bool m_initialized = false;
    };
}
