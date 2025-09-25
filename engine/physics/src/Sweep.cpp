#include "../include/Sweep.hpp"

#include <algorithm>

namespace Engine::Physics {
    namespace {
        constexpr float k_epsilon = 1e-6f;

        /**
         * Computes the entry point of a ray into an axis-aligned bounding box (AABB).
         *
         * This function calculates the smallest 't' parameter at which a ray, defined by its origin and direction,
         * intersects the given AABB, within the specified length of the ray.
         *
         * @param origin The starting point of the ray as a 3D vector.
         * @param dir The direction of the ray as a normalized 3D vector.
         * @param len The maximum length of the ray.
         * @param box The axis-aligned bounding box to test against, defined by its minimum and maximum coordinates.
         * @return The minimum 't' value at which the ray enters the AABB. Returns a value greater than the ray length
         *         if there's no intersection.
         */
        float RayAABB_tEnter(const glm::vec3 &origin, const glm::vec3 &dir, const float len, const AABB &box) noexcept {
            float tmin = 0.0f;
            float tmax = len;

            for (int i = 0; i < 3; ++i) {
                const float o = origin[i];
                const float d = dir[i];
                float t1, t2;

                if (std::abs(d) < k_epsilon) {
                    if (o < box.min[i] || o > box.max[i]) {
                        return len + 1.0f;
                    }
                    continue;
                }

                t1 = (box.min[i] - o) / d;
                t2 = (box.max[i] - o) / d;
                if (t1 > t2) {
                    std::swap(t1, t2);
                }
                tmin = std::min(tmin, t1);
                tmax = std::max(tmax, t1);
                if (tmin > tmax) {
                    return len + 1.0f;
                }
            }
            return tmin;
        }

        /**
         *
         */
        glm::vec3 NormalFromEntryPoint(const glm::vec3 &origin, const glm::vec3 &dir, const float t,
                                       const AABB &b) noexcept {
            const glm::vec3 p = origin + dir * t;

            const float dx_min = std::abs(p.x - b.min.x);
            const float dx_max = std::abs(b.max.x - p.x);
            const float dy_min = std::abs(p.y - b.min.y);
            const float dy_max = std::abs(b.max.y - p.y);
            const float dz_min = std::abs(p.z - b.min.z);
            const float dz_max = std::abs(b.max.z - p.z);

            float m = dx_min;
            glm::vec3 n(-1, 0, 0);

            if (dx_max < m) {
                m = dx_max;
                n = glm::vec3(1, 0, 0);
            }
            if (dy_min < m) {
                m = dy_min;
                n = glm::vec3(0, -1, 0);
            }
            if (dy_max < m) {
                m = dy_max;
                n = glm::vec3(0, 1, 0);
            }
            if (dz_min < m) {
                m = dz_min;
                n = glm::vec3(0, 0, -1);
            }
            if (dz_max < m) {
                m = dz_max;
                n = glm::vec3(0, 0, 1);
            }
            return n;
        }

        /**
         *
         */
        inline AABB ExpandedByRadius(const AABB &box, const float r) noexcept {
            const auto vec_r = glm::vec3(r);
            return AABB(box.min - vec_r, box.max + vec_r);
        }

        inline void ToLocalSphereAndMotion(const Sphere &sphere, const glm::vec3 &vec, const OBB &box,
                                           Sphere &sphere_local, glm::vec3 &vec_local, AABB &aabb_local) noexcept {
            const glm::mat3 r = box.orientation;
            sphere_local.center = glm::transpose(r) * (sphere.center - box.center);
            sphere_local.radius = sphere.radius;
            vec_local = glm::transpose(r) * vec;
            aabb_local.min = -box.half_extents;
            aabb_local.max = box.half_extents;
        }
    }

    CollisionHit Sweep(const Sphere &sphere, const glm::vec3 &vec, const AABB &box) noexcept {
        CollisionHit hit{};
        const float len = glm::length(vec);
        if (len < k_epsilon) {
            return hit;
        }

        const glm::vec3 dir = vec / len;

        const AABB expanded = ExpandedByRadius(box, sphere.radius);
        const float t_enter = RayAABB_tEnter(sphere.center, dir, len, expanded);

        if (t_enter <= len) {
            hit.hit = true;

            hit.time_of_impact = std::max(0.0f, std::min(t_enter, len));
            hit.point = sphere.center + dir * hit.time_of_impact;
            hit.normal = NormalFromEntryPoint(hit.point, dir, 0.0f, expanded);
            hit.penetration_depth = 0.0f;
        }
        return hit;
    }

    CollisionHit Sweep(const Sphere &sphere, const glm::vec3 &vec, const OBB &box) noexcept {
        Sphere sphere_local{};
        glm::vec3 vec_local{};
        AABB aabb_local{};
        ToLocalSphereAndMotion(sphere, vec, box, sphere_local, vec_local, aabb_local);

        CollisionHit hit = Sweep(sphere_local, vec_local, aabb_local);
        if (!hit.hit) {
            return hit;
        }

        const glm::mat3 r = box.orientation;
        CollisionHit hit_out{};
        hit_out.hit = true;
        hit_out.time_of_impact = hit.time_of_impact;
        hit_out.point = box.center + r * hit.point;
        hit_out.normal = glm::normalize(r * hit.normal);
        hit_out.penetration_depth = 0.0f;
        return hit_out;
    }
} // namespace
