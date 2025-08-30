#include <iostream>

#include "generated/Generated.hpp"
#include "engine/core/EngineController.hpp"
#include "gameplay/GameplayManager.hpp"

struct App {
    App() = default;

    ~App() = default;

    void Initialize() {
        const std::vector<Engine::Ecs::SystemMeta> systems = MazeGame::GetSystemsFromGeneratedSource();
        m_engineController = std::make_unique<Engine::Core::EngineController>();
        m_engineController->Initialize(systems);

        m_gameplayManager = std::make_unique<Gameplay::GameplayManager>(*m_engineController);
        m_gameplayManager->Initialize();
    }

    void Run() {
        m_engineController->Update();
    }

    void Shutdown() {
        m_gameplayManager->Shutdown();
        m_engineController->Shutdown();
    }

private:
    std::unique_ptr<Engine::Core::EngineController> m_engineController;
    std::unique_ptr<Gameplay::GameplayManager> m_gameplayManager;
};


int main() {
    App app{};
    app.Initialize();
    app.Run();
    app.Shutdown();
    return 0;
}
