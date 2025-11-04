#include "EngineController.hpp"

#include "Transform.hpp"
#include "EnvironmentBuilder.hpp"
#include "SystemManager.hpp"

namespace Engine::Core {
    EngineController::EngineController() {
        m_services = std::make_unique<ServiceLocator>();
        m_is_running = true;
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

        auto input = CreateInput(*m_window);
        m_input_manager = std::make_unique<InputManager>(std::move(input));

        auto render_controller = std::make_unique<Renderer::RenderController>(m_window->GetWindowContext());
        m_services->RegisterService(std::move(render_controller));

        m_world = std::make_unique<Ecs::World>();
        m_game_world = std::make_unique<GameWorld>(m_world.get(), m_input_manager.get());
        m_system_manager = std::make_unique<Ecs::SystemManager>();
        m_system_manager->RegisterSystems(systems, m_world.get(), m_services.get(), m_game_world.get());
    }

    void EngineController::Update() const {
        auto last_time = std::chrono::high_resolution_clock::now();


        const auto app_events = m_input_manager->GetAppEventSnapshot();
        //m_services->TryGetService<Environment::IInput>()->GetAppEventSnapshot();

        while (!app_events->is_closed && m_is_running) {
            auto now = std::chrono::high_resolution_clock::now();
            const float delta_time = std::chrono::duration_cast<std::chrono::duration<float> >(now - last_time).count();
            last_time = now;

            m_input_manager->UpdateInput();
            m_system_manager->RunSystems(delta_time);
            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() const {
        m_window->Shutdown();
    }

    void EngineController::StopExecution() {
        m_is_running = false;
    }

    GameWorld &EngineController::GetWorld() const {
        return *m_game_world;
    }

    Renderer::MeshHandle EngineController::RegisterMesh(const Renderer::MeshAsset &mesh_asset) {
        const auto mesh_handle = m_services->TryGetService<Renderer::RenderController>()->RegisterMesh(mesh_asset);
        return mesh_handle;
    }

    void EngineController::RegisterForSystemCommands(
        const std::function<void(std::vector<std::any>)> command_callback) {
        m_system_manager->RegisterForSystemCommands(command_callback);
    }

    void EngineController::RegisterInputMap(const InputMap map) {
        m_input_manager->AddInputMapping(map);
    }

    void EngineController::EnableInputMap(const std::string input_map) {
        m_input_manager->EnableInputMap(input_map);
    }

    void EngineController::DisableInputMap(const std::string input_map) {
        m_input_manager->DisableInputMap(input_map);
    }

    Screen EngineController::GetScreen() {
        const auto window_context = m_window->GetWindowContext();
        return Screen{
            .width = static_cast<float>(window_context.width),
            .height = static_cast<float>(window_context.height),
            .scale_x = static_cast<float>(window_context.drawableWidth / window_context.width),
            .scale_y = static_cast<float>(window_context.drawableHeight / window_context.height),
            .aspect = static_cast<float>(window_context.width / window_context.height),
        };
    }
} // namespace
