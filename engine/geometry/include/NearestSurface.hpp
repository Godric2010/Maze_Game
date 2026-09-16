#pragma once
#include <Math.hpp>
#include "AABB.hpp"

#include "OBB.hpp"

namespace yarep::geometry {
    struct NearestSurface {
        math::Vec3 normal{};
        float distance{};
    };

    NearestSurface nearest_surface(const AABB& aabb, const math::Vec3& point);

    NearestSurface nearest_surface(const OBB& obb, const math::Vec3& point);
}
