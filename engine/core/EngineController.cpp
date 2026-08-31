#include "EngineController.hpp"

#include "CacheManagerFactory.hpp"
#include "DebugBuilder.hpp"
#include "EnvironmentBuilder.hpp"
#include "InputManagerBuilder.hpp"
#include "RenderControllerFactory.hpp"
#include "SystemManager.hpp"
#include "TextController.hpp"
#include "settings/settings.hpp"
#include "settings/SettingsHandler.hpp"

namespace yarep::core {
    EngineController::EngineController() {
        m_services = std::make_unique<ServiceLocator>();
        m_cache_manager = systems::CacheManagerFactory::CreateCacheManager();
        m_file_manager = environment::EnvironmentBuilder::CreateFileManager();
        m_is_running = true;
    };

    EngineController::~EngineController() = default;


    void EngineController::Initialize(const std::vector<ecs::SystemMeta>& systems) {
        const auto engine_settings = settings::SettingsHandler::ReadSettingsFromDisk(m_file_manager.get());
        SetupWindow(engine_settings);

        asset_handling::AssetHandler* asset_handler_service = SetupAssetHandler();
        SetupInputManager(asset_handler_service);

        SetupRenderController(asset_handler_service);

        auto text_controller = std::make_unique<text::TextController>(asset_handler_service);
        m_services->RegisterService(std::move(text_controller));

        m_debug_console = debug::CreateDebugConsole(m_services->TryGetService<text::TextController>(),
                                                    m_services->GetService<renderer::IRenderController>(),
                                                    m_services->GetService<asset_handling::AssetHandler>(),
                                                    m_window->GetWindowContext(),
                                                    90
                );

        m_system_manager = std::make_unique<ecs::SystemManager>(systems, m_services.get(), m_cache_manager.get());

        const auto window_context = m_window->GetWindowContext();
        m_scene_manager = std::make_unique<scene_management::SceneManager>(*this,
                                                                          *m_system_manager,
                                                                          *m_input_manager,
                                                                          reinterpret_cast<assets::IAssetLibrary&>(*
                                                                              asset_handler_service),
                                                                          static_cast<float>(window_context.width),
                                                                          static_cast<float>(window_context.height)
                );
    }

    void EngineController::SetupWindow(const settings::EngineSettings& settings) {
        environment::WindowMode window_mode = {};
        switch (settings.window.mode) {
            case settings::WindowMode::Windowed:
                window_mode = environment::WindowMode::Window;
                break;
            case settings::WindowMode::Borderless:
                window_mode = environment::WindowMode::Borderless;
                break;
            case settings::WindowMode::Fullscreen:
                window_mode = environment::WindowMode::Fullscreen;
                break;
        }

        environment::Api render_api{};
        switch (settings.render.api) {
            case settings::RenderApi::OpenGL:
                render_api = environment::Api::OpenGl;
                break;
            case settings::RenderApi::Vulkan:
                render_api = environment::Api::Vulkan;
                break;
            case settings::RenderApi::Metal:
                render_api = environment::Api::Metal;
                break;
        }

        m_window = environment::EnvironmentBuilder::CreateEngineWindow();
        const environment::WindowConfig config{
            .width = settings.window.width,
            .height = settings.window.height,
            .title = settings.window.title,
            .render_api = render_api,
            .window_mode = window_mode,
            .vsync = settings.render.vsync
        };
        m_window->Setup(config);
    }

    asset_handling::AssetHandler* EngineController::SetupAssetHandler() const {
        auto asset_handler = std::make_unique<asset_handling::AssetHandler>();


        m_services->RegisterService(std::move(asset_handler));
        const auto asset_handler_service = m_services->GetService<asset_handling::AssetHandler>();
        return asset_handler_service;
    }

