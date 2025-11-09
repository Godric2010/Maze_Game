#pragma once
#include "IScene.hpp"
#include "mazegenerator/MazeBuilder.hpp"
#include "Transform.hpp"
#include "ui/RectTransform.hpp"
#include "ui/Image.hpp"
#include "ui/Button.hpp"
#include "MotionIntent.hpp"
#include "Collider.hpp"
#include "components/Inventory.hpp"
#include "commands/PauseCommand.hpp"
#include "commands/LevelFinished.hpp"
#include "commands/ui/ButtonClickedCommand.hpp"

namespace Gameplay {
    enum class Difficulty : int {
        Developer = 0,
        Easy = 1,
        Medium = 2,
        Hard = 3,
    };

    struct GameSceneSettings {
        MeshHandler *mesh_handler;
        Difficulty difficulty;
    };

    class GameScene : public Engine::Core::IScene {
    public:
        explicit GameScene(GameSceneSettings settings);

        ~GameScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any> &commands) override;

        void OnExit() override;

    private:
        std::unique_ptr<Mazegenerator::MazeBuilder> m_maze_builder;
        MeshHandler *m_mesh_handler;
        Difficulty m_difficulty;


        std::chrono::time_point<std::chrono::steady_clock> m_start_time;
        float m_time_passed = 0;

        std::vector<Engine::Ecs::EntityId> m_pause_entities;

        bool m_is_paused;

        void CreateMaze();

        void Pause();

        void Resume();

        void CreateCamera() const;

        void CreateIngameUiOverlay() const;

        void CreatePauseUiOverlay() const;
    };
} // namespace
