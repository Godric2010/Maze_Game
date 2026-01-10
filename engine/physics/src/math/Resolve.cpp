#include "../../include/math/Resolve.hpp"


namespace Engine::Physics::Math {
    namespace {
        constexpr float k_epsilon = 1e-6f;
    }

    CollisionHit Penetration(const Sphere& sphere, const AABB& box) noexcept {
        CollisionHit out{};

        const glm::vec3 closest_point = ClosestPoint(sphere.center, box);
        const glm::vec3 delta = sphere.center - closest_point;


        const float distance_squared = glm::length2(delta);
        const float radius_squared = sphere.radius * sphere.radius;

        if (distance_squared > radius_squared) {
            return out;
        }

        out.hit = true;
        out.point = closest_point;
        out.time_of_impact = 0.0f;

        if (distance_squared > k_epsilon) {
            const float distance = sqrt(distance_squared);
            out.normal = delta / distance;
            out.penetration_depth = sphere.radius - distance;
        } else {
            const float dx = std::min(std::abs(sphere.center.x - box.min.x), std::abs(box.max.x - sphere.center.x));
            const float dy = std::min(std::abs(sphere.center.y - box.min.y), std::abs(box.max.y - sphere.center.y));
            const float dz = std::min(std::abs(sphere.center.z - box.min.x), std::abs(box.max.x - sphere.center.z));

            const glm::vec3 box_center = (box.min + box.max) * 0.5f;
            if (dx <= dy && dx <= dz) {
                out.normal = glm::vec3(sphere.center.x > box_center.x ? 1.0f : -1.0f, 0, 0);
            } else if (dy <= dx && dy <= dz) {
                out.normal = glm::vec3(0, sphere.center.y > box_center.y ? 1.0f : -1.0f, 0);
            } else {
                out.normal = glm::vec3(0, 0, sphere.center.z > box_center.z ? 1.0f : -1.0f);
            }
            out.penetration_depth = sphere.radius;
        }
        return out;
    }

    CollisionHit Penetration(const AABB& box_a, const AABB& box_b) noexcept {
        CollisionHit out{};
        if (!Overlap(box_a, box_b)) {
            return out;
        }

        const float overlap_x = std::min(box_a.max.x, box_b.max.x) - std::max(box_a.min.x, box_b.min.x);
        const float overlap_y = std::min(box_a.max.y, box_b.max.y) - std::max(box_a.min.y, box_b.min.y);
        const float overlap_z = std::min(box_a.max.z, box_b.max.z) - std::max(box_a.min.z, box_b.min.z);

        out.hit = true;
        out.time_of_impact = 0.0f;

        const glm::vec3 center_a = (box_a.min + box_a.max) * 0.5f;
        const glm::vec3 center_b = (box_b.min + box_b.max) * 0.5f;
        if (overlap_x <= overlap_y && overlap_x <= overlap_z) {
            out.normal = glm::vec3(center_a.x < center_b.x ? -1.0f : 1.0f, 0.0f, 0.0);
            out.penetration_depth = overlap_x;
        } else if (overlap_y <= overlap_x && overlap_y <= overlap_z) {
            out.normal = glm::vec3(0, center_a.y < center_b.y ? -1.0f : 1.0f, 0);
            out.penetration_depth = overlap_y;
        } else {
            out.normal = glm::vec3(0, 0, center_a.z < center_b.z ? -1.0f : 1.0f);
            out.penetration_depth = overlap_z;
        }
        return out;
    }

    glm::vec3 Slide(const glm::vec3& vec, const glm::vec3& normal) noexcept {
        return vec - dot(vec, normal) * normal;
    }
} // namespace
