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

        m_context.emplace(SceneContext{
            .world = *m_world,
            .game_world = *m_game_world,
            .system_manager = *m_system_manager,
            .input = *m_input_manager,
            .screen_width = static_cast<float>(m_window->GetWindowContext().width),
            .screen_height = static_cast<float>(m_window->GetWindowContext().height),
        });
        m_scene_manager = std::make_unique<SceneManager>(*m_context);
    }

    void EngineController::Update() const {
        auto last_time = std::chrono::high_resolution_clock::now();
        const auto app_events = m_input_manager->GetAppEventSnapshot();

        while (!app_events->is_closed && m_is_running) {
            auto now = std::chrono::high_resolution_clock::now();
            const float delta_time = std::chrono::duration_cast<std::chrono::duration<float> >(now - last_time).count();
            last_time = now;

            m_input_manager->UpdateInput();
            m_scene_manager->Update(delta_time);
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

    void EngineController::LoadScene(std::unique_ptr<IScene> scene) {
        m_scene_manager->LoadScene(std::move(scene));
    }
} // namespace
