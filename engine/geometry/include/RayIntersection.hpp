#pragma once
#include <optional>

#include "AABB.hpp"
#include "OBB.hpp"
#include "Plane.hpp"
#include "Ray.hpp"
#include "RayHit.hpp"
#include "Sphere.hpp"

namespace yarep::geometry {
    std::optional<RayHit> ray_intersection(const Ray& ray, const Sphere& sphere);

    std::optional<RayHit> ray_intersection(const Ray& ray, const Plane& plane);

    std::optional<RayHit> ray_intersection(const Ray& ray, const AABB& aabb);

    std::optional<RayHit> ray_intersection(const Ray& ray, const OBB& obb);
}
