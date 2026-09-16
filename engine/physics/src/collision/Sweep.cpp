#include "collision/Sweep.hpp"

#include <algorithm>
#include <iostream>
#include "Math.hpp"

namespace yarep::physics::collision {
    namespace {
        constexpr float k_epsilon = 1e-6f;

        /**
         * Computes the entry point of a ray into an axis-aligned bounding box (AABB).
         *
         * This function calculates the smallest 't' parameter at which a ray, defined by its origin and direction,
         * intersects the given AABB, within the specified length of the ray.
         *
         * @param origin_vec
         * @param dir The direction of the ray as a normalized 3D vector.
         * @param len The maximum length of the ray.
         * @param box The axis-aligned bounding box to test against, defined by its minimum and maximum coordinates.
         * @return The minimum 't' value at which the ray enters the AABB. Returns a value greater than the ray length
         *         if there's no intersection.
         */
        float RayAabbTEnter(const math::Vec3& origin_vec, const math::Vec3& dir_vec, const float len,
                            const geometry::AABB& box) noexcept {
            float t_near = 0.0f;
            float t_far = len;
            const std::array origin = {origin_vec.x, origin_vec.y, origin_vec.z};
            const std::array dir = {dir_vec.x, dir_vec.y, dir_vec.z};
            const std::array box_min = {box.min.x, box.min.y, box.min.z};
            const std::array box_max = {box.max.x, box.max.y, box.max.z};

            for (int i = 0; i < 3; ++i) {
                const float o = origin[i];
                const float d = dir[i];

                if (std::abs(d) < k_epsilon) {
                    if (o < box_min[i] || o > box_max[i]) {
                        return len + 1.0f;
                    }
                    continue;
                }

                float t1 = (box_min[i] - o) / d;
                float t2 = (box_max[i] - o) / d;
                if (t1 > t2) {
                    std::swap(t1, t2);
                }
                t_near = std::max(t_near, t1);
                t_far = std::min(t_far, t2);
                if (t_near > t_far) {
                    return len + 1.0f;
                }
            }
            if (t_far < 0.0) {
                return len + 1.0f;
            }
            return t_near;
        }

        /**
         * Determines the normal vector of the surface a ray intersects with an axis-aligned bounding box (AABB).
         *
         * This function calculates the surface normal at the entry point where a ray defined by its origin and direction
         * intersects the given AABB at the parameter `t`. The normal vector is determined based on the closest face of the
         * AABB to the point of intersection.
         *
         * @param origin The starting point of the ray as a 3D vector.
         * @param dir The direction of the ray as a normalized 3D vector.
         * @param t The ray parameter at which the intersection occurs, representing the distance along the ray.
         * @param b The axis-aligned bounding box specified by its minimum and maximum coordinates.
         * @return The normal vector of the intersected AABB face, represented as a 3D vector.
         */
        math::Vec3 NormalFromEntryPoint(const math::Vec3& origin, const math::Vec3& dir, const float t,
                                        const geometry::AABB& b) noexcept {
            const math::Vec3 p = origin + dir * t;

            const float dx_min = std::abs(p.x - b.min.x);
            const float dx_max = std::abs(b.max.x - p.x);
            const float dy_min = std::abs(p.y - b.min.y);
            const float dy_max = std::abs(b.max.y - p.y);
            const float dz_min = std::abs(p.z - b.min.z);
            const float dz_max = std::abs(b.max.z - p.z);

            float min = dx_min;
            math::Vec3 normal(-1, 0, 0);

            if (dx_max < min) {
                min = dx_max;
                normal = math::Vec3(1, 0, 0);
            }
            if (dy_min < min) {
                min = dy_min;
                normal = math::Vec3(0, -1, 0);
            }
            if (dy_max < min) {
                min = dy_max;
                normal = math::Vec3(0, 1, 0);
            }
            if (dz_min < min) {
                min = dz_min;
                normal = math::Vec3(0, 0, -1);
            }
            if (dz_max < min) {
                min = dz_max;
                normal = math::Vec3(0, 0, 1);
            }
            return normal;
        }

        /**
         * Expands an axis-aligned bounding box (AABB) by a given radius.
         *
         * This function creates a new AABB that is enlarged outward by a specified radius in all directions.
         * The expansion is achieved by subtracting the radius from the minimum corner and adding it to the
         * maximum corner of the original AABB.
         *
         * @param box The original axis-aligned bounding box to be expanded, defined by its minimum and maximum coordinates.
         * @param r The radius by which the AABB should be expanded. This value is applied uniformly along all axes.
         * @return A new AABB that represents the expanded bounding box.
         */
        inline geometry::AABB ExpandedByRadius(const geometry::AABB& box, const float r) noexcept {
            const auto vec_r = math::Vec3{r, r, r};
            return geometry::AABB(box.min - vec_r, box.max + vec_r);
        }

