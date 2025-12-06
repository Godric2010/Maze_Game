#include "GameEndScene.hpp"

#include "../engine/components/Transform.hpp"
#include "commands/ui/ButtonClickedCommand.hpp"
#include "../components/ui/Button.hpp"
#include "../components/ui/Image.hpp"
#include "../components/ui/RectTransform.hpp"
#include "../components/ui/Text.hpp"

namespace Gameplay {
    GameEndScene::GameEndScene(const GameEndShowData game_end_show_data) {
        m_mesh_handler = game_end_show_data.mesh_handler;
        m_time_to_completion = game_end_show_data.time_to_completion;
    }

    GameEndScene::~GameEndScene() {
        m_mesh_handler = nullptr;
    }

    void GameEndScene::OnStart() {
        std::cout << "GameEndScene::OnStart" << std::endl;
        SetupCamera();
        CreateVictoryUiElements();
        Input().EnableInputMap("UIInputMap");
        Input().SetMouseVisibility(true);

        std::cout << "Time until level complete: " << m_time_to_completion << std::endl;
    }

    void GameEndScene::EvaluateSystemCommands(const std::vector<std::any>& commands) {
        for (const std::any& command: commands) {
            if (command.type() == typeid(Engine::Core::Commands::UI::ButtonClickedCommand)) {
                auto button_clicked = std::any_cast<Engine::Core::Commands::UI::ButtonClickedCommand>(command);
                const auto button_id = button_clicked.GetButtonId();
                if (button_id == m_back_to_main_menu_button_id) {
                    SceneManager().LoadScene("MainMenu", Engine::SceneManagement::SceneArgs{.payload = m_mesh_handler});
                }
            }
        }
    }

    void GameEndScene::OnExit() {
        std::cout << "GameEndScene::OnExit" << std::endl;
        Input().DisableInputMap("UIInputMap");
        Input().SetMouseVisibility(false);
    }

    void GameEndScene::SetupCamera() const {
        const auto camera_entity = World().CreateEntity("MainCamera");
        const auto [width, height, aspect_ratio] = Screen();
        const auto camera_component = Engine::Components::Camera{
            .Width = width,
            .Height = height,
            .FieldOfView = 60,
            .AspectRatio = aspect_ratio,
            .NearClip = 0.01f,
            .FarClip = 1000.0f,

        };
        World().AddComponent<Engine::Components::Camera>(camera_entity, camera_component);

        const auto camera_transform = Engine::Components::Transform();
        World().AddComponent(camera_entity, camera_transform);
    }

    void GameEndScene::CreateVictoryUiElements() const {
        const auto bg_entity = World().CreateEntity("MenuBackground");
        const auto screen = Screen();
        const auto bg_position = glm::vec2(screen.width / 2.0f, screen.height / 2.0f);
        const auto bg_size = glm::vec2(screen.width, screen.height);
        constexpr auto bg_pivot = glm::vec2(0.5f, 0.5f);
        const auto bg_rect_transform = Engine::Components::UI::RectTransform()
                .SetPosition(bg_position)
                .SetSize(bg_size)
                .SetPivot(bg_pivot);
        World().AddComponent(bg_entity, bg_rect_transform);
        constexpr auto bg_image = Engine::Components::UI::Image{.color = {0.2, 0.4, 0.2, 1.0}};
        World().AddComponent(bg_entity, bg_image);

        const auto heading_entity = World().CreateEntity("Heading");
        const auto heading_transform = Engine::Components::UI::RectTransform()
                .SetPosition(glm::vec2(0.0f, 300))
                .SetPivot(glm::vec2(0.5f, 0.0f))
                .SetAnchor(Engine::Components::UI::Anchor::TopCenter)
                .SetParent(bg_entity);
        const auto heading_text = Engine::Components::UI::Text()
                .SetText("Congratulations!")
                .SetFontName("SpaceFont.ttf")
                .SetFontSize(128.0f);
        World().AddComponent(heading_entity, heading_text);
        World().AddComponent(heading_entity, heading_transform);


        const auto sub_heading_entity = World().CreateEntity("SubHeading");
        const auto sub_heading_transform = Engine::Components::UI::RectTransform()
                .SetPosition(glm::vec2(0.0f, 420.0f))
                .SetPivot(glm::vec2(0.5f, 0.0f))
                .SetAnchor(Engine::Components::UI::Anchor::TopCenter)
                .SetParent(bg_entity);
        const auto sub_heading_text = Engine::Components::UI::Text()
                .SetText("You escaped the maze!")
                .SetFontName("SpaceFont.ttf")
                .SetFontSize(48.0f);
        World().AddComponent(sub_heading_entity, sub_heading_text);
        World().AddComponent(sub_heading_entity, sub_heading_transform);

        const float completed_in_seconds = m_time_to_completion / 1000.0f;
        const float minutes = (completed_in_seconds / 60.0f);
        const float seconds = (completed_in_seconds - (static_cast<int>(minutes) * 60));
        const std::string time_needed_str = "Time needed: " + std::to_string(static_cast<int>(minutes)) + ":"
                                            + std::to_string(static_cast<int>(seconds));
        const auto time_display_entity = World().CreateEntity("TimeDisplay");
        const auto time_display_transform = Engine::Components::UI::RectTransform()
                .SetPosition(glm::vec2(0.0f, 550.0f))
                .SetPivot(glm::vec2(0.5f, 0.0f))
                .SetAnchor(Engine::Components::UI::Anchor::TopCenter)
                .SetParent(bg_entity);
        const auto time_display_text = Engine::Components::UI::Text()
                .SetText(time_needed_str)
                .SetFontName("SpaceFont.ttf")
                .SetFontSize(32.0f);
        World().AddComponent(time_display_entity, time_display_text);
        World().AddComponent(time_display_entity, time_display_transform);

        constexpr auto button_size = glm::vec2(200, 70);
        const auto main_menu_button = World().CreateEntity("MainMenuButton");
        const auto pos = glm::vec2(0, 900);
        constexpr auto pivot = glm::vec2(0.5f, 0.5f);
        auto main_menu_button_rect = Engine::Components::UI::RectTransform()
                .SetPosition(pos)
                .SetSize(button_size)
                .SetPivot(pivot)
                .SetAnchor(Engine::Components::UI::Anchor::TopCenter)
                .SetParent(bg_entity);
        World().AddComponent(main_menu_button, main_menu_button_rect);

        auto main_menu = Engine::Components::UI::Button();
        main_menu.button_id = m_back_to_main_menu_button_id;
        main_menu.enabled = true;
        main_menu.default_color = {1.0f, 1.0f, 1.0f, 0.0f};
        main_menu.highlight_color = {1.0f, 1.0f, 1.0f, 0.3f};
        main_menu.click_color = {1.0f, 1.0f, 1.0f, 0.8f};
        main_menu.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        World().AddComponent(main_menu_button, main_menu);

        const auto menu_button_text_entity = World().CreateEntity("MenuButton");
        const auto menu_button_text_transform = Engine::Components::UI::RectTransform()
                .SetPosition(glm::vec2(0, 10))
                .SetPivot(glm::vec2(0.5f, 0.0f))
                .SetAnchor(Engine::Components::UI::Anchor::Center)
                .SetParent(main_menu_button);
        const auto menu_button_text = Engine::Components::UI::Text()
                .SetText("Main Menu")
                .SetFontName("SpaceFont.ttf")
                .SetFontSize(32.0f);
        World().AddComponent(menu_button_text_entity, menu_button_text_transform);
        World().AddComponent(menu_button_text_entity, menu_button_text);
    }
} // namespace
