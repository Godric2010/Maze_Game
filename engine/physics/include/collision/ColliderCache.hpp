//
// Created by Sebastian Borsch on 29.09.25.
//

#pragma once
#include <unordered_map>
#include <unordered_set>

#include "AABB.hpp"
#include "OBB.hpp"
#include "Sphere.hpp"
#include "Types.hpp"
#include "../../../ecs/src/Entity.hpp"

namespace yarep::physics::collision {

    struct BoxColliderInfo {
        geometry::AABB world_box;
        geometry::OBB world_obb;
        bool is_static;
        bool is_trigger;
    };

    struct SphereColliderInfo {
        geometry::Sphere world_sphere;
        bool is_static;
        bool is_trigger;
    };

    struct ColliderCache {
        std::unordered_map<ecs::EntityId, BoxColliderInfo> box_colliders;
        std::unordered_map<ecs::EntityId, SphereColliderInfo> sphere_colliders;
    };
}
