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
#include "commands/QuitCommand.hpp"
#include "commands/ui/ButtonClickedCommand.hpp"

namespace Gameplay {
    class GameScene : public Engine::Core::IScene {
    public:
        explicit GameScene(MeshHandler *mesh_handler);

        ~GameScene() override;

        void OnStart() override;

        void EvaluateSystemCommands(const std::vector<std::any> &commands) override;

        void OnExit() override;

    private:
        std::unique_ptr<Mazegenerator::MazeBuilder> m_maze_builder;
        MeshHandler *m_mesh_handler;

        std::vector<Engine::Ecs::EntityId> m_pause_entities;

        bool m_is_paused;

        void CreateMaze();

        void Pause() const;

        void Resume() const;

        void CreateCamera() const;

        void CreateIngameUiOverlay() const;

        void CreatePauseUiOverlay() const;
    };
} // namespace
