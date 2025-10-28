#include "CommandSystem.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Core::Systems {
    void CommandSystem::Initialize() {
    }

    void CommandSystem::Run(float delta_time) {
        EcsWorld()->ApplyCommands();
    }
} // namespace
