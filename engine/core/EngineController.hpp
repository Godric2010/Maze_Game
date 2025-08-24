#pragma once
#include <memory>

#include "Camera.hpp"
#include "SystemManager.hpp"
#include "../ecs/include/World.hpp"
#include "../renderer/RenderController.hpp"
#include "../environment/include/Window.hpp"
#include "../environment/include/Input.hpp"

namespace Engine::Core {
    /**
     * @class EngineController
     * The central engine controller that brings all elements of the engine together.
     * All systems and their events are managed through this instance.
     */
    class EngineController {
    public:
        EngineController();

        ~EngineController();

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
        void Shutdown();

    private:

        Environment::InputSnapshot PumpInput();

        void UpdateSystems(float dt, const Environment::InputSnapshot &snapshot) const;

        void RenderFrame() const;

        bool m_isClosed;
        bool m_isPaused;

        std::unique_ptr<Environment::IWindow> m_window;
        std::unique_ptr<Environment::IInput> m_input;
        std::unique_ptr<Renderer::RenderController> m_rendererController;
        std::unique_ptr<Ecs::World> m_world;
        std::unique_ptr<Ecs::SystemManager> m_systemManager;

        // TODO: Move this into the ECS as soon as Version 0.3 is in the making
        std::unique_ptr<Camera> m_camera;
        std::vector<Renderer::MeshAsset> m_meshes;
        std::vector<Renderer::MeshHandle> m_meshHandles;
        std::vector<Renderer::DrawAsset> m_drawAssets;
    };
} // namespace
