#include "EngineTestSystem.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Core::Systems {
    EngineTestSystem::EngineTestSystem() {

    }

    EngineTestSystem::~EngineTestSystem() = default;

    void EngineTestSystem::Run(Ecs::World &world, float deltaTime) {
        std::cout << "Running Engine test system..." << std::endl;
    }



} // namespace