#include "EngineController.hpp"

#include "Transform.hpp"
#include "EnvironmentBuilder.hpp"
#include "SystemManager.hpp"

namespace Engine::Core {
    EngineController::EngineController() {
        m_services = std::make_unique<ServiceLocator>();
    };

    EngineController::~EngineController() = default;

    void EngineController::Initialize(const std::vector<Ecs::SystemMeta> &systems) {
        m_window = Environment::CreateWindow();
        const Environment::WindowConfig config{
            .width = 1920,
            .height = 1080,
            .title = "MazeGame",
            .renderApi = Environment::API::OpenGL,
            .windowMode = Environment::WindowMode::Window
        };
        m_window->Setup(config);

        auto input = Environment::CreateInput(*m_window);
        m_services->RegisterService(std::move(input));

        auto renderController = std::make_unique<Renderer::RenderController>(m_window->GetWindowContext());
        m_services->RegisterService(std::move(renderController));

        m_world = std::make_unique<Ecs::World>();
        m_systemManager = std::make_unique<Ecs::SystemManager>();
        m_systemManager->RegisterSystems(systems, m_services.get());
    }

    void EngineController::Update() const {
        auto lastTime = std::chrono::high_resolution_clock::now();

        const auto appEvents = m_services->TryGetService<Environment::IInput>()->GetAppEventSnapshot();

        while (!appEvents->IsClosed) {
            auto now = std::chrono::high_resolution_clock::now();
            const float deltaTime = std::chrono::duration_cast<std::chrono::duration<float> >(now - lastTime).count();
            lastTime = now;

            m_systemManager->RunSystems(*m_world, deltaTime);
            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() const {
        m_window->Shutdown();
    }

    Ecs::World &EngineController::GetWorld() const {
        return *m_world;
    }

    Renderer::MeshHandle EngineController::RegisterMesh(const Renderer::MeshAsset &meshAsset) {
        const auto meshHandle = m_services->TryGetService<Renderer::RenderController>()->RegisterMesh(meshAsset);
        return meshHandle;
    }

} // namespace
