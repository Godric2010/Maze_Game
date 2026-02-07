#include "EngineController.hpp"

#include "CacheManagerFactory.hpp"
#include "DebugBuilder.hpp"
#include "EnvironmentBuilder.hpp"
#include "RenderControllerFactory.hpp"
#include "SystemManager.hpp"
#include "TextController.hpp"
#include "../input/include/InputManagerBuilder.hpp"

namespace Engine::Core
{
    EngineController::EngineController()
    {
        m_services = std::make_unique<ServiceLocator>();
        m_cache_manager = Systems::CacheManagerFactory::CreateCacheManager();
        m_file_reader = Environment::EnvironmentBuilder::CreateFileReader();
        m_is_running = true;
    };

    EngineController::~EngineController() = default;

    void EngineController::Initialize(const std::vector<Ecs::SystemMeta>& systems)
    {
        m_window = Environment::EnvironmentBuilder::CreateEngineWindow();
        const Environment::WindowConfig config{
            .width = 1920,
            .height = 1080,
            .title = "MazeGame",
            .renderApi = Environment::API::OpenGL,
            .windowMode = Environment::WindowMode::Window
        };
        m_window->Setup(config);
        m_asset_handler = std::make_unique<AssetHandling::AssetHandler>();

        m_input_manager = Input::InputManagerBuilder::CreateInputManager(m_window.get());

        auto render_controller = Renderer::RenderControllerFactory::CreateRenderController(
            m_window->GetWindowContext(),
            m_asset_handler.get());
        m_services->RegisterService(std::move(render_controller));

        auto text_controller = std::make_unique<Text::TextController>(m_asset_handler.get());
        m_services->RegisterService(std::move(text_controller));

        m_debug_console = Debug::CreateDebugConsole(m_services->TryGetService<Text::TextController>(),
                                                    m_services->GetService<Renderer::IRenderController>(),
                                                    m_window->GetWindowContext(),
                                                    90
        );

        m_system_manager = std::make_unique<Ecs::SystemManager>(systems, m_services.get(), m_cache_manager.get());

        auto renderer = m_services->TryGetService<Renderer::IRenderController>();
        const auto window_context = m_window->GetWindowContext();
        m_scene_manager = std::make_unique<SceneManagement::SceneManager>(*this,
                                                                          *m_system_manager,
                                                                          *m_input_manager,
                                                                          reinterpret_cast<Renderer::IRenderer&>(*
                                                                              renderer),
                                                                          static_cast<float>(window_context.width),
                                                                          static_cast<float>(window_context.height)
        );
    }

    void EngineController::Update()
    {
        using clock = std::chrono::high_resolution_clock;
        auto last_time = clock::now();

        constexpr float fixed_delta_time = 1.0f / 60.0f;
        constexpr float max_frame_dt = 0.25f;
        constexpr int max_steps_per_frame = 8;

        float accumulator = 0.0f;

        while (m_is_running)
        {
            const auto app_events = m_input_manager->GetAppEventSnapshot();
            if (app_events->is_closed)
            {
                break;
            }

            const auto now = clock::now();
            float frame_dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_time).count();
            last_time = now;

            if (frame_dt >= max_frame_dt)
            {
                frame_dt = max_frame_dt;
            }

            m_fps_accumulator += frame_dt;
            m_fps_frames++;
            if (m_fps_accumulator >= 1.0f)
            {
                const auto fps = m_fps_frames / m_fps_accumulator;
                m_fps_accumulator = 0.0f;
                m_fps_frames = 0;
                m_debug_console->PushValue("FPS:", static_cast<size_t>(fps));
            }
            m_debug_console->PushValue("Draws:",
                                       m_services->GetService<Renderer::IRenderController>()->GetDrawCalls()
            );

            accumulator += frame_dt;

            m_scene_manager->PreFixed(frame_dt);
            int steps = 0;
            while (accumulator >= fixed_delta_time && steps < max_steps_per_frame)
            {
                m_scene_manager->FixedUpdate(fixed_delta_time);
                accumulator -= fixed_delta_time;
                ++steps;
            }

            if (steps == max_steps_per_frame)
            {
                accumulator = 0.0f;
            }

            m_debug_console->PushToFrame();
            m_input_manager->UpdateInput();
            m_scene_manager->Update(frame_dt);
            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() const
    {
        m_window->Shutdown();
    }

    void EngineController::Quit()
    {
        m_is_running = false;
    }

    void EngineController::RegisterInputMap(const Input::InputMap map)
    {
        m_input_manager->AddInputMapping(map);
    }

    void EngineController::RegisterScene(const std::string& name, const SceneManagement::SceneFactory scene_factory)
    {
        m_scene_manager->RegisterScene(name, scene_factory);
    }

    void EngineController::SetInitialScene(const std::string& name, const SceneManagement::SceneArgs& args)
    {
        m_scene_manager->LoadScene(name, args);
    }
} // namespace
