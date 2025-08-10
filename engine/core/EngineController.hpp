#pragma once
#include <memory>

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
        void Update();

        /**
         * Shutdown the engines system and free all resources.
         */
        void Shutdown();

    private:
        std::unique_ptr<Window::IWindow> m_window;
        std::unique_ptr<Renderer::RenderController> m_rendererController;
    };
} // namespace
