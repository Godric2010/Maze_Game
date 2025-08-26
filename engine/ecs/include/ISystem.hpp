//
// Created by Sebastian Borsch on 24.08.25.
//

#pragma once
#include "IServiceToEcsProvider.hpp"
#include "World.hpp"

namespace Engine::Ecs {
#define ECS_SYSTEM(name, phase, tags)

    class ISystem {
    public:
        virtual ~ISystem() = default;

        virtual void SetServices(IServiceToEcsProvider* serviceLocator) = 0;

        virtual void Run(World& world, float deltaTime) = 0;
    };
}
