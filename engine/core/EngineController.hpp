#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>

#include "IDebugConsole.hpp"
#include "IEngine.hpp"
#include "IInputManager.hpp"
#include "../scenemanagement/src/SceneManager.hpp"
#include "ServiceLocator.hpp"
#include "SystemManager.hpp"
#include "../ecs/include/World.hpp"
#include "../environment/include/Window.hpp"
#include "../systems/include/CacheManager.hpp"

namespace Engine::Core {
    /**
     * @class EngineController
     * The central engine controller that brings all elements of the engine together.
     * All systems and their events are managed through this instance.
     */
    class EngineController final : public IEngine, public IApplication {
    public:
        EngineController();

        ~EngineController() override;

        /**
         * Initialize the engine backend
         */
        void Initialize(const std::vector<Ecs::SystemMeta>& systems);

        /**
         * Update the engines systems, like drawing, the objects in the world, etc.
         */
        void Update();

        /**
         * Shutdown the engines system and free all resources.
         */
        void Shutdown() const;

        void Quit() override;

        Renderer::MeshHandle RegisterMesh(const Renderer::MeshAsset& mesh_asset) override;

        void RegisterInputMap(Input::InputMap map) override;

        void RegisterScene(const std::string& name, SceneManagement::SceneFactory scene_factory) override;

        void SetInitialScene(const std::string& name, const SceneManagement::SceneArgs& args) override;

    private:
        std::unique_ptr<ServiceLocator> m_services;
        std::unique_ptr<Environment::IWindow> m_window;
        std::unique_ptr<Ecs::SystemManager> m_system_manager;
        std::unique_ptr<Systems::CacheManager> m_cache_manager;
        std::unique_ptr<Input::IInputManager> m_input_manager;
        std::unique_ptr<Debug::IDebugConsole> m_debug_console;

        std::unique_ptr<SceneManagement::SceneManager> m_scene_manager;

        bool m_is_running = true;
        std::string m_initial_scene_name;

        float m_fps_frames = 0;
        float m_fps_accumulator = 0;
    };
} // namespace
