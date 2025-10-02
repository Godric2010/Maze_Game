#pragma once
#include <unordered_map>

#include "collision/IBroadphase.hpp"

namespace Engine::Physics::Collision {
    struct CellKey {
        int x, y, z;
        bool operator==(const CellKey& other) const noexcept{ return x == other.x && y == other.y && z == other.z; }
    };

    struct CellKeyHash {
        std::size_t operator()(const CellKey& key) const noexcept {
            const auto ux = static_cast<uint64_t>(static_cast<uint32_t>(key.x));
            const auto uy = static_cast<uint64_t>(static_cast<uint32_t>(key.y));
            const auto uz = static_cast<uint64_t>(static_cast<uint32_t>(key.z));
            const auto h = ux * 73856093u ^ uy * 19349663u ^ uz * 83492791u;
            return static_cast<std::size_t>(h);
        }
    };

    class SpatialHashBroadphase : public IBroadphase {
    public:
        explicit SpatialHashBroadphase(float cell_size);

         ~SpatialHashBroadphase() override = default;

        void Insert(const BroadphaseProxy& proxy) override;
        void Remove(Ecs::EntityId entity) override;
        void Update(Ecs::EntityId entity, const Math::AABB &new_aabb) override;

        void QueryAABB(const Math::AABB &area, std::vector<Ecs::EntityId> &out, const QueryFilter *filter) override;

    private:
        float m_cell_size;
        float m_inv_cell_size;

        std::unordered_map<CellKey, std::vector<Ecs::EntityId>, CellKeyHash> m_buckets;
        std::unordered_map<Ecs::EntityId, std::vector<CellKey>> m_back_references;
        std::unordered_map<Ecs::EntityId, BroadphaseProxy> m_proxies;

        static inline int FloorToCell(const float v, const float inv_cell) {
            return static_cast<int>(std::floor(v * inv_cell));
        }

        void BoxToCells(const Math::AABB& aabb, std::vector<CellKey>& tmp) const;

        static inline bool PassFilter(const BroadphaseProxy& proxy, const QueryFilter* filter) {
            if (!filter) return true;
            return (proxy.category_bits & filter->mask_bits) && (filter->category_bits & proxy.mask_bits);
        }

        static void Dedupe(std::vector<Ecs::EntityId>& vec);
    };
} // namespace
