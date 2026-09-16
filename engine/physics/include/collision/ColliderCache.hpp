//
// Created by Sebastian Borsch on 29.09.25.
//

#pragma once
#include <unordered_map>
#include <unordered_set>

#include "Types.hpp"
#include "../../../ecs/src/Entity.hpp"

namespace yarep::physics::collision {

    struct BoxColliderInfo {
        AABB world_box;
        OBB world_obb;
        bool is_static;
        bool is_trigger;
    };

    struct SphereColliderInfo {
        Sphere world_sphere;
        bool is_static;
        bool is_trigger;
    };

    struct ColliderCache {
        std::unordered_map<ecs::EntityId, BoxColliderInfo> box_colliders;
        std::unordered_map<ecs::EntityId, SphereColliderInfo> sphere_colliders;
    };
}
