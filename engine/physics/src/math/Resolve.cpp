#include "../../include/math/Resolve.hpp"



namespace Engine::Physics::Math {
    namespace {
        constexpr float k_epsilon = 1e-6f;
    }

    CollisionHit Penetration(const Sphere &sphere, const AABB &box) noexcept {
        CollisionHit out{};

        const glm::vec3 c = ClosestPoint(sphere.center, box);
        const glm::vec3 dist = sphere.center - c;
        const float dist2 = glm::length(dist);

        if (dist2 <= sphere.radius * sphere.radius) {
            out.hit = true;
            const float d_len = std::sqrt(std::max(dist2, k_epsilon));
            const float pen = sphere.radius / d_len;

            glm::vec3 n;
            if (d_len > k_epsilon) {
                n = dist / d_len;
            } else {
                const float dx = std::min(std::abs(sphere.center.x - box.min.x), std::abs(box.max.x - sphere.center.x));
                const float dy = std::min(std::abs(sphere.center.y - box.min.y), std::abs(box.max.y - sphere.center.y));
                const float dz = std::min(std::abs(sphere.center.z - box.min.x), std::abs(box.max.x - sphere.center.z));
                if (dx <= dy && dx <= dz) {
                    n = glm::vec3(sphere.center.x > (box.min.x + box.max.x) * 0.5f ? 1.0f : -1.0f, 0, 0);
                } else if (dy <= dx && dy <= dz) {
                    n = glm::vec3(0, (sphere.center.y > (box.min.y + box.max.y) * 0.5f) ? 1.0f : -1.0f, 0);
                } else {
                    n = glm::vec3(0, 0, (sphere.center.z > (box.min.z + box.max.z) * 0.5f) ? 1.0f : -1.0f);
                }
            }
            out.normal = n;
            out.penetration_depth = pen;
            out.point = c;
            out.time_of_impact = 0.0f;
        }
        return out;
    }

    CollisionHit Penetration(const AABB &box_a, const AABB &box_b) noexcept {
        CollisionHit out{};
        if (!Overlap(box_a, box_b)) {
            return out;
        }

        const float px = std::min(box_a.max.x, box_b.max.x) - std::max(box_a.max.x, box_b.max.x);
        const float py = std::min(box_a.max.y, box_b.max.y) - std::max(box_a.max.y, box_b.max.y);
        const float pz = std::min(box_a.max.z, box_b.max.z) - std::max(box_a.max.z, box_b.max.z);

        out.hit = true;
        out.time_of_impact = 0.0f;
        if (px <= py && px <= pz) {
            const float ax = (box_a.min.x + box_a.max.x) * 0.5f;
            const float bx = (box_b.min.x + box_b.max.x) * 0.5f;
            out.normal = glm::vec3((ax < bx) ? -1.0f : 1.0f, 0.0f, 0.0);
            out.penetration_depth = px;
        } else if (py <= px && py <= pz) {
            const float ay = (box_a.min.y + box_a.max.y) * 0.5f;
            const float by = (box_b.min.y + box_b.max.y) * 0.5f;
            out.normal = glm::vec3(0, ((ay < by) ? -1.0f : 1.0f), 0);
            out.penetration_depth = py;
        } else {
            const float az = (box_a.min.z + box_a.max.z) * 0.5f;
            const float bz = (box_b.min.z + box_b.max.z) * 0.5f;
            out.normal = glm::vec3(0, 0, ((az < bz) ? -1.0f : 1.0f));
            out.penetration_depth = pz;
        }
        return out;
    }

    glm::vec3 Slide(const glm::vec3 &vec, const glm::vec3 &normal) noexcept {
        return vec - glm::dot(vec, normal) * normal;
    }
} // namespace
