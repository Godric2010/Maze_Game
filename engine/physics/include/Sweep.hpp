#pragma once
#include "Types.hpp"

namespace Engine::Physics {
    /**
     * Performs swept collision detection between a moving sphere and an axis-aligned bounding box (AABB).
     *
     * This function checks if a sphere, moving along a specified direction and distance, collides with an AABB.
     * The AABB is expanded by the sphere's radius to account for the spherical shape during the collision test.
     * If a collision is detected, details such as the time of impact, collision point, collision normal, and penetration depth are provided.
     *
     * @param sphere The sphere being tested, defined by its center position and radius.
     * @param vec The motion vector of the sphere, representing the scale and direction of movement.
     * @param box The axis-aligned bounding box (AABB) being tested, defined by its minimum and maximum extents.
     * @return A CollisionHit object containing information about the collision. If a collision occurs, details such as the
     *         point of impact and collision normal are included. If no collision occurs, the object indicates no collision.
     */
    CollisionHit Sweep(const Sphere &sphere, const glm::vec3 &vec, const AABB &box) noexcept;

    /**
     * Performs swept collision detection between a moving sphere and an oriented bounding box (OBB).
     *
     * This function checks if and when a sphere, moving along a specified direction and distance, collides with an OBB.
     * The sphere and its motion are transformed into the OBB's local coordinate space for the collision test, and the
     * result is transformed back into the world coordinate space after the calculation. Collision details such as the
     * time of impact, collision point, and collision normal are returned.
     *
     * @param sphere The sphere being tested, defined by its center position and radius.
     * @param vec The motion vector of the sphere, representing the scale and direction of movement.
     * @param box The oriented bounding box (OBB) being tested, defined by its center position, orientation matrix, and extents.
     * @return A CollisionHit object containing information about the collision. If a collision occurs, details such as the
     *         time of impact, point of impact, normal at the collision, and penetration depth are included. If no collision
     *         occurs, the returned object indicates no collision occurred.
     */
    CollisionHit Sweep(const Sphere &sphere, const glm::vec3 &vec, const OBB &box) noexcept;
} // namespace
