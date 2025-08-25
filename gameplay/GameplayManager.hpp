#pragma once
#include <EngineController.hpp>

namespace Gameplay {
    class GameplayManager {
    public:
        GameplayManager(Engine::Core::EngineController* engineController);

        ~GameplayManager();

        void Initialize();

        void Shutdown();
    private:
        Engine::Core::EngineController* EngineController;

        Engine::Ecs::EntityId m_cameraEntity;
        std::vector<Engine::Ecs::EntityId> m_objects;

        void createCamera();
        void createObjects();
    };
} // namespace