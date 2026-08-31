#pragma once
#include "IScene.hpp"

namespace gameplay {
    struct GameEndShowData {
        float time_to_completion;
    };

    class GameEndScene : public yarep::SceneManagement::IScene {
    public:
        explicit GameEndScene(GameEndShowData game_end_show_data);

        ~GameEndScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any> &commands) override;

        void OnExit() override;

    private:
        float m_time_to_completion;

        const int m_back_to_main_menu_button_id = 1;
        void SetupCamera() const;
        void CreateVictoryUiElements() const;
    };
} // namespace
