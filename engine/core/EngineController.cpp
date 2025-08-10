#include "EngineController.hpp"
#include "WindowBuilder.hpp"

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

        m_rendererController = std::make_unique<Renderer::RenderController>(m_window->GetWindowContext());
    }

    void EngineController::Update() {
        while (true) {
            if (!m_window->PollEvents()) {
                return;
            }
            m_rendererController->Render();

            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() {
        m_rendererController.reset();
        m_window->Shutdown();
    }
} // namespace
