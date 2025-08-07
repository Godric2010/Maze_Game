#include "EngineController.hpp"

#include "RendererBuilder.hpp"

namespace Engine::Core {
    EngineController::EngineController() = default;

    EngineController::~EngineController() = default;

    void EngineController::Initialize() {
        m_window = Window::CreateWindow();
        Window::WindowConfig config{
            .width = 800,
            .height = 600,
            .title = "MazeGame",
            .renderApi = Window::API::OpenGL,
            .windowMode = Window::WindowMode::Window
        };
        m_window->Setup(config);

        m_renderer = Renderer::CreateRenderer(m_window->GetWindowContext());
        m_renderer->Initialize();
    }

    void EngineController::Update() {
        while (true) {
            if (!m_window->PollEvents()) {
                return;
            }
            m_renderer->Render();

            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() {
        m_renderer->Shutdown();
        m_window->Shutdown();
    }
} // namespace
