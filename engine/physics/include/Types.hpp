//
// Created by Sebastian Borsch on 21.09.25.
//

#pragma once
#include <glm/glm.hpp>

namespace Engine::Physics {
    /**
     * @struct Ray
     * @brief Represents a 3D ray in space.
     *
     * The Ray structure consists of an origin point and a direction vector.
     * It is commonly used in simulations, ray-casting, and collision detection
     * within physics and rendering calculations.
     *
     * @note The direction vector is typically expected to be normalized.
     */
    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;
    };

    // ReSharper disable once CppInconsistentNaming
    /**
     * @struct AABB
     * @brief Represents an axis-aligned bounding box in a 3D space.
     *
     * The AABB structure is defined by two points, `min` and `max`, which represent
     * the minimum and maximum corners of the box respectively. It is commonly used
     * in spatial partitioning, collision detection, and other physics-related
     * calculations.
     *
     * @note The components of `min` should be less than or equal to the corresponding
     * components of `max`.
     */
    struct AABB {
        glm::vec3 min;
        glm::vec3 max;
    };

    // ReSharper disable once CppInconsistentNaming
    /**
     * @struct OBB
     * @brief Represents an oriented bounding box in a 3D space.
     *
     * The OBB (Oriented Bounding Box) structure is defined by a center position,
     * half-extents along its local axes, and a rotation matrix which describes its
     * orientation relative to a world or local coordinate system. It is commonly
     * used in collision detection, spatial queries, and other physics-based
     * calculations where bounding volumes with arbitrary orientations are required.
     *
     * @note The orientation is represented by a 3x3 matrix, where each column
     * corresponds to a local axis of the OBB. The half-extents represent the
     * scaled distances from the center to each side along the respective axes.
     */
    struct OBB {
        glm::vec3 center;
        glm::vec3 half_extents;
        glm::mat3 orientation;
    };

    /**
     * @struct Sphere
     * @brief Represents a 3D geometric sphere.
     *
     * The Sphere structure is defined by a center point in 3D space and a radius.
     * It is widely used in physics simulations, collision detection, bounding volume
     * hierarchies, and graphical rendering.
     *
     * @note The radius is expected to be non-negative.
     */
    struct Sphere {
        glm::vec3 center;
        float radius;
    };

    /**
     * @struct CollisionHit
     * @brief Represents the result of a collision detection operation.
     *
     * The CollisionHit structure contains information about a detected collision,
     * such as whether a collision occurred, the time of impact, the collision point,
     * the normal vector at the point of collision, and the penetration depth.
     *
     * @note The `hit` field indicates if a collision occurred, while the `time_of_impact`
     * specifies the normalized time (from 0 to 1) at which the collision took place.
     * The `point` and `normal` provide spatial details of the collision, and the
     * `penetration_depth` describes the overlap distance in case of penetrating objects.
     */
    struct CollisionHit {
        bool hit{false};
        float time_of_impact{1.0f};
        glm::vec3 point;
        glm::vec3 normal;
        float penetration_depth{0};
    };
}
