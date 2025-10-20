//
// Created by Sebastian Borsch on 29.09.25.
//

#pragma once
#include <unordered_map>
#include <unordered_set>

#include "../math/Types.hpp"
#include "../../../ecs/src/Entity.hpp"

namespace Engine::Physics::Collision {
    struct ColliderCache {
        std::unordered_map<Ecs::EntityId, Math::AABB> box_colliders;
        std::unordered_map<Ecs::EntityId, Math::OBB> box_obbs;
        std::unordered_map<Ecs::EntityId, Math::Sphere> sphere_colliders;

        std::unordered_set<Ecs::EntityId> static_colliders;
    };
}
