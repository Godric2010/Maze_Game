#include "SpatialHashBroadphase.hpp"

#include <algorithm>
#include <unordered_set>

namespace Engine::Physics::Collision {
    SpatialHashBroadphase::SpatialHashBroadphase(const float cell_size) {
        m_cell_size = cell_size;
        m_inv_cell_size = (1.0f / cell_size);
    }

    void SpatialHashBroadphase::BoxToCells(const Math::AABB &aabb, std::vector<CellKey> &tmp) const {
        tmp.clear();
        const int ix0 = FloorToCell(aabb.min.x, m_inv_cell_size);
        const int iy0 = FloorToCell(aabb.min.y, m_inv_cell_size);
        const int iz0 = FloorToCell(aabb.min.z, m_inv_cell_size);
        const int ix1 = FloorToCell(aabb.max.x, m_inv_cell_size);
        const int iy1 = FloorToCell(aabb.max.y, m_inv_cell_size);
        const int iz1 = FloorToCell(aabb.max.z, m_inv_cell_size);

        tmp.reserve(static_cast<size_t>(std::max(1, (ix1 - ix0 + 1) * (iy1 - iz0 + 1) * (iz1 - iz0 + 1))));

        for (int z = iz0; z <= iz1; ++z)
            for (int y = iy0; y <= iy1; ++y)
                for (int x = ix0; x <= ix1; ++x) {
                    tmp.push_back({x, y, z});
                }
    }

    void SpatialHashBroadphase::Insert(const BroadphaseProxy &proxy) {
        m_proxies[proxy.entity] = proxy;

        std::vector<CellKey> cells;
        BoxToCells(proxy.aabb, cells);
        auto &back = m_back_references[proxy.entity];
        back = cells;

        for (const auto &cell: cells) {
            auto &vec = m_buckets[cell];
            vec.push_back(proxy.entity);
        }
    }

    void SpatialHashBroadphase::Remove(const Ecs::EntityId entity) {
        const auto it_back = m_back_references.find(entity);
        if (it_back != m_back_references.end()) {
            for (const auto &cell: it_back->second) {
                auto bucket_it = m_buckets.find(cell);
                if (bucket_it == m_buckets.end()) {
                    continue;
                }
                auto &vec = bucket_it->second;
                auto it = std::ranges::remove(vec, entity).begin();
                vec.erase(it, vec.end());
                if (vec.empty()) {
                    m_buckets.erase(bucket_it);
                }
            }
            m_back_references.erase(it_back);
        }
        m_proxies.erase(entity);
    }

    void SpatialHashBroadphase::Update(Ecs::EntityId entity, const Math::AABB &new_aabb) {
        auto it_proxy = m_proxies.find(entity);
        if (it_proxy == m_proxies.end()) {
            BroadphaseProxy new_proxy{};
            new_proxy.entity = entity;
            new_proxy.aabb = new_aabb;
            Insert(new_proxy);
            return;
        }

        std::vector<CellKey> new_cells;
        BoxToCells(new_aabb, new_cells);

        auto &old_cells = m_back_references[entity];

        // fast -> identical cells set and updated to new aabb
        if (old_cells.size() == new_cells.size() &&
            std::equal(old_cells.begin(), old_cells.end(), new_cells.begin(),
                       [](const CellKey &a, const CellKey &b) { return a.x == b.x && a.y == b.y && a.z == b.z; })) {
            it_proxy->second.aabb = new_aabb;
            return;
        }

        // Remove form old cells not in new
        {
            std::unordered_set<CellKey, CellKeyHash> new_set(new_cells.begin(), new_cells.end());
            for (const auto &cell: old_cells) {
                if (!new_set.contains(cell)) {
                    auto bucket_it = m_buckets.find(cell);
                    if (bucket_it != m_buckets.end()) {
                        auto &vec = bucket_it->second;
                        auto it = std::ranges::remove(vec, entity).begin();
                        vec.erase(it, vec.end());
                        if (vec.empty()) {
                            m_buckets.erase(bucket_it);
                        }
                    }
                }
            }
        }

        // Add to new cells not in old
        {
            std::unordered_set<CellKey, CellKeyHash> old_set(old_cells.begin(), old_cells.end());
            for (const auto &cell: old_cells) {
                if (!old_set.contains(cell)) {
                    m_buckets[cell].push_back(entity);
                }
            }
        }
        // Update backreferences and proxy aabb
        old_cells = std::move(new_cells);
        it_proxy->second.aabb = new_aabb;
    }

    void SpatialHashBroadphase::Dedupe(std::vector<Ecs::EntityId> &vec) {
        std::ranges::sort(vec);
        vec.erase(std::ranges::unique(vec).begin(), vec.end());
    }

    void SpatialHashBroadphase::QueryAABB(const Math::AABB &area, std::vector<Ecs::EntityId> &out,
                                          const QueryFilter *filter) {
        out.clear();
        std::vector<CellKey> cells;
        BoxToCells(area, cells);

        for (const auto &cell: cells) {
            auto it = m_buckets.find(cell);
            if (it == m_buckets.end()) {
                continue;
            }
            const auto &vec = it->second;

            if (!filter) {
                out.insert(out.end(), vec.begin(), vec.end());
            } else {
                for (Ecs::EntityId entity: vec) {
                    auto proxy_it = m_proxies.find(entity);
                    if (proxy_it != m_proxies.end() && PassFilter(proxy_it->second, filter)) {
                        out.push_back(entity);
                    }
                }
            }
        }
        Dedupe(out);
    }
} // namespace
