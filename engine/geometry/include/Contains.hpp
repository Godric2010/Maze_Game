#pragma once
#include "AABB.hpp"
#include "OBB.hpp"
#include "Sphere.hpp"

namespace yarep::geometry {
    bool contains(const AABB& aabb, const math::Vec3& point);

    bool contains(const Sphere& sphere, const math::Vec3& point);

    bool contains(const OBB& obb, const math::Vec3& point);
}
