//
// Created by Sebastian Borsch on 07.10.25.
//

#pragma once

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

}
