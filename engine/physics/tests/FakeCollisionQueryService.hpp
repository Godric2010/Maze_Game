//
// Created by Sebastian Borsch on 06.10.25.
//

#pragma once
#include <ranges>

#include "collision/CollisionQueryService.hpp"

namespace yarep::Physics::Collision {
    class FakeCollisionQueryService final : public ICollisionQueryService {
    public:
        std::unordered_map<ecs::EntityId, Math::AABB> aabbs;
        std::unordered_map<ecs::EntityId, Math::OBB> obbs;

        void QuerySphereSweep(const glm::vec3 &pos, const glm::vec3 &rest, float radius,
                              std::vector<ecs::EntityId> &out, const QueryFilter *f) const override {
            out.clear();
            for (const auto &entity: aabbs | std::views::keys) {
                out.push_back(entity);
            }
        }

        [[nodiscard]] const Math::AABB *GetAabb(const ecs::EntityId entity) const override {
            const auto it = aabbs.find(entity);
            return it == aabbs.end() ? nullptr : &it->second;
        }

        [[nodiscard]] const Math::OBB *GetObb(const ecs::EntityId entity) const override {
            const auto it = obbs.find(entity);
            return it == obbs.end() ? nullptr : &it->second;
        }
    };
}
