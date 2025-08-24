#include "TestSystem.hpp"

#include <iostream>
#include <ostream>

namespace Gameplay {
    TestSystem::TestSystem() {

    }

    TestSystem::~TestSystem() {

    }

    void TestSystem::Run(Engine::Ecs::World &world, float deltaTime) {
        std::cout << "Running test system..." << std::endl;
    }


} // namespace