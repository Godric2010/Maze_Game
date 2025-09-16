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

        auto render_controller = std::make_unique<Renderer::RenderController>(m_window->GetWindowContext());
        m_services->RegisterService(std::move(render_controller));

        m_world = std::make_unique<Ecs::World>();
        m_game_world = std::make_unique<GameWorld>(m_world.get());
        m_system_manager = std::make_unique<Ecs::SystemManager>();
        m_system_manager->RegisterSystems(systems, m_services.get());
    }

    void EngineController::Update() const {
        auto last_time = std::chrono::high_resolution_clock::now();

        const auto app_events = m_services->TryGetService<Environment::IInput>()->GetAppEventSnapshot();

        while (!app_events->IsClosed) {
            auto now = std::chrono::high_resolution_clock::now();
            const float delta_time = std::chrono::duration_cast<std::chrono::duration<float> >(now - last_time).count();
            last_time = now;

            m_system_manager->RunSystems(*m_world, delta_time);
            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() const {
        m_window->Shutdown();
    }

    GameWorld &EngineController::GetWorld() const {
        return *m_game_world;
    }

    Renderer::MeshHandle EngineController::RegisterMesh(const Renderer::MeshAsset &mesh_asset) {
        const auto mesh_handle = m_services->TryGetService<Renderer::RenderController>()->RegisterMesh(mesh_asset);
        return mesh_handle;
    }
} // namespace
