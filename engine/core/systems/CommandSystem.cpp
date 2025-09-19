#include "CommandSystem.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Core::Systems {
    void CommandSystem::SetServices(Ecs::IServiceToEcsProvider *service_locator) {
    }

    void CommandSystem::Run(Ecs::World &world, float delta_time) {
        std::cout << "Apply commands..." << std::endl;
        world.ApplyCommands();
    }
} // namespace
