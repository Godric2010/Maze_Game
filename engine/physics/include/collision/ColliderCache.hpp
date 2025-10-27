//
// Created by Sebastian Borsch on 29.09.25.
//

#pragma once
#include <unordered_map>
#include <unordered_set>

#include "../math/Types.hpp"
#include "../../../ecs/src/Entity.hpp"

namespace Engine::Physics::Collision {

    struct BoxColliderInfo {
        Math::AABB world_box;
        Math::OBB world_obb;
        bool is_static;
        bool is_trigger;
    };

    struct SphereColliderInfo {
        Math::Sphere world_sphere;
        bool is_static;
        bool is_trigger;
    };

    struct ColliderCache {
        std::unordered_map<Ecs::EntityId, BoxColliderInfo> box_colliders;
        std::unordered_map<Ecs::EntityId, SphereColliderInfo> sphere_colliders;
    };
}
