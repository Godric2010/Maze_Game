#pragma once
#include "IScene.hpp"
#include "MeshHandler.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "commands/ui/ButtonClickedCommand.hpp"
#include "ui/Button.hpp"
#include "ui/Image.hpp"
#include "ui/RectTransform.hpp"

namespace Gameplay {
    class MainMenuScene : public Engine::Core::IScene {
    public:
        explicit MainMenuScene(MeshHandler *mesh_handler);

        ~MainMenuScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any> &commands) override;

        void OnExit() override;

    private:
        std::vector<Engine::Ecs::EntityId> m_scene_entities;
        const int m_start_game_button = 1;
        const int m_quit_button = 2;

        //temp:
        MeshHandler *m_mesh_handler;

        void SetupCamera();

        void CreateMainMenuUiElements();
    };
} // namespace
