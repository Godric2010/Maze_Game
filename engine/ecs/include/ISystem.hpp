//
// Created by Sebastian Borsch on 24.08.25.
//

#pragma once
#include "IServiceToEcsProvider.hpp"
#include "World.hpp"

namespace Engine::Core {
    class GameWorld;
}

namespace Engine::Ecs {
#define ECS_SYSTEM(name, phase, tags)

    class ISystem {
        friend class SystemManager;

    public:
        virtual ~ISystem() = default;

        virtual void Initialize() = 0;

        virtual void Run(float delta_time) = 0;

    protected:
        [[nodiscard]] Core::GameWorld *GameWorld() const { return m_game_world; }

    private:
        Core::GameWorld *m_game_world{};
    };

    class IEngineSystem : public ISystem {
        friend class SystemManager;

    public:
        ~IEngineSystem() override = default;

    private:
        IServiceToEcsProvider *m_service_locator{};
        Ecs::World *m_world{};

    protected:
        [[nodiscard]] IServiceToEcsProvider *ServiceLocator() const { return m_service_locator; }
        [[nodiscard]] World* EcsWorld() const {return m_world;}
    };
}
