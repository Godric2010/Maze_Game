#include <iostream>

#include "generated/Generated.hpp"
#include "engine/core/EngineController.hpp"
#include "gameplay/GameplayManager.hpp"

struct App {
    App() = default;

    ~App() = default;

    void Initialize() {
        const std::vector<yarep::ecs::SystemMeta> systems = MazeGame::GetSystemsFromGeneratedSource();
        m_engine_controller = std::make_unique<yarep::Core::EngineController>();
        m_engine_controller->Initialize(systems);

        m_gameplay_manager = std::make_unique<gameplay::GameplayManager>(*m_engine_controller);
        m_gameplay_manager->Initialize();
    }

    void Run() const {
        m_engine_controller->Update();
    }

    void Shutdown() const {
        m_gameplay_manager->Shutdown();
        m_engine_controller->Shutdown();
    }

private:
    std::unique_ptr<yarep::Core::EngineController> m_engine_controller;
    std::unique_ptr<gameplay::GameplayManager> m_gameplay_manager;
};


int main() {
    App app{};
    app.Initialize();
    app.Run();
    app.Shutdown();
    return 0;
}
