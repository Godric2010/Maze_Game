//
// Created by Sebastian Borsch on 06.10.25.
//

#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <vector>

#include "../../../ecs/src/Entity.hpp"
#include "collision/IBroadphase.hpp"
#include "math/Types.hpp"

namespace Engine::Physics::Collision {
    struct ICollisionQueryService {
        virtual ~ICollisionQueryService() = default;

        virtual void QuerySphereSweep(const glm::vec3 &pos, const glm::vec3 &rest, float radius,
                                      std::vector<Ecs::EntityId> &out,
                                      const QueryFilter *f) const = 0;

        [[nodiscard]] virtual const Math::AABB *GetAabb(Ecs::EntityId) const = 0;
    };

    class CollisionQueryService final : public ICollisionQueryService {
    public:
        CollisionQueryService(IBroadphase &broadphase,
                              const std::unordered_map<Ecs::EntityId, Math::AABB> &aabbs)
            : m_broadphase(broadphase), m_aabbs(aabbs) {
        }

        void QuerySphereSweep(const glm::vec3 &pos, const glm::vec3 &rest, const float radius,
                              std::vector<Ecs::EntityId> &out, const QueryFilter *filter) const override {
            const Math::AABB swept = BuildSweptAabb(pos, rest, radius);
            out.clear();
            m_broadphase.QueryAABB(swept, out, filter);
        }

        [[nodiscard]] const Math::AABB *GetAabb(const Ecs::EntityId entity) const override {
            const auto it = m_aabbs.find(entity);
            return it == m_aabbs.end() ? nullptr : &it->second;
        }

    private:
        IBroadphase &m_broadphase;
        const std::unordered_map<Ecs::EntityId, Math::AABB> &m_aabbs;

        static Math::AABB BuildSweptAabb(const glm::vec3 &pos, const glm::vec3 &rest, const float radius) noexcept {
            const glm::vec3 p0 = pos;
            const glm::vec3 p1 = pos + rest;

            const glm::vec3 min = glm::min(p0, p1) - glm::vec3(radius);
            const glm::vec3 max = glm::max(p0, p1) + glm::vec3(radius);
            return {min, max};
        }
    };
}
