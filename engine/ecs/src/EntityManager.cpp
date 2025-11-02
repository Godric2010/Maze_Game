#include "EntityManager.hpp"

namespace Engine::Ecs {
    EntityManager::EntityManager() {
        // Never use index 0 since it is the indicator for a non-existing entity
        m_generations.push_back(1);
        m_alive_entities.push_back(0);
        m_pending_entities.push_back(0);
        m_alive_count = 0;
        m_next_idx = 1;
    }

    EntityManager::~EntityManager() {
        m_generations.clear();
        m_alive_entities.clear();
        m_pending_entities.clear();
        m_free_entity_indices.clear();
    }


    EntityId EntityManager::ReserveEntity(const std::string &name) {
        uint64_t idx;
        if (!m_free_entity_indices.empty()) {
            idx = GetEntityIndex(m_free_entity_indices.back());
            m_free_entity_indices.pop_back();
        } else {
            idx = m_next_idx++;
            EnsureCapacity(idx);
        }

        m_pending_entities[idx] = 1;

        const uint64_t generation = m_generations[idx] & GENRATION_MASK;
        const auto entity = (generation << INDEX_BITS) | (idx & INDEX_MASK);

        if (m_entities_lookup.contains(name)) {
            throw std::runtime_error("Entity with the name " + name + " already exists");
        }
        m_entities_lookup.emplace(name, entity);
        m_reverse_lookup.emplace(entity, name);

        return entity;
    }

    void EntityManager::CommitEntity(const EntityId entity) {
        if (entity == INVALID_ENTITY_ID) {
            return;
        }

        const uint64_t idx = GetEntityIndex(entity);
        if (idx >= m_generations.size()) {
            return;
        }

        const uint64_t generation = GetEntityGenration(entity);
        if ((m_generations[idx] & GENRATION_MASK) != generation) {
            return;
        }

        if (m_pending_entities[idx]) {
            m_pending_entities[idx] = 0;
            if (!m_alive_entities[idx]) {
                m_alive_entities[idx] = 1;
                ++m_alive_count;
            }
        }
    }

    void EntityManager::DestroyEntity(const EntityId entity) {
        if (!IsEntityAlive(entity) && !IsEntityPending(entity)) {
            return;
        }
        const uint64_t idx = GetEntityIndex(entity);
        if (idx >= m_generations.size()) { return; }

        if (m_alive_entities[idx]) {
            m_alive_entities[idx] = 0;
            if (m_alive_count > 0) {
                --m_alive_count;
            }
        }
        if (m_pending_entities[idx]) {
            m_pending_entities[idx] = 0;
        }

        uint64_t gen = m_generations[idx] & GENRATION_MASK;
        gen = (gen + 1u) & GENRATION_MASK;
        m_generations[idx] = gen;
        m_generations[idx] = static_cast<uint32_t>(gen);
        m_free_entity_indices.push_back(entity);

        const auto name = m_reverse_lookup.find(entity)->second;
        m_entities_lookup.erase(name);
        m_reverse_lookup.erase(entity);
    }

    bool EntityManager::IsEntityAlive(const EntityId entity) const {
        if (entity == INVALID_ENTITY_ID) {
            return false;
        }
        const uint64_t idx = GetEntityIndex(entity);
        if (idx >= m_generations.size()) {
            return false;
        }
        const uint64_t gen = GetEntityGenration(entity);
        if ((m_generations[idx] & GENRATION_MASK) != gen) {
            return false;
        }
        return m_alive_entities[idx] != 0;
    }

    bool EntityManager::IsEntityPending(EntityId entity) const {
        if (entity == INVALID_ENTITY_ID) {
            return false;
        }
        const uint64_t idx = GetEntityIndex(entity);
        if (idx >= m_generations.size()) {
            return false;
        }
        const uint64_t gen = GetEntityGenration(entity);
        if ((m_generations[idx] & GENRATION_MASK) != gen) {
            return false;
        }
        return m_pending_entities[idx] != 0;
    }

    EntityId EntityManager::GetEntityByName(const std::string &name) const {
        if (m_entities_lookup.contains(name)) {
            return m_entities_lookup.at(name);
        }
        return INVALID_ENTITY_ID;
    }

    std::vector<EntityId> EntityManager::GetAllActiveEntities() const {
        std::vector<EntityId> result;
        result.reserve(m_alive_count);

        for (uint64_t i = 1; i < m_generations.size(); ++i) {
            if (m_alive_entities[i]) {
                const uint64_t gen = m_generations[i] & GENRATION_MASK;
                const auto entity = (gen << INDEX_BITS) | (i & INDEX_MASK);
                result.push_back(entity);
            }
        }

        return result;
    }

    void EntityManager::EnsureCapacity(const uint64_t idx) {
        const auto need = static_cast<size_t>(idx + 1);
        const auto size = m_generations.size();
        if (size < need) {
            const auto new_size = std::max(need, size * 2);
            m_generations.resize(new_size, 0);
            m_alive_entities.resize(new_size, 0);
            m_pending_entities.resize(new_size, 0);
        }
    }
} // namespace
