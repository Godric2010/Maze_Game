//
// Created by Sebastian Borsch on 04.11.25.
//

#pragma once
#include <exception>
#include <iostream>
#include <ostream>
#include <spdlog/spdlog.h>

#include "../src/SceneContext.hpp"
#include "Input/IInput.hpp"

namespace Engine::SceneManagement
{
    class SceneRegistry;

    struct ScreenInfo
    {
        float width;
        float height;
        float aspect_ratio;
    };

    class SceneRuntimeException : public std::exception
    {
        const char* what() const throw() override
        {
            spdlog::critical("Error in scene runtime. Context has not been initialized!");
            return "SceneRuntimeException";
        }
    };

    class IScene
    {
        friend class SceneRegistry;

    public:
        virtual ~IScene() = default;

        void Initialize(const SceneContext& scene_context)
        {
            assert(!m_initialized && "Initialize of scene should be called only once!");
            std::cout << "Initializing scene" << m_scene_name << "..." << std::endl;
            m_context = &scene_context;
            m_initialized = true;
            m_context->system_manager.RegisterForSystemCommands(
                m_scene_name,
                [this](const std::vector<std::any>& commands)
                {
                    this->EvaluateSystemCommands(commands);
                }
            );
        }

        virtual void OnStart() = 0;

        virtual void EvaluateSystemCommands(const std::vector<std::any>& commands)
        {
        }

        void Update(const float delta_time) const
        {
            m_context->system_manager.RunSystems(delta_time);
        }

        virtual void OnExit() = 0;

        void UnloadScene()
        {
            std::cout << "Unloading scene " << m_scene_name << "..." << std::endl;
            m_context->world.ClearEntities();
            m_context->system_manager.DeregisterForSystemCommands(m_scene_name);

            m_initialized = false;
        }

    protected:
        [[nodiscard]] SceneWorld& World() const
        {
            if (m_context == nullptr)
            {
                throw SceneRuntimeException();
            }
            return m_context->game_world;
        }

        [[nodiscard]] Input::IInput& Input() const
        {
            if (m_context == nullptr)
            {
                throw SceneRuntimeException();
            }
            return m_context->input;
        }

        [[nodiscard]] IApplication& Application() const
        {
            if (m_context == nullptr)
            {
                throw SceneRuntimeException();
            }
            return m_context->app;
        }

        [[nodiscard]] ISceneManager& SceneManager() const
        {
            if (m_context == nullptr)
            {
                throw SceneRuntimeException();
            }
            return m_context->scene_manager;
        }

        [[nodiscard]] Renderer::IRenderer& Renderer() const
        {
            if (m_context == nullptr)
            {
                throw SceneRuntimeException();
            }
            return m_context->renderer;
        }

        [[nodiscard]] ScreenInfo Screen() const
        {
            return ScreenInfo{
                .width = m_context->screen_width,
                .height = m_context->screen_height,
                .aspect_ratio = m_context->screen_width / m_context->screen_height
            };
        }

    private:
        std::string m_scene_name;
        const SceneContext* m_context{};
        bool m_initialized = false;
    };
}
