//
// Created by Sebastian Borsch on 02.10.25.
//

#pragma once
#include <vector>

#include "Ecs/Types.hpp"
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

        virtual void Insert(const BroadphaseProxy& proxy) = 0;

        virtual void Remove(Ecs::EntityId entity) = 0;

        virtual void Update(Ecs::EntityId entity, const Math::AABB& new_aabb) = 0;

        virtual void QueryAabb(const Math::AABB& area, std::vector<Ecs::EntityId>& out, const QueryFilter* filter) = 0;
    };
}
