#pragma once
#include "../engine/scenemanagement/include/IScene.hpp"
#include "MeshHandler.hpp"
#include "Transform.hpp"
#include "ui/RectTransform.hpp"

namespace Gameplay {
    enum class MenuState {
        Main,
        DifficultySelect,
    };

    class MainMenuScene : public Engine::SceneManagement::IScene {
    public:
        explicit MainMenuScene(MeshHandler *mesh_handler);

        ~MainMenuScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any> &commands) override;

        void OnExit() override;

    private:
        Engine::Ecs::EntityId m_background_entity = Engine::Ecs::INVALID_ENTITY_ID;
        std::vector<Engine::Ecs::EntityId> m_active_state_entities;
        const int m_start_game_button = 1;
        const int m_quit_button = 2;

        const int m_easy_difficulty_button = 1;
        const int m_medium_difficulty_button = 2;
        const int m_hard_difficulty_button = 3;
        const int m_back_button = 4;
        const int m_dev_scene_button = 5;

        MenuState m_menu_state = MenuState::Main;

        glm::vec4 m_button_default_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
        glm::vec4 m_button_highlight_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.3f);
        glm::vec4 m_button_click_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
        glm::vec4 m_button_disabled_color = glm::vec4(0.0f, 0.0f, 0.0f, 0.1f);

        //temp:
        MeshHandler *m_mesh_handler;

        void SetupCamera() const;

        void SwitchUiElements(MenuState new_state);

        Engine::Ecs::EntityId CreateMenuBackground() const;

        Engine::Ecs::EntityId CreateMenuText(const std::string &content, const std::string &font_name, int font_size,
                                             glm::vec2 pos,
                                             glm::vec2 size, Engine::Ecs::EntityId parent_entity);

        Engine::Ecs::EntityId CreateMenuButton(const std::string &name, uint32_t button_id, glm::vec2 pos,
                                               const std::string &content, Engine::Ecs::EntityId parent_entity);

        void CreateMainMenuUiElements();

        void EvaluateMainMenuUiElementCommands(uint32_t button_id);

        void CreateDifficultyUiElements();

        void EvaluateDifficultyUiElementCommands(uint32_t button_id);
    };
} // namespace
