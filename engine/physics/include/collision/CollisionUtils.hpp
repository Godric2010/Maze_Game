//
// Created by Sebastian Borsch on 27.10.25.
//

#pragma once
#include <glm/vec3.hpp>

#include "ColliderCache.hpp"
#include "math/Overlap.hpp"
#include "math/Types.hpp"

namespace Engine::Physics::Collision {
    inline bool CheckOverlapSphereWithBox(const BoxColliderInfo &box_collider, const glm::vec3 &position,
                                          const float radius) {
        const auto sphere = Math::Sphere{.center = position, .radius = radius};
        return Overlap(sphere, box_collider.world_obb);
    }

    inline bool CheckOverlapSphereWithSphere(const SphereColliderInfo &sphere_collider, const glm::vec3 &position,
                                             const float radius) {
        const auto sphere = Math::Sphere{.center = position, .radius = radius};
        return Math::Overlap(sphere, sphere_collider.world_sphere);
    }
}
