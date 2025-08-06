#include "EngineController.hpp"

namespace Engine::Core {
    EngineController::EngineController() = default;

    EngineController::~EngineController() = default;

    void EngineController::Initialize() {
        m_window = Window::CreateWindow();
        Window::WindowConfig config{};
        m_window->Setup(config);
    }

    void EngineController::Update() {
        m_window->PollEvents();
    }

    void EngineController::Shutdown() {
        m_window->Shutdown();
    }
} // namespace
