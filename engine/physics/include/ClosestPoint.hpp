#pragma once
#include <glm/glm.hpp>
#include "Types.hpp"

namespace Engine::Physics {
    glm::vec3 ClosestPoint(const glm::vec3 &point, const AABB &box) noexcept;

    glm::vec3 ClosestPoint(const glm::vec3 &point, const OBB &box) noexcept;
} // namespace
