
#pragma once
#include "AABB.hpp"
#include "OBB.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

namespace yarep::geometry {

    math::Vec3 closest_point(const AABB& aabb, const math::Vec3& point);

    math::Vec3 closest_point(const OBB& obb, const math::Vec3& point);

    math::Vec3 closest_point(const Sphere& sphere, const math::Vec3& point);

    math::Vec3 closest_point(const Plane& plane, const math::Vec3& point);
}
