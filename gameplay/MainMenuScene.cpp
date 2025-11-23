#include "MainMenuScene.hpp"

#include "Camera.hpp"
#include "GameScene.hpp"
#include "ui/Text.hpp"


namespace Gameplay {
    MainMenuScene::MainMenuScene(MeshHandler* mesh_handler) {
        m_mesh_handler = mesh_handler;
    }

    MainMenuScene::~MainMenuScene() {
        m_mesh_handler = nullptr;
    }

    void MainMenuScene::OnStart() {
        std::cout << "MainMenuScene::OnStart" << std::endl;
        SetupCamera();
        m_background_entity = CreateMenuBackground();
        Input().EnableInputMap("UIInputMap");
        Input().SetMouseVisibility(true);

        SwitchUiElements(MenuState::Main);
    }

    void MainMenuScene::EvaluateSystemCommands(const std::vector<std::any>& commands) {
        for (const std::any& command: commands) {
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
        for (auto& active_element: m_active_state_entities) {
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

    Engine::Ecs::EntityId MainMenuScene::CreateMenuBackground() const {
        const auto bg_entity = World().CreateEntity("MenuBackground");
        const auto screen = Screen();
        const auto bg_position = glm::vec2(screen.width / 2.0f, screen.height / 2.0f);
        const auto bg_size = glm::vec2(screen.width, screen.height);
        constexpr auto bg_pivot = glm::vec2(0.5f, 0.5f);
        const auto bg_rect_transform = Engine::Core::Components::UI::RectTransform()
                .SetPosition(bg_position)
                .SetSize(bg_size)
                .SetPivot(bg_pivot);
        World().AddComponent(bg_entity, bg_rect_transform);
        constexpr auto bg_image = Engine::Core::Components::UI::Image{.color = {0.2, 0.4, 0.2, 1.0}};
        World().AddComponent(bg_entity, bg_image);
        return bg_entity;
    }

    Engine::Ecs::EntityId MainMenuScene::CreateMenuText(const std::string& content, const std::string& font_name,
                                                        const int font_size,
                                                        const glm::vec2 pos, const glm::vec2 size,
                                                        const Engine::Ecs::EntityId parent_entity) {
        const auto text_entity = World().CreateEntity("MenuText_" + content);
        const auto text_transform = Engine::Core::Components::UI::RectTransform()
                .SetPosition(pos)
                .SetSize(size)
                .SetAnchor(Engine::Core::Components::UI::Anchor::Center)
                .SetPivot(glm::vec2{0.5f, 0.0f})
                .SetParent(parent_entity);
        const auto text = Engine::Core::Components::UI::Text(content, font_name, font_size);
        World().AddComponent(text_entity, text_transform);
        World().AddComponent(text_entity, text);
        m_active_state_entities.push_back(text_entity);
        return text_entity;
    }

    Engine::Ecs::EntityId MainMenuScene::CreateMenuButton(const std::string& name, uint32_t button_id, glm::vec2 pos,
                                                          const std::string& content,
                                                          Engine::Ecs::EntityId parent_entity) {
        constexpr auto button_size = glm::vec2(200, 70);
        const auto button_entity = World().CreateEntity(name);
        constexpr auto pivot = glm::vec2(0.5f, 0.5f);
        auto resume_rect = Engine::Core::Components::UI::RectTransform()
                .SetPosition(pos)
                .SetSize(button_size)
                .SetPivot(pivot)
                .SetAnchor(Engine::Core::Components::UI::Anchor::Center)
                .SetParent(parent_entity);
        World().AddComponent(button_entity, resume_rect);

        auto button = Engine::Core::Components::UI::Button();
        button.button_id = button_id;
        button.enabled = true;
        button.default_color = m_button_default_color;
        button.highlight_color = m_button_highlight_color;
        button.click_color = m_button_click_color;
        button.disabled_color = m_button_disabled_color;
        World().AddComponent(button_entity, button);
        m_active_state_entities.push_back(button_entity);

        CreateMenuText(content, "SpaceFont.ttf", 32, glm::vec2(0, 10), glm::vec2(1, 1), button_entity);
        return button_entity;
    }

    void MainMenuScene::CreateMainMenuUiElements() {
        CreateMenuText("Space Maze",
                       "SpaceFont.ttf",
                       128,
                       glm::vec2(0.0f, -300),
                       glm::vec2(1, 1),
                       m_background_entity
                );

        auto position = glm::vec2(0.0f, -100.0f);
        CreateMenuButton("StartGameButton", m_start_game_button, position, "Start", m_background_entity);

        position = glm::vec2(0.0f, 100.0f);
        CreateMenuButton("QuitGameButton", m_quit_button, position, "Quit", m_background_entity);
    }

    void MainMenuScene::EvaluateMainMenuUiElementCommands(const uint32_t button_id) {
        if (button_id == m_start_game_button) {
            SwitchUiElements(MenuState::DifficultySelect);
        } else if (button_id == m_quit_button) {
            Application().Quit();
        }
    }

    void MainMenuScene::CreateDifficultyUiElements() {
        CreateMenuButton("EasyDifficultyButton",
                         m_easy_difficulty_button,
                         glm::vec2(-300, 0.0f),
                         "Easy",
                         m_background_entity
                );
        CreateMenuButton("MediumDifficultyButton",
                         m_medium_difficulty_button,
                         glm::vec2(0.0f, 0.0f),
                         "Medium",
                         m_background_entity
                );
        CreateMenuButton("HardDifficultyButton",
                         m_hard_difficulty_button,
                         glm::vec2(300, 0.0f),
                         "Hard",
                         m_background_entity
                );
        CreateMenuButton("BackButton", m_back_button, glm::vec2(-300, 300), "<-", m_background_entity);
        CreateMenuButton("DevSceneButton", m_dev_scene_button, glm::vec2(300, 300), "Dev", m_background_entity);
    }

    void MainMenuScene::EvaluateDifficultyUiElementCommands(const uint32_t button_id) {
        if (button_id == m_easy_difficulty_button) {
            SceneManager().LoadScene("Game",
                                     Engine::Core::SceneArgs{
                                         .payload = GameSceneSettings{
                                             .mesh_handler = m_mesh_handler,
                                             .difficulty = Difficulty::Easy
                                         }
                                     }
                    );
        } else if (button_id == m_medium_difficulty_button) {
            SceneManager().LoadScene("Game",
                                     Engine::Core::SceneArgs{
                                         .payload = GameSceneSettings{
                                             .mesh_handler = m_mesh_handler,
                                             .difficulty = Difficulty::Medium
                                         }
                                     }
                    );
        } else if (button_id == m_hard_difficulty_button) {
            SceneManager().LoadScene("Game",
                                     Engine::Core::SceneArgs{
                                         .payload = GameSceneSettings{
                                             .mesh_handler = m_mesh_handler,
                                             .difficulty = Difficulty::Hard
                                         }
                                     }
                    );
        } else if (button_id == m_back_button) {
            SwitchUiElements(MenuState::Main);
        } else if (button_id == m_dev_scene_button) {
            SceneManager().LoadScene("Game",
                                     Engine::Core::SceneArgs{
                                         .payload = GameSceneSettings{
                                             .mesh_handler = m_mesh_handler,
                                             .difficulty = Difficulty::Developer
                                         }
                                     }
                    );
        }
    }
} // namespace
