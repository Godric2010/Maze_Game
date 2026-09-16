#include "collision/Sweep.hpp"

#include <algorithm>
#include <iostream>

#include "Bounds.hpp"
#include "Contains.hpp"
#include "Math.hpp"
#include "NearestSurface.hpp"
#include "Ray.hpp"
#include "RayIntersection.hpp"

namespace yarep::physics::collision {
    namespace {
        constexpr float k_epsilon = 1e-6f;
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
        CollisionHit out{};
        const float movement_length = math::length(vec);
        if (movement_length < k_epsilon) {
            return out;
        }

        const geometry::AABB expanded = geometry::expand(box, sphere.radius);
        if (geometry::contains(expanded, sphere.center)) {
            const auto surface = geometry::nearest_surface(expanded, sphere.center);
            out.hit = true;
            out.time_of_impact = 0.0f;
            out.point = sphere.center;
            out.normal = surface.normal;
            out.penetration_depth = 0.0f;
            return out;
        }

        const geometry::Ray ray{
            sphere.center,
            vec
        };

        const auto ray_hit = geometry::ray_intersection(ray, expanded);
        if (!ray_hit || ray_hit->distance > movement_length) {
            return out;
        }
        out.hit = true;
        out.time_of_impact = ray_hit->distance;
        out.point = ray_hit->point;
        out.normal = ray_hit->normal;
        out.penetration_depth = 0.0f;
        return out;
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
        CollisionHit out{};
        const float movement_length = math::length(vec);
        if (movement_length < k_epsilon) {
            return out;
        }

        const auto expanded = geometry::expand(box, sphere.radius);
        if (geometry::contains(expanded, sphere.center)) {
            const auto surface = geometry::nearest_surface(expanded, sphere.center);
            out.hit = true;
            out.time_of_impact = 0.0f;
            out.point = sphere.center;
            out.normal = surface.normal;
            out.penetration_depth = 0.0f;
            return out;
        }

        const geometry::Ray ray{
            sphere.center,
            vec
        };

        const auto ray_hit = geometry::ray_intersection(ray, expanded);
        if (!ray_hit || ray_hit->distance > movement_length) {
            return out;
        }
        out.hit = true;
        out.time_of_impact = ray_hit->distance;
        out.point = ray_hit->point;
        out.normal = ray_hit->normal;
        out.penetration_depth = 0.0f;
        return out;
    }
} // namespace
