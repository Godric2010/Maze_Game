#pragma once
#include <EngineController.hpp>

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

        void createCamera();
        void createObjects() const;
    };
} // namespace