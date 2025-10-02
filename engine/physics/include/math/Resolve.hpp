#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>


#include "Types.hpp"
#include "ClosestPoint.hpp"
#include "Overlap.hpp"

namespace Engine::Physics::Math {
    /**
     * Calculates collision penetration properties between a sphere and an axis-aligned bounding box (AABB).
     *
     * This function determines if a collision has occurred between a given sphere and an AABB, and computes
     * details of the collision if one exists. The returned `CollisionHit` object includes information about
     * whether a collision occurred, the point of contact, normal vector at the contact point, the penetration
     * depth, and the time of impact.
     *
     * The computation involves finding the closest point on the AABB to the sphere's center, and checking
     * whether the sphere's radius overlaps with this point. If an overlap is detected, the penetration depth
     * and collision normal are calculated accordingly.
     *
     * @param sphere The sphere involved in the collision check, characterized by its center position and radius.
     * @param box The axis-aligned bounding box (AABB) involved in the collision check.
     * @return A `CollisionHit` object containing details about the collision. The `hit` member indicates if
     *         a collision occurred, and if so, the remaining members provide additional details about the
     *         collision.
     * @throws None This function is declared `noexcept` and does not throw exceptions.
     */
    CollisionHit Penetration(const Sphere &sphere, const AABB &box) noexcept;

    /**
     * Calculates collision penetration properties between two axis-aligned bounding boxes (AABBs).
     *
     * This function determines if a collision has occurred between two AABBs and computes details of
     * the collision if one exists. The returned `CollisionHit` object provides information about whether
     * a collision occurred, the penetration depth, and the collision normal.
     *
     * The computation involves checking if the two AABBs overlap based on their min and max bounds.
     * If an overlap is detected, the function calculates the penetration depth along each axis and determines
     * the axis of minimum penetration to compute the collision normal.
     *
     * @param box_a The first AABB involved in the collision check.
     * @param box_b The second AABB involved in the collision check.
     * @return A `CollisionHit` object containing details about the collision. The `hit` member indicates if
     *         a collision occurred, and the remaining members provide additional details such as the penetration
     *         depth and collision normal.
     * @throws None This function is declared `noexcept` and does not throw exceptions.
     */
    CollisionHit Penetration(const AABB &box_a, const AABB &box_b) noexcept;

    /**
     * Calculates the sliding vector by removing the component of a given vector
     * that is aligned with a surface normal.
     *
     * This function resolves the input vector into two components: one parallel
     * to the surface defined by the normal vector and one perpendicular to it.
     * The resulting vector represents the projected component along the surface,
     * simulating the sliding motion along that surface.
     *
     * @param vec The vector to be resolved and projected along the surface.
     * @param normal The surface normal against which the vector is projected. It is assumed to be a unit vector.
     * @return The sliding vector obtained by projecting the input vector onto the surface.
     * @throws None This function is declared `noexcept` and does not throw exceptions.
     */
    glm::vec3 Slide(const glm::vec3 &vec, const glm::vec3 &normal) noexcept;
} // namespace
