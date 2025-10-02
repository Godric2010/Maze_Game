//
// Created by Sebastian Borsch on 02.10.25.
//

#pragma once
#include <vector>

#include "../../../ecs/src/Entity.hpp"
#include "math/Types.hpp"

namespace Engine::Physics::Collision {

    struct QueryFilter {
        uint32_t category_bits{0xFFFFFFFF};
        uint32_t mask_bits{0xFFFFFFFF};
    };

    struct BroadphaseProxy {
        Ecs::EntityId entity{};
        Math::AABB aabb{};
        uint32_t category_bits{0xFFFFFFFF};
        uint32_t mask_bits{0xFFFFFFFF};
        bool is_static{false};
    };

    class IBroadphase {
    public:
        virtual ~IBroadphase() = default;

        virtual void Insert(const BroadphaseProxy &proxy) = 0;

        virtual void Remove(Ecs::EntityId entity) = 0;

        virtual void Update(Ecs::EntityId entity, const Math::AABB &new_aabb) = 0;

        virtual void QueryAABB(const Math::AABB &area, std::vector<Ecs::EntityId> &out, const QueryFilter *filter) = 0;

        static inline Math::AABB FromSphere(const Math::Sphere &sphere) {
            const glm::vec3 r(sphere.radius);
            return {sphere.center - r, sphere.center + r};
        }

        static inline Math::AABB FromOBB(const Math::OBB &obb) {
            const glm::mat3 r = obb.orientation;
            const glm::vec3 e = {
                std::abs(r[0][0]) * obb.half_extents.x + std::abs(r[1][0]) * obb.half_extents.y + std::abs(r[2][0]) *
                obb.half_extents.z,
                std::abs(r[0][1]) * obb.half_extents.x + std::abs(r[1][1]) * obb.half_extents.y + std::abs(r[2][1]) *
                obb.half_extents.z,
                std::abs(r[0][2]) * obb.half_extents.x + std::abs(r[1][2]) * obb.half_extents.y + std::abs(r[2][2]) *
                obb.half_extents.z,
            };
            return {obb.center - e, obb.center + e};
        }
    };
}
