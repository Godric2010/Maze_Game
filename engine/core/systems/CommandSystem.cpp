#include "CommandSystem.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Core::Systems {
    void CommandSystem::Initialize(Ecs::World *world, Ecs::IServiceToEcsProvider *service_locator) {
    }

    void CommandSystem::Run(Ecs::World &world, float delta_time) {
        world.ApplyCommands();
    }
} // namespace
