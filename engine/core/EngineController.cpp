#include "EngineController.hpp"

#include "CacheManagerFactory.hpp"
#include "DebugBuilder.hpp"
#include "EnvironmentBuilder.hpp"
#include "RenderControllerFactory.hpp"
#include "SystemManager.hpp"
#include "TextController.hpp"
#include "../input/include/InputManagerBuilder.hpp"

namespace Engine::Core {
    EngineController::EngineController() {
        m_services = std::make_unique<ServiceLocator>();
        m_cache_manager = Systems::CacheManagerFactory::CreateCacheManager();
        m_is_running = true;
    };

    EngineController::~EngineController() = default;

    void EngineController::Initialize(const std::vector<Ecs::SystemMeta>& systems) {
        m_window = Environment::CreateWindow();
        const Environment::WindowConfig config{
            .width = 1920,
            .height = 1080,
            .title = "MazeGame",
            .renderApi = Environment::API::OpenGL,
            .windowMode = Environment::WindowMode::Window
        };
        m_window->Setup(config);

        m_input_manager = Input::CreateInputManager(m_window.get());

        auto render_controller =
                Renderer::RenderControllerFactory::CreateRenderController(m_window->GetWindowContext());
        m_services->RegisterService(std::move(render_controller));

        auto text_controller = std::make_unique<Text::TextController>();
        m_services->RegisterService(std::move(text_controller));

        m_debug_console = Debug::CreateDebugConsole(m_services->TryGetService<Text::TextController>(),
                                                    m_services->GetService<Renderer::IRenderController>(),
                                                    m_window->GetWindowContext(),
                                                    90
                );

        m_system_manager = std::make_unique<Ecs::SystemManager>(systems, m_services.get(), m_cache_manager.get());

        const auto window_context = m_window->GetWindowContext();
        m_scene_manager = std::make_unique<SceneManagement::SceneManager>(*this,
                                                                          *m_system_manager,
                                                                          *m_input_manager,
                                                                          static_cast<float>(window_context.width),
                                                                          static_cast<float>(window_context.height)
                );
    }

    void EngineController::Update() {
        auto last_time = std::chrono::high_resolution_clock::now();
        const auto app_events = m_input_manager->GetAppEventSnapshot();

        while (!app_events->is_closed && m_is_running) {
            auto now = std::chrono::high_resolution_clock::now();
            const float delta_time = std::chrono::duration_cast<std::chrono::duration<float> >(now - last_time).count();
            last_time = now;

            m_fps_accumulator += delta_time;
            m_fps_frames++;
            if (m_fps_accumulator >= 1.0f) {
                const auto fps = m_fps_frames / m_fps_accumulator;
                m_fps_accumulator = 0.0f;
                m_fps_frames = 0;
                m_debug_console->PushValue("FPS:", fps);
            }
            m_debug_console->PushValue("Draws:",
                                       m_services->GetService<Renderer::IRenderController>()->GetDrawCalls()
                    );

            m_debug_console->PushToFrame();
            m_input_manager->UpdateInput();
            m_scene_manager->Update(delta_time);
            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() const {
        m_window->Shutdown();
    }

    void EngineController::Quit() {
        m_is_running = false;
    }

    Renderer::MeshHandle EngineController::RegisterMesh(const Renderer::MeshAsset& mesh_asset) {
        const auto mesh_handle = m_services->TryGetService<Renderer::IRenderController>()->RegisterMesh(mesh_asset);
        return mesh_handle;
    }

    void EngineController::RegisterInputMap(const Input::InputMap map) {
        m_input_manager->AddInputMapping(map);
    }

    void EngineController::RegisterScene(const std::string& name, const SceneManagement::SceneFactory scene_factory) {
        m_scene_manager->RegisterScene(name, scene_factory);
    }

    void EngineController::SetInitialScene(const std::string& name, const SceneManagement::SceneArgs& args) {
        m_scene_manager->LoadScene(name, args);
    }
} // namespace
