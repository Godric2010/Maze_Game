#include "RayIntersection.hpp"

#include "QuaternionMath.hpp"
#include "RayIntersectionHelper.hpp"
#include "VectorMath.hpp"

namespace yarep::geometry {
    std::optional<RayHit> ray_intersection(const Ray& ray, const Sphere& sphere) {
        // build quadratic equation
        const auto origin_center_dir = ray.origin - sphere.center;
        const auto a = math::dot(ray.direction, ray.direction);
        const auto b = 2 * math::dot(origin_center_dir, ray.direction);
        const auto c = math::dot(origin_center_dir, origin_center_dir) - sphere.radius * sphere.radius;

        // check discriminant to eliminate missing rays early
        const auto discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            return {};
        }

        const auto t1 = (-b - sqrt(discriminant)) / (2 * a);
        const auto t2 = (-b + sqrt(discriminant)) / (2 * a);

        if (t1 < 0 && t2 < 0) {
            // Sphere is behind ray -> no hit
            return {};
        }

        auto t = 0.0f;
        if (t1 >= 0 && t2 >= 0) {
            // Sphere is in front of ray -> two hits, first one wins
            t = static_cast<float>(std::min(t1, t2));
        } else {
            // Ray starts inside of sphere -> one valid hit -> bigger value (positive) wins
            t = static_cast<float>(std::max(t1, t2));
        }

        RayHit hit;
        hit.point = ray.origin + ray.direction * t;
        hit.distance = math::distance(ray.origin, hit.point);
        hit.normal = math::normalize(hit.point - sphere.center);
        return {hit};
    }

    std::optional<RayHit> ray_intersection(const Ray& ray, const Plane& plane) {
        const auto signed_distance_origin_to_plane = math::dot(plane.normal, ray.origin) + plane.distance;
        const auto direction_strength = math::dot(plane.normal, ray.direction);
        if (direction_strength == 0) {
            // Ray and plane are parallel
            return {};
        }

        const auto t = -signed_distance_origin_to_plane / direction_strength;
        if (t < 0) {
            // Intersection point is behind the ray -> no hit
            return {};
        }

        RayHit hit;
        hit.point = ray.origin + ray.direction * t;
        hit.distance = math::distance(ray.origin, hit.point);
        hit.normal = plane.normal;
        return {hit};
    }

    std::optional<RayHit> ray_intersection(const Ray& ray, const AABB& aabb) {
        RaySlabTestInterval interval;
        if (!slab_test_interval(ray.origin.x,
                                ray.direction.x,
                                aabb.min.x,
                                aabb.max.x,
                                {-1, 0, 0},
                                {1, 0, 0},
                                interval
                )) {
            return {};
        }
        if (!slab_test_interval(ray.origin.y,
                                ray.direction.y,
                                aabb.min.y,
                                aabb.max.y,
                                {0, -1, 0},
                                {0, 1, 0},
                                interval
                )) {
            return {};
        }
        if (!slab_test_interval(ray.origin.z,
                                ray.direction.z,
                                aabb.min.z,
                                aabb.max.z,
                                {0, 0, -1},
                                {0, 0, 1},
                                interval
                )) {
            return {};
        }

        // Entire intersection interval lies behind the ray origin
        if (interval.exit < 0.0f) { return {}; }

        float hit_t;
        math::Vec3 hit_normal;
        if (interval.entry >= 0.0f) {
            // Ray starts outside the AABB
            hit_t = interval.entry;
            hit_normal = interval.entry_normal;
        } else {
            // Ray starts inside the AABB
            hit_t = interval.exit;
            hit_normal = interval.exit_normal;
        }

        RayHit hit;
        hit.point = ray.origin + ray.direction * hit_t;
        hit.distance = math::distance(ray.origin, hit.point);
        hit.normal = hit_normal;
        return {hit};
    }

    std::optional<RayHit> ray_intersection(const Ray& ray, const OBB& obb) {




        throw std::runtime_error("ray_intersection not implemented");
    }
}
