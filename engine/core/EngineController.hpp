#pragma once
#include <memory>
#include <functional>
#include <vector>

#include "DebugUIDrawer.hpp"
#include "DebugViewData.hpp"
#include "IApplication.hpp"
#include "IEngine.hpp"
#include "InputManager.hpp"
#include "SceneManager.hpp"
#include "ServiceLocator.hpp"
#include "SystemManager.hpp"
#include "../ecs/include/World.hpp"
#include "../renderer/RenderController.hpp"
#include "../environment/include/Window.hpp"
#include "SceneArgs.hpp"

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

        void RegisterInputMap(InputMap map) override;

        void RegisterScene(const std::string& name, SceneFactory scene_factory) override;

        void SetInitialScene(const std::string& name, const SceneArgs& args) override;

    private:
        std::unique_ptr<ServiceLocator> m_services;
        std::unique_ptr<Environment::IWindow> m_window;
        std::unique_ptr<Ecs::SystemManager> m_system_manager;
        std::unique_ptr<InputManager> m_input_manager;

        std::unique_ptr<SceneManager> m_scene_manager;

        bool m_is_running = true;
        std::string m_initial_scene_name;

        DebugViewData m_view_data{};

        float m_fps_frames = 0;
        float m_fps_accumulator = 0;
    };
} // namespace