    void EngineController::SetupInputManager(asset_handling::AssetHandler* asset_handler) {
        // Load all input maps from disk and store them as assets
        const std::string directory = "resources/inputmaps";
        const std::vector<std::string> file_extensions = {".inputmap"};
        const auto input_map_files = m_file_manager->FindResourceFilesOfTypes(directory, file_extensions);

        if (!input_map_files.Ok()) {
            throw std::runtime_error("Failed to load input map files" + input_map_files.error.message);
        }

        const auto input_map_asset_ids = asset_handler->LoadAssets<asset_handling::InputMapAsset>(input_map_files.value);

        // Fetch the created input map assets from the asset handler
        std::vector<input::InputMap> input_maps;
        input_maps.resize(input_map_asset_ids.size());
        for (size_t i = 0; i < input_map_asset_ids.size(); ++i) {
            const auto input_map_asset = asset_handler->GetAsset<asset_handling::InputMapAsset>(
                    input_map_asset_ids[i]
                    );
            input_maps[i] = input_map_asset->input_map;
        }

        // Create the input manager with all available input maps
        m_input_manager = input::InputManagerBuilder::CreateInputManager(m_window.get(), input_maps);
    }

    void EngineController::SetupRenderController(asset_handling::AssetHandler* asset_handler_service) const {
        // Load all shader files from disk and store them as assets
        const std::string directory = "resources/shaders";
        const std::vector<std::string> file_extensions = {".vert", ".frag", ".glsl"};
        const auto shader_files = m_file_manager->FindResourceFilesOfTypes(directory, file_extensions);

        if (!shader_files.Ok()) {
            throw std::runtime_error("Failed to load input map files" + shader_files.error.message);
        }
        asset_handler_service->LoadAssets<asset_handling::ShaderAsset>(shader_files.value);

        auto render_controller = renderer::RenderControllerFactory::CreateRenderController(
                m_window->GetWindowContext(),
                asset_handler_service
                );
        m_services->RegisterService(std::move(render_controller));
    }

    void EngineController::Update() {
        using Clock = std::chrono::steady_clock;
        auto last_time = Clock::now();

        constexpr float fixed_delta_time = 1.0f / 60.0f;
        constexpr float max_frame_dt = 0.25f;
        constexpr int max_steps_per_frame = 8;

        float accumulator = 0.0f;

        while (m_is_running) {
            const auto app_events = m_input_manager->GetAppEventSnapshot();
            if (app_events.is_closed) {
                break;
            }

            const auto now = Clock::now();
            float frame_dt = std::chrono::duration_cast<std::chrono::duration<float> >(now - last_time).count();
            last_time = now;

            if (frame_dt >= max_frame_dt) {
                frame_dt = max_frame_dt;
            }

            m_fps_accumulator += frame_dt;
            m_fps_frames++;
            if (m_fps_accumulator >= 1.0f) {
                const auto fps = m_fps_frames / m_fps_accumulator;
                m_fps_accumulator = 0.0f;
                m_fps_frames = 0;
                m_debug_console->PushValue("FPS:", static_cast<size_t>(fps));
                m_debug_console->PushValue("Draws:",
                                           m_services->GetService<renderer::IRenderController>()->GetDrawCalls()
                        );
            }

            accumulator += frame_dt;

            m_input_manager->UpdateInput();
            m_scene_manager->PreFixed(frame_dt);
            int steps = 0;
            while (accumulator >= fixed_delta_time && steps < max_steps_per_frame) {
                m_scene_manager->FixedUpdate(fixed_delta_time);
                accumulator -= fixed_delta_time;
                ++steps;
            }

            if (steps == max_steps_per_frame) {
                accumulator = 0.0f;
            }

            m_debug_console->PushToFrame();
            m_scene_manager->Update(frame_dt);
            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() const {
        m_window->Shutdown();
    }

    void EngineController::Quit() {
        m_is_running = false;
    }

    void EngineController::RegisterScene(const std::string& name,
                                         const scene_management::SceneFactory scene_factory) {
        m_scene_manager->RegisterScene(name, scene_factory);
    }

    void EngineController::SetInitialScene(const std::string& name, const scene_management::SceneArgs& args) {
        m_scene_manager->LoadScene(name, args);
    }
} // namespace