        /**
         * Converts a sphere's position and motion into the local space of an oriented bounding box (OBB).
         *
         * This function transforms the position and motion vector of a sphere into the local coordinate
         * space of the provided OBB using the OBB's orientation and center. Additionally, it computes
         * the axis-aligned bounding box (AABB) in the local space of the OBB.
         *
         * @param sphere The original sphere in world space, defined by its center and radius.
         * @param vec The motion vector of the sphere in world space.
         * @param box The oriented bounding box in world space, defined by its center, half extents, and orientation.
         * @param sphere_local The transformed sphere in the local space of the OBB. This value is modified by the function.
         * @param vec_local The transformed motion vector in the local space of the OBB. This value is modified by the function.
         * @param aabb_local The axis-aligned bounding box in the local space of the OBB. This value is modified by the function.
         */
        inline void ToLocalSphereAndMotion(const geometry::Sphere& sphere, const math::Vec3& vec,
                                           const geometry::OBB& box,
                                           geometry::Sphere& sphere_local, math::Vec3& vec_local,
                                           geometry::AABB& aabb_local) noexcept {
            const auto r = box.rotation;
            sphere_local.center = math::rotate(math::inverse(r), sphere.center - box.center);
            sphere_local.radius = sphere.radius;
            vec_local = math::rotate(math::inverse(r), vec);
            aabb_local.min = -box.half_extents;
            aabb_local.max = box.half_extents;
        }
    }

    /**
     * Performs a swept collision detection between a moving sphere and an axis-aligned bounding box (AABB).
     *
     * This function determines if a sphere, moving along a given 3D vector, intersects with an AABB.
     * It calculates the time of impact, collision point, collision normal, and other relevant details.
     * The AABB is expanded by the sphere's radius to account for the sphere's shape during the collision test.
     *
     * @param sphere The sphere to test, defined by its center and radius.
     * @param vec The motion vector of the sphere, representing its direction and magnitude of movement.
     * @param box The axis-aligned bounding box (AABB) to test against, defined by its minimum and maximum coordinates.
     * @return A CollisionHit object containing information about whether a collision occurred. If a collision occurs,
     *         the object includes the time of impact, collision point, collision normal, and penetration depth. If no
     *         collision occurs, the default CollisionHit values indicate no interaction.
     */
    CollisionHit Sweep(const geometry::Sphere& sphere, const math::Vec3& vec, const geometry::AABB& box) noexcept {
        CollisionHit hit{};
        const float len = math::length(vec);
        if (len < k_epsilon) {
            return hit;
        }

        const math::Vec3 dir = vec / len;

        const geometry::AABB expanded = ExpandedByRadius(box, sphere.radius);
        const float t_enter = RayAabbTEnter(sphere.center, dir, len, expanded);

        if (t_enter <= len) {
            hit.hit = true;

            hit.time_of_impact = std::max(0.0f, std::min(t_enter, len));
            hit.point = sphere.center + dir * hit.time_of_impact;
            hit.normal = NormalFromEntryPoint(sphere.center, dir, hit.time_of_impact, expanded);
            hit.penetration_depth = 0.0f;
        }
        return hit;
    }

    /**
     * Performs a swept test of a moving sphere against an oriented bounding box (OBB) to detect collisions.
     *
     * This function calculates if and when a sphere moving along a given direction vector intersects with an OBB.
     * It transforms the sphere and its motion into the local space of the OBB, performs a collision sweep in the local space,
     * and then converts the results back into world space.
     *
     * @param sphere The sphere to be tested, defined by its center position and radius.
     * @param vec The motion vector of the sphere, representing its direction and distance of movement.
     * @param box The oriented bounding box (OBB) to test against, defined by its center position, orientation matrix, and extents.
     * @return A CollisionHit object containing information about whether a collision occurred, the time of impact,
     *         the collision point, the collision normal, and the penetration depth.
     */
    CollisionHit Sweep(const geometry::Sphere& sphere, const math::Vec3& vec, const geometry::OBB& box) noexcept {
        geometry::Sphere sphere_local{};
        math::Vec3 vec_local{};
        geometry::AABB aabb_local{};
        ToLocalSphereAndMotion(sphere, vec, box, sphere_local, vec_local, aabb_local);

        CollisionHit hit = Sweep(sphere_local, vec_local, aabb_local);
        if (!hit.hit) {
            return hit;
        }

        const auto r = box.rotation;
        CollisionHit hit_out{};
        hit_out.hit = true;
        hit_out.time_of_impact = hit.time_of_impact;
        hit_out.point = box.center + math::rotate(box.rotation, hit.point);
        hit_out.normal = math::normalize(math::rotate(r, hit.normal));
        hit_out.penetration_depth = hit.penetration_depth;
        return hit_out;
    }
} // namespace
