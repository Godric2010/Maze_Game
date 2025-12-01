#pragma once
#include "../engine/scenemanagement/include/IScene.hpp"
#include "MeshHandler.hpp"
#include "Camera.hpp"

namespace Gameplay {
    struct GameEndShowData {
        MeshHandler *mesh_handler;
        float time_to_completion;
    };

    class GameEndScene : public Engine::SceneManagement::IScene {
    public:
        explicit GameEndScene(GameEndShowData game_end_show_data);

        ~GameEndScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any> &commands) override;

        void OnExit() override;

    private:
        MeshHandler *m_mesh_handler;
        float m_time_to_completion;

        const int m_back_to_main_menu_button_id = 1;
        void SetupCamera() const;
        void CreateVictoryUiElements() const;
    };
} // namespace
