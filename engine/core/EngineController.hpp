#pragma once
#include <memory>

#include "Camera.hpp"
#include "../renderer/RenderController.hpp"
#include "../window/include/Window.hpp"

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
        void Initialize();

        /**
         * Update the engines systems, like drawing, the objects in the world, etc.
         */
        void Update() const;

        /**
         * Shutdown the engines system and free all resources.
         */
        void Shutdown();

    private:

        void RenderFrame() const;

        std::unique_ptr<Window::IWindow> m_window;
        std::unique_ptr<Renderer::RenderController> m_rendererController;

        // TODO: Move this into the ECS as soon as Version 0.3 is in the making
        std::unique_ptr<Camera> m_camera;
        std::vector<Renderer::MeshAsset> m_meshes;
        std::vector<Renderer::MeshHandle> m_meshHandles;
        std::vector<Renderer::DrawAsset> m_drawAssets;
    };
} // namespace
