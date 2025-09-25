#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>


#include "Types.hpp"
#include "ClosestPoint.hpp"
#include "Overlap.hpp"

namespace Engine::Physics {
    CollisionHit Penetration(const Sphere &sphere, const AABB &box) noexcept;

    CollisionHit Penetration(const AABB &box_a, const AABB &box_b) noexcept;

    glm::vec3 Slide(const glm::vec3 &vec, const glm::vec3 &normal) noexcept;
} // namespace
