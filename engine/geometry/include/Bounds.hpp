#pragma once
#include "AABB.hpp"
#include "OBB.hpp"
#include "Sphere.hpp"
#include "Transform.hpp"

namespace yarep::geometry {
    AABB to_aabb(const Sphere& sphere);

    AABB to_aabb(const OBB& obb);

    OBB to_obb(const AABB& aabb, const math::Transform& transform);

    AABB transform_bounds(const AABB& aabb, const math::Transform& transform);
}
