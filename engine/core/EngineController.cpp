#include "EngineController.hpp"

#include "CacheManagerFactory.hpp"
#include "DebugBuilder.hpp"
#include "EnvironmentBuilder.hpp"
#include "RenderControllerFactory.hpp"
#include "settings/Settings.hpp"
#include "SystemManager.hpp"
#include "TextController.hpp"
#include "../input/include/InputManagerBuilder.hpp"
#include "settings/SettingsHandler.hpp"

namespace Engine::Core
{
    EngineController::EngineController()
    {
        m_services = std::make_unique<ServiceLocator>();
        m_cache_manager = Systems::CacheManagerFactory::CreateCacheManager();
        m_file_manager = Environment::EnvironmentBuilder::CreateFileManager();
        m_is_running = true;
    };

    EngineController::~EngineController() = default;


    void EngineController::Initialize(const std::vector<Ecs::SystemMeta>& systems)
    {
        const auto engine_settings = Settings::SettingsHandler::ReadSettingsFromDisk(m_file_manager.get());
        SetupWindow(engine_settings);

        AssetHandling::AssetHandler* asset_handler_service = SetupAssetHandler();
        SetupInputManager(asset_handler_service);

        auto render_controller = Renderer::RenderControllerFactory::CreateRenderController(
             m_window->GetWindowContext(),
             asset_handler_service);
        m_services->RegisterService(std::move(render_controller));

        auto text_controller = std::make_unique<Text::TextController>(asset_handler_service);
        m_services->RegisterService(std::move(text_controller));

        m_debug_console = Debug::CreateDebugConsole(m_services->TryGetService<Text::TextController>(),
                                                    m_services->GetService<Renderer::IRenderController>(),
                                                    m_services->GetService<AssetHandling::AssetHandler>(),
                                                    m_window->GetWindowContext(),
                                                    90
                                                   );

        m_system_manager = std::make_unique<Ecs::SystemManager>(systems, m_services.get(), m_cache_manager.get());

        const auto window_context = m_window->GetWindowContext();
        m_scene_manager = std::make_unique<SceneManagement::SceneManager>(*this,
                                                                          *m_system_manager,
                                                                          *m_input_manager,
                                                                          reinterpret_cast<Assets::IAssetLibrary&>(*
                                                                              asset_handler_service),
                                                                          static_cast<float>(window_context.width),
                                                                          static_cast<float>(window_context.height)
                                                                         );
    }

    void EngineController::SetupWindow(const Settings::EngineSettings& settings)
    {
        Environment::WindowMode window_mode = {};
        switch (settings.window.mode)
        {
            case Settings::WindowMode::Windowed:
                window_mode = Environment::WindowMode::Window;
                break;
            case Settings::WindowMode::Borderless:
                window_mode = Environment::WindowMode::Borderless;
                break;
            case Settings::WindowMode::Fullscreen:
                window_mode = Environment::WindowMode::Fullscreen;
                break;
        }

        Environment::API render_api{};
        switch (settings.render.api)
        {
            case Settings::RenderApi::OpenGL:
                render_api = Environment::API::OpenGL;
                break;
            case Settings::RenderApi::Vulkan:
                render_api = Environment::API::Vulkan;
                break;
            case Settings::RenderApi::Metal:
                render_api = Environment::API::Metal;
                break;
        }

        m_window = Environment::EnvironmentBuilder::CreateEngineWindow();
        const Environment::WindowConfig config{
            .width = settings.window.width,
            .height = settings.window.height,
            .title = settings.window.title,
            .renderApi = render_api,
            .windowMode = window_mode,
            .vsync = settings.render.vsync
        };
        m_window->Setup(config);
    }

    AssetHandling::AssetHandler* EngineController::SetupAssetHandler() const
    {
        auto asset_handler = std::make_unique<AssetHandling::AssetHandler>();

        const auto input_map_files = m_file_manager->FindResourceFilesOfType("resources/inputmaps", ".inputmap");
        for (const auto& file : input_map_files)
        {
            asset_handler->LoadAsset<AssetHandling::InputMapAsset>(file);
        }

        m_services->RegisterService(std::move(asset_handler));
        const auto asset_handler_service = m_services->GetService<AssetHandling::AssetHandler>();
        return asset_handler_service;
    }

    void EngineController::SetupInputManager(AssetHandling::AssetHandler* asset_handler)
    {
        const auto input_map_asset_ids = asset_handler->GetAllAssetHandlesOfType<AssetHandling::InputMapAsset>();
        std::vector<Input::InputMap> input_maps;
        input_maps.resize(input_map_asset_ids.size());
        for (size_t i = 0; i < input_map_asset_ids.size(); ++i)
        {
            const auto input_map_asset = asset_handler->GetAsset<AssetHandling::InputMapAsset>(input_map_asset_ids[i]);
            input_maps[i] = input_map_asset->input_map;
        }


        m_input_manager = Input::InputManagerBuilder::CreateInputManager(m_window.get(), input_maps);
    }

    void EngineController::Update()
    {
        using clock = std::chrono::steady_clock;
        auto last_time = clock::now();

        constexpr float fixed_delta_time = 1.0f / 60.0f;
        constexpr float max_frame_dt = 0.25f;
        constexpr int max_steps_per_frame = 8;

        float accumulator = 0.0f;

        while (m_is_running)
        {
            const auto app_events = m_input_manager->GetAppEventSnapshot();
            if (app_events.is_closed)
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
                m_debug_console->PushValue("Draws:",
                                           m_services->GetService<Renderer::IRenderController>()->GetDrawCalls()
                                          );
            }

            accumulator += frame_dt;

            m_input_manager->UpdateInput();
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

    void EngineController::RegisterScene(const std::string& name, const SceneManagement::SceneFactory scene_factory)
    {
        m_scene_manager->RegisterScene(name, scene_factory);
    }

    void EngineController::SetInitialScene(const std::string& name, const SceneManagement::SceneArgs& args)
    {
        m_scene_manager->LoadScene(name, args);
    }
} // namespace
