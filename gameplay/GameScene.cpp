#include "GameScene.hpp"

#include <iostream>
#include <ostream>

namespace Gameplay {
    GameScene::GameScene(MeshHandler *mesh_handler) {
        m_mesh_handler = mesh_handler;
        m_is_paused = false;
    }

    GameScene::~GameScene() {
        m_mesh_handler = nullptr;
        m_maze_builder.reset();
    }

    void GameScene::OnStart() {
        std::cout << "GameScene::OnStart()" << std::endl;
        CreateMaze();
        CreateCamera();
        CreateIngameUiOverlay();
        Input().EnableInputMap("PlayerInputMap");
        Input().SetMouseVisibility(false);
    }

    void GameScene::EvaluateSystemCommands(const std::vector<std::any> &commands) {
        for (const auto &command: commands) {
            if (command.type() == typeid(Commands::PauseCommand)) {
                auto pause_command = std::any_cast<Commands::PauseCommand>(command);
                std::cout << "Enable Pause: " << (pause_command.IsPaused() ? "true" : "false") << std::endl;
                pause_command.IsPaused() ? Pause() : Resume();
                continue;
            }
            if (command.type() == typeid(Commands::LevelFinished)) {
                SceneManager().LoadScene("MainMenu", Engine::Core::SceneArgs{.payload = m_mesh_handler});
                continue;
            }
            if (command.type() == typeid(Engine::Core::Commands::UI::ButtonClickedCommand)) {
                auto button_clicked = std::any_cast<Engine::Core::Commands::UI::ButtonClickedCommand>(command);
                const auto button_id = button_clicked.GetButtonId();
                if (button_id == 1) {
                    Resume();
                } else if (button_id == 2) {
                    SceneManager().LoadScene("MainMenu", Engine::Core::SceneArgs{.payload = m_mesh_handler});
                } else if (button_id == 3) {
                    Application().Quit();
                }
            }
        }
    }

    void GameScene::OnExit() {
        std::cout << "GameScene::OnExit()" << std::endl;
    }

    void GameScene::CreateMaze() {
        m_maze_builder = std::make_unique<Mazegenerator::MazeBuilder>(&World(),
                                                                      m_mesh_handler->GetFloorMesh(),
                                                                      m_mesh_handler->GetWallMesh(),
                                                                      m_mesh_handler->GetKeyMesh(),
                                                                      true);
        m_maze_builder->BuildMaze(5, 5, 1337);
    }

    void GameScene::Pause() const {
        std::cout << "GameScene::Pause()" << std::endl;
        CreatePauseUiOverlay();
        Input().DisableInputMap("PlayerInputMap");
        Input().EnableInputMap("UIInputMap");
        Input().EnableInputMap("PauseInputMap");
        Input().SetMouseVisibility(true);
    }

    void GameScene::Resume() const {
        Input().DisableInputMap("UIInputMap");
        Input().DisableInputMap("PauseInputMap");
        Input().EnableInputMap("PlayerInputMap");
        Input().SetMouseVisibility(false);

        const auto resume_button = World().GetEntityByName("ResumeButton");
        World().DestroyEntity(resume_button);
        const auto main_menu_button = World().GetEntityByName("MainMenuButton");
        World().DestroyEntity(main_menu_button);
        const auto quit_button = World().GetEntityByName("QuitButton");
        World().DestroyEntity(quit_button);

        const auto pause_entity = World().GetEntityByName("PauseBackground");
        World().DestroyEntity(pause_entity);
    }

    void GameScene::CreateCamera() const {
        auto player = World().CreateEntity("Player");
        const auto [width, height, aspect_ratio] = Screen();
        const auto camera_component = Engine::Core::Components::Camera{
            .Width = width,
            .Height = height,
            .FieldOfView = 60,
            .AspectRatio = aspect_ratio,
            .NearClip = 0.01f,
            .FarClip = 1000.0f,

        };
        World().AddComponent<Engine::Core::Components::Camera>(player, camera_component);

        const auto camera_transform = Engine::Core::Components::Transform(m_maze_builder->GetMazeStartPosition(),
                                                                          glm::vec3(-10.f, 180.0f, 0.0f));
        World().AddComponent(player, camera_transform);
        const auto camera_motion_intent = Engine::Core::Components::MotionIntent();
        World().AddComponent(player, camera_motion_intent);

        constexpr auto camera_collider = Engine::Core::Components::SphereCollider{
            .is_static = false, .radius = 0.1f
        };
        World().AddComponent(player, camera_collider);

        constexpr auto inventory = Components::Inventory();
        World().AddComponent(player, inventory);
    }

