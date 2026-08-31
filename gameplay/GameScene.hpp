#pragma once
#include <chrono>

#include "IScene.hpp"
#include "mazegenerator/MazeBuilder.hpp"

namespace gameplay {
    enum class Difficulty : int {
        Developer = 0,
        Easy = 1,
        Medium = 2,
        Hard = 3,
    };

    struct GameSceneSettings {
        Difficulty difficulty;
    };

    class GameScene : public yarep::scene_management::IScene {
    public:
        explicit GameScene(GameSceneSettings settings);

        ~GameScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any>& commands) override;

        void OnExit() override;

    private:
        std::unique_ptr<maze_generator::MazeBuilder> m_maze_builder;
        Difficulty m_difficulty;


        std::chrono::time_point<std::chrono::steady_clock> m_start_time;
        float m_time_passed = 0;

        std::vector<yarep::ecs::EntityId> m_pause_entities;

        bool m_is_paused;

        void CreateMaze();

        void Pause();

        void Resume();

        void CreateCamera() const;

        void CreateIngameUiOverlay() const;

        void CreatePauseUiOverlay() ;

        void CreateUiButton(const glm::vec2& position, const glm::vec2& size, const std::string& content, int button_id,
                            const yarep::ecs::EntityId& parent_entity);
    };
} // namespace
