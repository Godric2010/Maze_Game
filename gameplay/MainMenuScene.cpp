#include "MainMenuScene.hpp"
#include "GameScene.hpp"
#include "ui/Text.hpp"


namespace Gameplay {
    MainMenuScene::MainMenuScene(MeshHandler *mesh_handler) {
        m_mesh_handler = mesh_handler;
    }

    MainMenuScene::~MainMenuScene() {
        m_mesh_handler = nullptr;
    }

    void MainMenuScene::OnStart() {
        std::cout << "MainMenuScene::OnStart" << std::endl;
        SetupCamera();
        CreateMenuBackground();
        Input().EnableInputMap("UIInputMap");
        Input().SetMouseVisibility(true);

        SwitchUiElements(MenuState::Main);
    }

    void MainMenuScene::EvaluateSystemCommands(const std::vector<std::any> &commands) {
        for (const std::any &command: commands) {
            if (command.type() == typeid(Engine::Core::Commands::UI::ButtonClickedCommand)) {
                auto button_clicked = std::any_cast<Engine::Core::Commands::UI::ButtonClickedCommand>(command);
                const auto button_id = button_clicked.GetButtonId();
                switch (m_menu_state) {
                    case MenuState::Main:
                        EvaluateMainMenuUiElementCommands(button_id);
                        break;
                    case MenuState::DifficultySelect:
                        EvaluateDifficultyUiElementCommands(button_id);
                        break;
                }
            }
        }
    }

    void MainMenuScene::OnExit() {
        std::cout << "MainMenuScene::OnExit" << std::endl;
        Input().DisableInputMap("UIInputMap");
        Input().SetMouseVisibility(false);
    }

    void MainMenuScene::SetupCamera() const {
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
    }

    void MainMenuScene::SwitchUiElements(const MenuState new_state) {
        for (auto &active_element: m_active_state_entities) {
            World().DestroyEntity(active_element);
        }

        switch (new_state) {
            case MenuState::Main:
                CreateMainMenuUiElements();
                break;
            case MenuState::DifficultySelect:
                CreateDifficultyUiElements();
                break;
        }
        m_menu_state = new_state;
    }

    void MainMenuScene::CreateMenuBackground() const {
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
    }

    void MainMenuScene::CreateMenuText(const std::string &content, const std::string &font_name, const int font_size,
                                       const glm::vec2 pos, const glm::vec2 size) const {
        const auto text_entity = World().CreateEntity("MenuText_" + content);
        const auto text_transform = Engine::Core::Components::UI::RectTransform(pos, size, 2);
        const auto text = Engine::Core::Components::UI::Text(content, font_name, font_size);
        World().AddComponent(text_entity, text_transform);
        World().AddComponent(text_entity, text);
    }

    void MainMenuScene::CreateMenuButton(const std::string &name, uint32_t button_id, glm::vec2 pos, glm::vec4 color,
                                         glm::vec4 highlight_color) {
        constexpr auto button_size = glm::vec2(200, 100);
        const auto start_game_button_entity = World().CreateEntity(name);
        constexpr auto pivot = glm::vec2(0.5f, 0.5f);
        auto resume_rect = Engine::Core::Components::UI::RectTransform(pos, button_size, pivot, 0.0f, 1);
        World().AddComponent(start_game_button_entity, resume_rect);

        auto start_game = Engine::Core::Components::UI::Button();
        start_game.button_id = button_id;
        start_game.enabled = true;
        start_game.default_color = color;
        start_game.highlight_color = highlight_color;
        start_game.click_color = {1.0f, 1.0f, 1.0f, 1.0f};
        start_game.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        World().AddComponent(start_game_button_entity, start_game);
        m_active_state_entities.push_back(start_game_button_entity);
    }

