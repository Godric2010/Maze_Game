#pragma once
#include <memory>
#include "../window/include/Window.hpp"
#include "../window/include/WindowBuilder.hpp"

namespace Engine::Core {
    class EngineController {
    public:
        EngineController();

        ~EngineController();

        void Initialize();

        void Update();

        void Shutdown();

    private:
        std::unique_ptr<Window::IWindow> m_window;
    };
} // namespace