#pragma once
#include <chrono>

#include "../engine/scenemanagement/include/IScene.hpp"
#include "mazegenerator/MazeBuilder.hpp"
#include "../engine/components/Transform.hpp"

namespace Gameplay {
    enum class Difficulty : int {
        Developer = 0,
        Easy = 1,
        Medium = 2,
        Hard = 3,
    };

    struct GameSceneSettings {
        MeshHandler* mesh_handler;
        Difficulty difficulty;
    };

    class GameScene : public Engine::SceneManagement::IScene {
    public:
        explicit GameScene(GameSceneSettings settings);

        ~GameScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any>& commands) override;

        void OnExit() override;

    private:
        std::unique_ptr<Mazegenerator::MazeBuilder> m_maze_builder;
        MeshHandler* m_mesh_handler;
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

        void CreatePauseUiOverlay() ;

        void CreateUiButton(const glm::vec2& position, const glm::vec2& size, const std::string& content, int button_id,
                            const Engine::Ecs::EntityId& parent_entity);
    };
} // namespace
