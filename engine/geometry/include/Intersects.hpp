#pragma once
#include "AABB.hpp"
#include "OBB.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

namespace yarep::geometry {
    bool intersects(const AABB& lhs, const AABB& rhs);

    bool intersects(const Sphere& lhs, const Sphere& rhs);

    bool intersects(const Sphere& sphere, const AABB& aabb);

    bool intersects(const AABB& aabb, const Sphere& sphere);

    bool intersects(const Sphere& sphere, const OBB& obb);

    bool intersects(const OBB& obb, const Sphere& sphere);

    bool intersects(const OBB& obb, const AABB& aabb);

    bool intersects(const AABB& aabb, const OBB& obb);

    bool intersects(const OBB& lhs, const OBB& rhs);

    bool intersects(const Plane& plane, const Sphere& sphere);

    bool intersects(const Sphere& sphere, const Plane& plane);

    bool intersects(const Plane& plane, const AABB& aabb);

    bool intersects(const AABB& aabb, const Plane& plane);

    bool intersects(const Plane& plane, const OBB& obb);

    bool intersects(const OBB& obb, const Plane& plane);
}
