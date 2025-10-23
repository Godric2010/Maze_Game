//
// Created by Sebastian Borsch on 07.10.25.
//

#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "Types.hpp"

namespace Engine::Physics::Math::Util {
    inline OBB BuildWorldObb(const glm::vec3 & position, const glm::vec3 & rotation, const float width,
                             const float height, const float depth) {
        OBB obb{};
        obb.center = position;

        const auto half_extents = glm::vec3(0.5f * width, 0.5f * height, 0.5f * depth);
        obb.half_extents = half_extents;

        const glm::mat4 rot_mat = glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x),
                                                    glm::radians(rotation.z));
        obb.orientation = glm::mat3(rot_mat);
        return obb;
    }

    inline AABB ToTightAabb(const OBB &obb) {
        const glm::mat3 &rot = obb.orientation;
        const glm::vec3 &extents = obb.half_extents;
        const glm::vec3 e = {
            std::abs(rot[0][0]) * extents.x + std::abs(rot[1][0]) * extents.y + std::abs(rot[2][0]) * extents.z,
            std::abs(rot[0][1]) * extents.x + std::abs(rot[1][1]) * extents.y + std::abs(rot[2][1]) * extents.z,
            std::abs(rot[0][2]) * extents.x + std::abs(rot[1][2]) * extents.y + std::abs(rot[2][2]) * extents.z,
        };
        return {obb.center - e, obb.center + e};
    }

    inline AABB FromSphere(const Sphere &sphere) {
        if (sphere.radius <= 0.0f) {
            throw std::invalid_argument("Sphere radius must be greater than 0.0");
        }

        const glm::vec3 r(sphere.radius);
        return {sphere.center - r, sphere.center + r};
    }
}
