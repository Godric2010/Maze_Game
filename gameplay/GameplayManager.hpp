#pragma once
#include <EngineController.hpp>

#include "mazegenerator/MazeAlgorithm.hpp"

namespace Gameplay {
    class GameplayManager {
    public:
        explicit GameplayManager(Engine::Core::IEngine& iEngine);

        ~GameplayManager();

        void Initialize();

        void Shutdown();
    private:
         Engine::Core::IEngine& IEngine;

        Engine::Ecs::EntityId m_cameraEntity;
        std::vector<Engine::Ecs::EntityId> m_objects;
        std::unique_ptr<Mazegenerator::MazeAlgorithm> m_maze_algorithm;

        void createCamera();
        void createObjects() const;
    };
} // namespace