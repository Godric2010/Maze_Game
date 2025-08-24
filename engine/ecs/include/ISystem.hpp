//
// Created by Sebastian Borsch on 24.08.25.
//

#pragma once
#include "World.hpp"

namespace Engine::Ecs {
#define ECS_SYSTEM(name, phase, tags)

    class ISystem {
    public:

        virtual ~ISystem() = default;

        virtual void Run(World& world, float deltaTime) = 0;
    };
}
