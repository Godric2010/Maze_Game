#pragma once
#include "ISystem.hpp"

namespace Gameplay::Systems {
    ECS_SYSTEM(ExitSystem, Update, [])

    class ExitSystem final : public Engine::Ecs::ISystem {
    public:
        ExitSystem() = default;

        ~ExitSystem() override = default;

        void Initialize(Engine::Ecs::World *world, Engine::Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Engine::Ecs::World &world, float delta_time) override;

    private:
        static void CheckIfPlayerHasKeyToExit(Engine::Ecs::World *world, Engine::Ecs::EntityId target_entity,
                                              Engine::Ecs::EntityId potential_exit_entity);
    };
} // namespace
