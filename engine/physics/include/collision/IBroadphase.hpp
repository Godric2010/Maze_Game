//
// Created by Sebastian Borsch on 02.10.25.
//

#pragma once
#include <vector>

#include "Ecs/Types.hpp"
#include "math/Types.hpp"

namespace yarep::physics::collision {
    struct QueryFilter {
        uint32_t category_bits{0xFFFFFFFF};
        uint32_t mask_bits{0xFFFFFFFF};
    };

    struct BroadphaseProxy {
        ecs::EntityId entity{};
        math::AABB aabb{};
        uint32_t category_bits{0xFFFFFFFF};
        uint32_t mask_bits{0xFFFFFFFF};
        bool is_static{false};
    };

    class IBroadphase {
    public:
        virtual ~IBroadphase() = default;

        virtual void Insert(const BroadphaseProxy& proxy) = 0;

        virtual void Remove(ecs::EntityId entity) = 0;

        virtual void Update(ecs::EntityId entity, const math::AABB& new_aabb) = 0;

        virtual void QueryAabb(const math::AABB& area, std::vector<ecs::EntityId>& out, const QueryFilter* filter) = 0;
    };
}