    void MainMenuScene::CreateMainMenuUiElements() {
        const auto screen = Screen();

        CreateMenuText("Space Maze", "SpaceFont.ttf", 128, glm::vec2(screen.width / 2.0f, screen.height / 2.0f - 300),
                       glm::vec2(1, 1));

        CreateMenuButton("StartGameButton", m_start_game_button,
                         glm::vec2(screen.width / 2.0f, screen.height / 2.0f - 100), {0.0f, 0.7f, 0.0f, 1.0f},
                         {0.0f, 1.0f, 0.0f, 1.0f});

        CreateMenuButton("QuitGameButton", m_quit_button,
                         glm::vec2(screen.width / 2.0f, screen.height / 2.0f + 100), {0.7f, 0.0f, 0.0f, 1.0f},
                         {1.0f, 0.0f, 0.0f, 1.0f});
    }

    void MainMenuScene::EvaluateMainMenuUiElementCommands(const uint32_t button_id) {
        if (button_id == m_start_game_button) {
            SwitchUiElements(MenuState::DifficultySelect);
        } else if (button_id == m_quit_button) {
            Application().Quit();
        }
    }

    void MainMenuScene::CreateDifficultyUiElements() {
        const auto screen = Screen();
        CreateMenuButton("EasyDifficultyButton", m_easy_difficulty_button,
                         glm::vec2(screen.width / 2.0f - 300, screen.height / 2.0f), {1.0, 0.6f, 0.6f, 1.0f},
                         {1.0, 0.0f, 0.0f, 1.0f});
        CreateMenuButton("MediumDifficultyButton", m_medium_difficulty_button,
                         glm::vec2(screen.width / 2.0f, screen.height / 2.0f), {1.0, 0.4f, 0.4f, 1.0f},
                         {1.0, 0.0f, 0.0f, 1.0f});
        CreateMenuButton("HardDifficultyButton", m_hard_difficulty_button,
                         glm::vec2(screen.width / 2.0f + 300, screen.height / 2.0f), {1.0, 0.2f, 0.2f, 1.0f},
                         {1.0, 0.0f, 0.0f, 1.0f});
        CreateMenuButton("BackButton", m_back_button,
                         glm::vec2(screen.width / 2.0f - 300, screen.height / 2.0f + 300),
                         {0.0, 0.0f, 0.7f, 1.0f}, {0.0, 0.0f, 1.0f, 1.0f});
        CreateMenuButton("DevSceneButton", m_dev_scene_button,
                         glm::vec2(screen.width / 2.0f + 300, screen.height / 2.0f + 300),
                         {0.7, 0.7f, 0.7f, 1.0f}, {1.0, 1.0f, 1.0f, 1.0f});
    }

    void MainMenuScene::EvaluateDifficultyUiElementCommands(const uint32_t button_id) {
        if (button_id == m_easy_difficulty_button) {
            SceneManager().LoadScene("Game", Engine::Core::SceneArgs{
                                         .payload = GameSceneSettings{
                                             .mesh_handler = m_mesh_handler,
                                             .difficulty = Difficulty::Easy
                                         }
                                     });
        } else if (button_id == m_medium_difficulty_button) {
            SceneManager().LoadScene("Game", Engine::Core::SceneArgs{
                                         .payload = GameSceneSettings{
                                             .mesh_handler = m_mesh_handler,
                                             .difficulty = Difficulty::Medium
                                         }
                                     });
        } else if (button_id == m_hard_difficulty_button) {
            SceneManager().LoadScene("Game", Engine::Core::SceneArgs{
                                         .payload = GameSceneSettings{
                                             .mesh_handler = m_mesh_handler,
                                             .difficulty = Difficulty::Hard
                                         }
                                     });
        } else if (button_id == m_back_button) {
            SwitchUiElements(MenuState::Main);
        } else if (button_id == m_dev_scene_button) {
            SceneManager().LoadScene("Game", Engine::Core::SceneArgs{
                                         .payload = GameSceneSettings{
                                             .mesh_handler = m_mesh_handler,
                                             .difficulty = Difficulty::Developer
                                         }
                                     });
        }
    }
} // namespace
