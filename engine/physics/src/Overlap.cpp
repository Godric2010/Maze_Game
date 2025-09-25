#include "../include/Overlap.hpp"

namespace Engine::Physics {
    bool Overlap(const AABB &bounds_a, const AABB &bounds_b) noexcept {
        if (bounds_a.max.x < bounds_b.min.x || bounds_a.min.x > bounds_b.max.x) return false;
        if (bounds_a.max.y < bounds_b.min.y || bounds_a.min.y > bounds_b.max.y) return false;
        if (bounds_a.max.z < bounds_b.min.z || bounds_a.min.z > bounds_b.max.z) return false;

        return true;
    }

    bool Overlap(const Sphere &sphere, const AABB &bounding_box) noexcept {
        const glm::vec3 closest = ClosestPoint(sphere.center, bounding_box);
        const float delta_dist = length2(closest - sphere.center);
        return delta_dist <= sphere.radius * sphere.radius;
    }

    static glm::vec3 ToLocal(const glm::vec3 &point, const OBB &box) noexcept {
        const glm::vec3 delta = point - box.center;
        const glm::mat3 rotation_mat = box.orientation;
        return glm::transpose(rotation_mat) * delta;
    }

    bool Overlap(const Sphere &sphere, const OBB &obb) noexcept {
        const glm::vec3 local_point = ToLocal(sphere.center, obb);
        const AABB local_box{-obb.half_extents, obb.half_extents};
        const glm::vec3 closest = glm::clamp(local_point, local_box.min, local_box.max);
        const glm::vec3 diff = closest - local_point;
        return length2(diff) <= sphere.radius * sphere.radius;
    }
} // namespace
