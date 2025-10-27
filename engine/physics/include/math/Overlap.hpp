#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "Types.hpp"
#include "ClosestPoint.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace Engine::Physics::Math {
    /**
     * @brief Checks for overlap between a sphere and an axis-aligned bounding box (AABB).
     *
     * This function determines whether a given sphere intersects with an AABB.
     * The determination is performed by finding the closest point on the AABB to the center
     * of the sphere and calculating the squared distance. If the squared distance is less than
     * or equal to the squared radius of the sphere, the objects are considered to overlap.
     *
     * @param sphere The sphere to check for overlap.
     * @param bounding_box The AABB to check for overlap.
     * @return True if the sphere overlaps with the AABB, otherwise false.
     * @note This function operates in constant time and is marked as `noexcept`.
     */
    bool Overlap(const Sphere &sphere, const AABB &bounding_box) noexcept;


    /**
     * @brief Checks for overlap between two spheres.
     *
     * This function determines whether two spheres intersect based on their
     * centers and radii. The calculation involves finding the squared distance
     * between the centers of the two spheres and comparing it with the square of
     * the sum of their radii. If the squared distance is less than or equal to
     * the squared sum of their radii, the spheres are considered to overlap.
     *
     * @param sphere The first sphere to check for overlap.
     * @param other_sphere The second sphere to check for overlap.
     * @return True if the two spheres overlap, otherwise false.
     * @note This function is marked as `noexcept` and operates in constant time.
     */
    bool Overlap(const Sphere& sphere, const Sphere& other_sphere) noexcept;

    /**
     * @brief Checks for overlap between a sphere and an oriented bounding box (OBB).
     *
     * This function determines whether a given sphere intersects with an OBB. The determination
     * involves transforming the sphere's center into the OBB's local space, clamping it to the AABB
     * representation of the OBB, and comparing the squared distance between the sphere's center
     * and the clamped point with the squared radius of the sphere.
     *
     * @param sphere The sphere to check for overlap.
     * @param obb The oriented bounding box to check for overlap.
     * @return True if the sphere overlaps with the OBB, otherwise false.
     * @note This function is marked as `noexcept` and operates using mathematical transformations
     * to ensure efficient computation.
     */
    bool Overlap(const Sphere &sphere, const OBB &obb) noexcept;

    /**
     * @brief Determines whether two axis-aligned bounding boxes (AABBs) overlap.
     *
     * This function checks if the two specified AABBs intersect in 3D space
     * by comparing their minimum and maximum bounds. If there is an overlap
     * in all three axes (x, y, z), the function returns true.
     *
     * @param bounds_a The first AABB to test for overlap.
     * @param bounds_b The second AABB to test for overlap.
     * @return True if the two AABBs overlap, otherwise false.
     * @note This function is marked as `noexcept` and operates in constant time.
     */
    bool Overlap(const AABB &bounds_a, const AABB &bounds_b) noexcept;
} // namespace
