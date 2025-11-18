#pragma once
#include "Camera.hpp"
#include "Camera.hpp"
#include "IScene.hpp"
#include "MeshHandler.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "commands/ui/ButtonClickedCommand.hpp"
#include "ui/Button.hpp"
#include "ui/Image.hpp"
#include "ui/RectTransform.hpp"

namespace Gameplay {
    enum class MenuState {
        Main,
        DifficultySelect,
    };

    class MainMenuScene : public Engine::Core::IScene {
    public:
        explicit MainMenuScene(MeshHandler *mesh_handler);

        ~MainMenuScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any> &commands) override;

        void OnExit() override;

    private:
        std::vector<Engine::Ecs::EntityId> m_active_state_entities;
        const int m_start_game_button = 1;
        const int m_quit_button = 2;

        const int m_easy_difficulty_button = 1;
        const int m_medium_difficulty_button = 2;
        const int m_hard_difficulty_button = 3;
        const int m_back_button = 4;
        const int m_dev_scene_button = 5;

        MenuState m_menu_state = MenuState::Main;

        //temp:
        MeshHandler *m_mesh_handler;

        void SetupCamera() const;

        void SwitchUiElements(MenuState new_state);

        void CreateMenuBackground() const;

        void CreateMenuText(const std::string &content, const std::string &font_name, int font_size, glm::vec2 pos,
                            glm::vec2 size);

        void CreateMenuButton(const std::string &name, uint32_t button_id, glm::vec2 pos, glm::vec4 color,
                              glm::vec4 highlight_color);

        void CreateMainMenuUiElements();

        void EvaluateMainMenuUiElementCommands(uint32_t button_id);

        void CreateDifficultyUiElements();

        void EvaluateDifficultyUiElementCommands(uint32_t button_id);
    };
} // namespace
