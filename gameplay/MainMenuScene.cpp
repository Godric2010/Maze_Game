#include "MainMenuScene.hpp"



namespace Gameplay {
    MainMenuScene::MainMenuScene(MeshHandler *mesh_handler) {
        m_mesh_handler = mesh_handler;
    }

    MainMenuScene::~MainMenuScene() {
        m_mesh_handler = nullptr;
    }

    void MainMenuScene::OnStart() {
        SetupCamera();
        CreateMainMenuUiElements();
        Input().EnableInputMap("UIInputMap");
        Input().SetMouseVisibility(true);
    }

    void MainMenuScene::EvaluateSystemCommands(const std::vector<std::any> &commands) {
        for (const std::any &command: commands) {
            if (command.type() == typeid(Engine::Core::Commands::UI::ButtonClickedCommand)) {
                auto button_clicked = std::any_cast<Engine::Core::Commands::UI::ButtonClickedCommand>(command);
                const auto button_id = button_clicked.GetButtonId();
                if (button_id == m_start_game_button) {
                    // load main game level
                    SceneManager().LoadScene("Game", Engine::Core::SceneArgs{.payload = m_mesh_handler});
                } else if (button_id == m_quit_button) {
                    Application().Quit();
                }
            }
        }
    }

    void MainMenuScene::OnExit() {
        for (auto entity: m_scene_entities) {
            World().DestroyEntity(entity);
        }
        Input().DisableInputMap("UIInputMap");
        Input().SetMouseVisibility(false);
    }

    void MainMenuScene::SetupCamera() {
        const auto camera_entity = World().CreateEntity("MainCamera");
        const auto [width, height, aspect_ratio] = Screen();
        const auto camera_component = Engine::Core::Components::Camera{
            .Width = width,
            .Height = height,
            .FieldOfView = 60,
            .AspectRatio = aspect_ratio,
            .NearClip = 0.01f,
            .FarClip = 1000.0f,

        };
        World().AddComponent<Engine::Core::Components::Camera>(camera_entity, camera_component);

        const auto camera_transform = Engine::Core::Components::Transform();
        World().AddComponent(camera_entity, camera_transform);
        m_scene_entities.push_back(camera_entity);
    }

    void MainMenuScene::CreateMainMenuUiElements() {
        const auto pause_entity = World().CreateEntity("MenuBackground");
        const auto screen = Screen();
        const auto bg_position = glm::vec2(screen.width / 2.0f, screen.height / 2.0f);
        const auto bg_size = glm::vec2(screen.width, screen.height);
        constexpr auto bg_pivot = glm::vec2(0.5f, 0.5f);
        const auto bg_rect_transform = Engine::Core::Components::UI::RectTransform(
            bg_position, bg_size, bg_pivot, 0.0f, 1);
        World().AddComponent(pause_entity, bg_rect_transform);
        constexpr auto bg_image = Engine::Core::Components::UI::Image{.color = {0.2, 0.4, 0.2, 1.0}};
        World().AddComponent(pause_entity, bg_image);
        m_scene_entities.push_back(pause_entity);

        constexpr auto button_size = glm::vec2(200, 100);
        const auto start_game_button_entity = World().CreateEntity("StartGameButton");
        const auto pos = glm::vec2(screen.width / 2, screen.height / 2 - 100);
        constexpr auto pivot = glm::vec2(0.5f, 0.5f);
        auto resume_rect = Engine::Core::Components::UI::RectTransform(pos, button_size, pivot, 0.0f, 1);
        World().AddComponent(start_game_button_entity, resume_rect);

        auto start_game = Engine::Core::Components::UI::Button();
        start_game.button_id = m_start_game_button;
        start_game.enabled = true;
        start_game.default_color = {0.0f, 0.7f, 0.0f, 1.0f};
        start_game.highlight_color = {0.0f, 1.0f, 0.0f, 1.0f};
        start_game.click_color = {1.0f, 1.0f, 1.0f, 1.0f};
        start_game.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        World().AddComponent(start_game_button_entity, start_game);
        m_scene_entities.push_back(start_game_button_entity);

        const auto quit_button_entity = World().CreateEntity("QuitButton");
        const auto quit_pos = pos + glm::vec2(0, 200);
        World().AddComponent(quit_button_entity,
                             Engine::Core::Components::UI::RectTransform(
                                 quit_pos, button_size, pivot, 0.0f, 1));
        auto quit_button = Engine::Core::Components::UI::Button();
        quit_button.button_id = m_quit_button;
        quit_button.enabled = true;
        quit_button.default_color = {0.7f, 0.0f, 0.0f, 1.0f};
        quit_button.highlight_color = {1.0f, 0.0f, 0.0f, 1.0f};
        quit_button.click_color = {1.0f, 1.0f, 1.0f, 1.0f};
        quit_button.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        World().AddComponent(quit_button_entity, quit_button);
        m_scene_entities.push_back(quit_button_entity);
    }
} // namespace
