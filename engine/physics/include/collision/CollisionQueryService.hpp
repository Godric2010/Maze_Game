//
// Created by Sebastian Borsch on 06.10.25.
//

#pragma once
#include <unordered_map>
#include <vector>

#include "AABB.hpp"
#include "Bounds.hpp"
#include "Math.hpp"
#include "OBB.hpp"
#include "collision/ColliderCache.hpp"
#include "collision/IBroadphase.hpp"

namespace yarep::physics::collision {
    struct ICollisionQueryService {
        virtual ~ICollisionQueryService() = default;

        virtual void QuerySphereSweep(const math::Vec3& pos, const math::Vec3& rest, float radius,
                                      std::vector<ecs::EntityId>& out,
                                      const QueryFilter* f) const = 0;

        [[nodiscard]] virtual const geometry::AABB* GetAabb(ecs::EntityId) const = 0;

        [[nodiscard]] virtual const geometry::OBB* GetObb(ecs::EntityId) const = 0;
    };

    class CollisionQueryService final : public ICollisionQueryService {
    public:
        CollisionQueryService(IBroadphase& broadphase,
                              ColliderCache& collider_cache)
            : m_broadphase(broadphase), m_collider_cache(collider_cache) {
        }

        void QuerySphereSweep(const math::Vec3& pos, const math::Vec3& rest, const float radius,
                              std::vector<ecs::EntityId>& out, const QueryFilter* filter) const override {
            const geometry::AABB swept = BuildSweptAabb(pos, rest, radius);
            out.clear();
            m_broadphase.QueryAabb(swept, out, filter);
        }

        [[nodiscard]] const geometry::AABB* GetAabb(const ecs::EntityId entity) const override {
            const auto it = m_collider_cache.box_colliders.find(entity);
            if (m_collider_cache.box_colliders.end() == it)
                return nullptr;

            if (it->second.is_trigger)
                return nullptr;

            return &it->second.world_box;
        }

        [[nodiscard]] const geometry::OBB* GetObb(const ecs::EntityId entity) const override {
            const auto it = m_collider_cache.box_colliders.find(entity);
            if (m_collider_cache.box_colliders.end() == it)
                return nullptr;

            if (it->second.is_trigger)
                return nullptr;

            return &it->second.world_obb;
        }

    private:
        IBroadphase& m_broadphase;
        ColliderCache& m_collider_cache;

        static geometry::AABB BuildSweptAabb(const math::Vec3& pos, const math::Vec3& rest,
                                             const float radius) noexcept {
            const geometry::Sphere start{
                pos,
                radius
            };

            const geometry::Sphere end{
                pos + rest,
                radius
            };

            return geometry::merge(geometry::to_aabb(start), geometry::to_aabb(end));
        }
    };
}
