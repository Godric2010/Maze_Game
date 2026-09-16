#include "collision/Resolve.hpp"

#include "ClosestPoint.hpp"
#include "Intersects.hpp"
#include "NearestSurface.hpp"
#include "VectorMath.hpp"


namespace yarep::physics::collision {
    namespace {
        constexpr float k_epsilon = 1e-6f;
    }

    CollisionHit Penetration(const geometry::Sphere& sphere, const geometry::AABB& box) noexcept {
        CollisionHit out{};

        const auto closest_point = geometry::closest_point(box, sphere.center);
        const auto delta = sphere.center - closest_point;


        const float distance_squared = math::length_squared(delta);
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
            const auto surface = geometry::nearest_surface(box, sphere.center);
            out.normal = surface.normal;
            out.penetration_depth = surface.distance + sphere.radius;
        }
        return out;
    }

    CollisionHit Penetration(const geometry::AABB& box_a, const geometry::AABB& box_b) noexcept {
        CollisionHit out{};
        if (!geometry::intersects(box_a, box_b)) {
            return out;
        }

        const float overlap_x = std::min(box_a.max.x, box_b.max.x) - std::max(box_a.min.x, box_b.min.x);
        const float overlap_y = std::min(box_a.max.y, box_b.max.y) - std::max(box_a.min.y, box_b.min.y);
        const float overlap_z = std::min(box_a.max.z, box_b.max.z) - std::max(box_a.min.z, box_b.min.z);

        out.hit = true;
        out.time_of_impact = 0.0f;

        const math::Vec3 center_a = geometry::center(box_a);
        const math::Vec3 center_b = geometry::center(box_b);
        if (overlap_x <= overlap_y && overlap_x <= overlap_z) {
            out.normal = math::Vec3(center_a.x < center_b.x ? -1.0f : 1.0f, 0.0f, 0.0);
            out.penetration_depth = overlap_x;
        } else if (overlap_y <= overlap_x && overlap_y <= overlap_z) {
            out.normal = math::Vec3(0, center_a.y < center_b.y ? -1.0f : 1.0f, 0);
            out.penetration_depth = overlap_y;
        } else {
            out.normal = math::Vec3(0, 0, center_a.z < center_b.z ? -1.0f : 1.0f);
            out.penetration_depth = overlap_z;
        }
        return out;
    }

    math::Vec3 Slide(const math::Vec3& vec, const math::Vec3& normal) noexcept {
        return vec - dot(vec, normal) * normal;
    }
} // namespace
