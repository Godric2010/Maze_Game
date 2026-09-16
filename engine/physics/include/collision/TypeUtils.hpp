//
// Created by Sebastian Borsch on 07.10.25.
//

#pragma once
#include "stdexcept"


namespace yarep::physics::collision::util {
    inline geometry::OBB BuildWorldObb(const math::Vec3& position, const math::Quaternion& rotation, const float width,
                                       const float height, const float depth) {
        geometry::OBB obb{};
        obb.center = position;

        const auto half_extents = math::Vec3(0.5f * width, 0.5f * height, 0.5f * depth);
        obb.half_extents = half_extents;
        obb.rotation = rotation;
        return obb;
    }


    inline geometry::AABB FromSphere(const geometry::Sphere& sphere) {
        if (sphere.radius <= 0.0f) {
            throw std::invalid_argument("Sphere radius must be greater than 0.0");
        }

        const math::Vec3 r{sphere.radius, sphere.radius, sphere.radius};
        return {sphere.center - r, sphere.center + r};
    }
}
