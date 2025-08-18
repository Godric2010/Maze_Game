//
// Created by Sebastian Borsch on 18.08.25.
//

#pragma once
#include "../src/EntityManager.hpp"

namespace Engine::Ecs {

    class IWorld {
    public:
        virtual ~IWorld() = default;

        virtual EntityId CreateEntity() = 0;

        virtual void DestroyEntity(EntityId entity) = 0;
    };
}
