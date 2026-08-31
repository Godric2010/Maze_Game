#pragma once
#include "IScene.hpp"
#include "ui/RectTransform.hpp"

namespace gameplay {
    enum class MenuState {
        Main,
        DifficultySelect,
    };

    class MainMenuScene : public yarep::scene_management::IScene {
    public:
        explicit MainMenuScene();

        ~MainMenuScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any> &commands) override;

        void OnExit() override;

    private:
        yarep::ecs::EntityId m_background_entity = yarep::ecs::INVALID_ENTITY_ID;
        std::vector<yarep::ecs::EntityId> m_active_state_entities;
        const int m_start_game_button = 1;
        const int m_quit_button = 2;

        const int m_easy_difficulty_button = 1;
        const int m_medium_difficulty_button = 2;
        const int m_hard_difficulty_button = 3;
        const int m_back_button = 4;
        const int m_dev_scene_button = 5;

        MenuState m_menu_state = MenuState::Main;

        glm::vec4 m_button_default_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.1f);
        glm::vec4 m_button_highlight_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.3f);
        glm::vec4 m_button_click_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
        glm::vec4 m_button_disabled_color = glm::vec4(0.0f, 0.0f, 0.0f, 0.1f);

        void SetupCamera() const;

        void SwitchUiElements(MenuState new_state);

        yarep::ecs::EntityId CreateMenuBackground() const;

        yarep::ecs::EntityId CreateMenuText(const std::string &content, const std::string &font_name, int font_size,
                                             glm::vec2 pos,
                                             glm::vec2 size, yarep::ecs::EntityId parent_entity);

        yarep::ecs::EntityId CreateMenuButton(const std::string &name, uint32_t button_id, glm::vec2 pos,
                                               const std::string &content, yarep::ecs::EntityId parent_entity);

        void CreateMainMenuUiElements();

        void EvaluateMainMenuUiElementCommands(uint32_t button_id);

        void CreateDifficultyUiElements();

        void EvaluateDifficultyUiElementCommands(uint32_t button_id);
    };
} // namespace
