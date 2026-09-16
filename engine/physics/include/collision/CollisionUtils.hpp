//
// Created by Sebastian Borsch on 27.10.25.
//

#pragma once

#include "ColliderCache.hpp"
#include "Intersects.hpp"

namespace yarep::physics::collision {
    inline bool CheckOverlapSphereWithBox(const BoxColliderInfo& box_collider, const math::Vec3& position,
                                          const float radius) {
        const auto sphere = geometry::Sphere{position, radius};
        return geometry::intersects(sphere, box_collider.world_obb);
    }

    inline bool CheckOverlapSphereWithSphere(const SphereColliderInfo& sphere_collider, const math::Vec3& position,
                                             const float radius) {
        const auto sphere = geometry::Sphere{position, radius};
        return geometry::intersects(sphere, sphere_collider.world_sphere);
    }
}
