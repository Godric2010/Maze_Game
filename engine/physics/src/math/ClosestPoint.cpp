#include "../../include/math/ClosestPoint.hpp"

namespace Engine::Physics::Math {
    glm::vec3 ClosestPoint(const glm::vec3 &point, const AABB &box) noexcept {
        return clamp(point, box.min, box.max);
    }

    glm::vec3 ClosestPoint(const glm::vec3 &point, const OBB &box) noexcept {
        const glm::mat3 matrix = box.orientation;
        const glm::vec3 distance = point - box.center;
        const glm::vec3 local = transpose(matrix) * distance;

        const glm::vec3 clamped = clamp(local, -box.half_extents, box.half_extents);
        return box.center + matrix * clamped;
    }

} // namespace
