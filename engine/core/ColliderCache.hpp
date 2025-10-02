//
// Created by Sebastian Borsch on 29.09.25.
//

#pragma once
#include <unordered_map>
#include <unordered_set>

#include "../physics/include/math/Types.hpp"
#include "../ecs/src/Entity.hpp"

namespace Engine::Core {
    struct ColliderCache {
        std::unordered_map<Ecs::EntityId, Physics::Math::AABB> box_colliders;
        std::unordered_map<Ecs::EntityId, Physics::Math::Sphere> sphere_colliders;

        std::unordered_set<Ecs::EntityId> static_colliders;
    };
}
