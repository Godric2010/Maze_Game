#pragma once
#include "Types.hpp"

namespace Engine::Physics {
    CollisionHit Sweep(const Sphere &sphere, const glm::vec3 &vec, const AABB &box) noexcept;

    CollisionHit Sweep(const Sphere &sphere, const glm::vec3 &vec, const OBB &box) noexcept;
} // namespace
