#pragma once
#include <glm/glm.hpp>
#include "Types.hpp"

namespace Engine::Physics {
    /**
     * @brief Computes the closest point within an axis-aligned bounding box (AABB) to a given point.
     *
     * This function determines the nearest point on or within the specified AABB to the input point
     * by clamping the input point's coordinates to the bounds of the AABB.
     *
     * @param point The input point for which the closest point within the AABB is computed.
     * @param box The axis-aligned bounding box (AABB) to which the point's closeness is evaluated.
     * @return The closest point on or within the AABB to the input point.
     */
    glm::vec3 ClosestPoint(const glm::vec3 &point, const AABB &box) noexcept;

    /**
     * @brief Computes the closest point within an oriented bounding box (OBB) to a given point.
     *
     * This function determines the nearest point on or within the specified OBB to the input point
     * by transforming the point into the OBB's local space, clamping it to the box's bounds,
     * and then transforming it back to world space.
     *
     * @param point The input point for which the closest point within the OBB is computed.
     * @param box The oriented bounding box (OBB) to which the point's closeness is evaluated.
     * @return The closest point on or within the OBB to the input point.
     */
    glm::vec3 ClosestPoint(const glm::vec3 &point, const OBB &box) noexcept;
} // namespace
