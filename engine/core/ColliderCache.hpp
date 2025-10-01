//
// Created by Sebastian Borsch on 29.09.25.
//

#pragma once
#include <unordered_map>
#include <unordered_set>

#include "Types.hpp"
#include "../ecs/src/Entity.hpp"

namespace Engine::Core {
    struct ColliderCache {
        std::unordered_map<Ecs::EntityId, Physics::AABB> box_colliders;
        std::unordered_map<Ecs::EntityId, Physics::Sphere> sphere_colliders;

        std::unordered_set<Ecs::EntityId> static_colliders;
    };
}
