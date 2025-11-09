#include "GameEndScene.hpp"

#include "Transform.hpp"
#include "commands/ui/ButtonClickedCommand.hpp"
#include "ui/Button.hpp"
#include "ui/Image.hpp"
#include "ui/RectTransform.hpp"

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

    void GameEndScene::EvaluateSystemCommands(const std::vector<std::any> &commands) {
        for (const std::any &command: commands) {
            if (command.type() == typeid(Engine::Core::Commands::UI::ButtonClickedCommand)) {
                auto button_clicked = std::any_cast<Engine::Core::Commands::UI::ButtonClickedCommand>(command);
                const auto button_id = button_clicked.GetButtonId();
                if (button_id == m_back_to_main_menu_button_id) {
                    SceneManager().LoadScene("MainMenu", Engine::Core::SceneArgs{.payload = m_mesh_handler});
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

    void GameEndScene::CreateVictoryUiElements() const {
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

        constexpr auto button_size = glm::vec2(200, 100);
        const auto main_menu_button = World().CreateEntity("MainMenuButton");
        const auto pos = glm::vec2(screen.width / 2, screen.height / 2 + 200);
        constexpr auto pivot = glm::vec2(0.5f, 0.5f);
        auto main_menu_button_rect = Engine::Core::Components::UI::RectTransform(pos, button_size, pivot, 0.0f, 1);
        World().AddComponent(main_menu_button, main_menu_button_rect);

        auto main_menu = Engine::Core::Components::UI::Button();
        main_menu.button_id = m_back_to_main_menu_button_id;
        main_menu.enabled = true;
        main_menu.default_color = {0.0f, 0.7f, 0.0f, 1.0f};
        main_menu.highlight_color = {0.0f, 1.0f, 0.0f, 1.0f};
        main_menu.click_color = {1.0f, 1.0f, 1.0f, 1.0f};
        main_menu.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        World().AddComponent(main_menu_button, main_menu);
    }
} // namespace
