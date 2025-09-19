#pragma once
#include "ISystem.hpp"

namespace Engine::Core::Systems {
    ECS_SYSTEM(PhysicsSystem, Physics, [])

    class PhysicsSystem final : public Ecs::ISystem{
    public:
        PhysicsSystem();

        ~PhysicsSystem() override = default;

        void SetServices(Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Ecs::World &world, float delta_time) override;

    private:

    };
} // namespace
