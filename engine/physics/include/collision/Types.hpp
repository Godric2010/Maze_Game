//
// Created by Sebastian Borsch on 21.09.25.
//

#pragma once
#include <Math.hpp>

namespace yarep::physics::collision {
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
        float time_of_impact{0.0f};
        math::Vec3 point;
        math::Vec3 normal;
        float penetration_depth{0};
    };
}
