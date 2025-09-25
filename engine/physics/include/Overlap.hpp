#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "Types.hpp"
#include "ClosestPoint.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace Engine::Physics {
    bool Overlap(const Sphere &sphere, const AABB &bounding_box) noexcept;

    bool Overlap(const Sphere &sphere, const OBB &obb) noexcept;

    bool Overlap(const AABB &bounds_a, const AABB &bounds_b) noexcept;
} // namespace
