#include <iostream>

#include "generated/Generated.hpp"
#include "engine/core/EngineController.hpp"

struct App {
    App() = default;

    ~App() = default;

    void Initialize() {
        const std::vector<Engine::Ecs::SystemMeta> systems = MazeGame::GetSystemsFromGeneratedSource();
        m_engineController = std::make_unique<Engine::Core::EngineController>();
        m_engineController->Initialize(systems);
    }

    void Run() {
        m_engineController->Update();
    }

    void Shutdown() {
        m_engineController->Shutdown();
    }

private:
    std::unique_ptr<Engine::Core::EngineController> m_engineController;
};


int main() {
    App app{};
    app.Initialize();
    app.Run();
    app.Shutdown();
    return 0;
}
