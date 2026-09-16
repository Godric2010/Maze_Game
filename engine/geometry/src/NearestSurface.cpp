#include "NearestSurface.hpp"
#include <cmath>

namespace yarep::geometry {
    ;

    NearestSurface nearest_surface(const AABB& aabb, const math::Vec3& point) {
        const float min_x_distance =
                std::abs(point.x - aabb.min.x);

        const float max_x_distance =
                std::abs(aabb.max.x - point.x);

        const float min_y_distance =
                std::abs(point.y - aabb.min.y);

        const float max_y_distance =
                std::abs(aabb.max.y - point.y);

        const float min_z_distance =
                std::abs(point.z - aabb.min.z);

        const float max_z_distance =
                std::abs(aabb.max.z - point.z);

        float nearest_distance = min_x_distance;
        math::Vec3 nearest_normal{-1, 0, 0};

        if (max_x_distance < nearest_distance) {
            nearest_distance = max_x_distance;
            nearest_normal = {1, 0, 0};
        }

        if (min_y_distance < nearest_distance) {
            nearest_distance = min_y_distance;
            nearest_normal = {0, -1, 0};
        }

        if (max_y_distance < nearest_distance) {
            nearest_distance = max_y_distance;
            nearest_normal = {0, 1, 0};
        }

        if (min_z_distance < nearest_distance) {
            nearest_distance = min_z_distance;
            nearest_normal = {0, 0, -1};
        }

        if (max_z_distance < nearest_distance) {
            nearest_normal = {0, 0, 1};
            nearest_distance = max_z_distance;
        }

        return {.normal = nearest_normal, .distance = nearest_distance};
    }

    NearestSurface nearest_surface(const OBB& obb, const math::Vec3& point) {
        const auto inverse_rotation =
                math::inverse(obb.rotation);

        const auto local_point =
                math::rotate(
                        inverse_rotation,
                        point - obb.center
                        );

        const AABB local_bounds{
            -obb.half_extents,
            obb.half_extents
        };

        const auto nearest_local_surface =
                nearest_surface(
                        local_bounds,
                        local_point
                        );

        const auto normal = math::rotate(
                obb.rotation,
                nearest_local_surface.normal
                );
        return {.normal = normal, .distance = nearest_local_surface.distance};
    }
}