    void GameScene::CreateIngameUiOverlay() const {
        const auto key_indicator = World().CreateEntity("KeyIndicator");

        const auto screen = Screen();
        constexpr glm::vec2 size = {100, 100};
        const glm::vec2 position = {screen.width - size.x - 50, screen.height - size.y - 50};
        const auto transform = Engine::Core::Components::UI::RectTransform(position, size, 0);
        World().AddComponent(key_indicator, transform);

        constexpr auto image = Engine::Core::Components::UI::Image{.color = {1, 0, 0, 0.5}};
        World().AddComponent(key_indicator, image);
    }

    void GameScene::CreatePauseUiOverlay() const {
        const auto pause_entity = World().CreateEntity("PauseBackground");

        const auto screen = Screen();
        const auto bg_position = glm::vec2(screen.width / 2.0f, screen.height / 2.0f);
        const auto bg_size = glm::vec2(screen.width * 0.9f, screen.height * 0.9f);
        constexpr auto bg_pivot = glm::vec2(0.5f, 0.5f);
        const auto bg_rect_transform = Engine::Core::Components::UI::RectTransform(
            bg_position, bg_size, bg_pivot, 0.0f, 0);
        World().AddComponent(pause_entity, bg_rect_transform);

        constexpr auto bg_image = Engine::Core::Components::UI::Image{.color = {0.3, 0.3, 0.3, 0.9}};
        World().AddComponent(pause_entity, bg_image);

        constexpr auto button_size = glm::vec2(200, 100);
        const auto resume_button_entity = World().CreateEntity("ResumeButton");
        const auto pos = glm::vec2(screen.width / 2, screen.height / 2 - 100);
        constexpr auto pivot = glm::vec2(0.5f, 0.5f);
        auto resume_rect = Engine::Core::Components::UI::RectTransform(pos, button_size, pivot, 0.0f, 1);
        World().AddComponent(resume_button_entity, resume_rect);

        auto resume_button = Engine::Core::Components::UI::Button();
        resume_button.button_id = 1;
        resume_button.enabled = true;
        resume_button.default_color = {0.0f, 0.7f, 0.0f, 1.0f};
        resume_button.highlight_color = {0.0f, 1.0f, 0.0f, 1.0f};
        resume_button.click_color = {1.0f, 1.0f, 1.0f, 1.0f};
        resume_button.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        World().AddComponent(resume_button_entity, resume_button);

        const auto main_menu_button_entity = World().CreateEntity("MainMenuButton");
        const auto main_menu_button_pos = pos + glm::vec2(0, 200);
        auto main_menu_button_rect = Engine::Core::Components::UI::RectTransform(
            main_menu_button_pos, button_size, pivot, 0.0f, 1);
        World().AddComponent(main_menu_button_entity, main_menu_button_rect);

        auto main_menu_button = Engine::Core::Components::UI::Button();
        main_menu_button.button_id = 2;
        main_menu_button.enabled = true;
        main_menu_button.default_color = {0.0f, 0.0f, 0.7f, 1.0f};
        main_menu_button.highlight_color = {0.0f, 0.0f, 1.0f, 1.0f};
        main_menu_button.click_color = {1.0f, 1.0f, 1.0f, 1.0f};
        main_menu_button.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        World().AddComponent(main_menu_button_entity, main_menu_button);

        const auto quit_button_entity = World().CreateEntity("QuitButton");
        const auto quit_pos = pos + glm::vec2(0, 400);
        World().AddComponent(quit_button_entity,
                             Engine::Core::Components::UI::RectTransform(
                                 quit_pos, button_size, pivot, 0.0f, 1));
        auto quit_button = Engine::Core::Components::UI::Button();
        quit_button.button_id = 3;
        quit_button.enabled = true;
        quit_button.default_color = {0.7f, 0.0f, 0.0f, 1.0f};
        quit_button.highlight_color = {1.0f, 0.0f, 0.0f, 1.0f};
        quit_button.click_color = {1.0f, 1.0f, 1.0f, 1.0f};
        quit_button.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        World().AddComponent(quit_button_entity, quit_button);
    }
} // namespace
