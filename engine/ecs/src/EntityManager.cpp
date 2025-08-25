#include "EntityManager.hpp"

namespace Engine::Ecs {
    EntityManager::EntityManager() {
        // Never use index 0 since it is the indicator for a non-existing entity
        m_generations.push_back(1);
    }

    EntityManager::~EntityManager() {
        m_generations.clear();
        m_generations.clear();
    }


    EntityId EntityManager::GenerateEntity() {
        uint64_t idx;
        if (!m_freeEntities.empty()) {
            idx = m_freeEntities.back();
            m_freeEntities.pop_back();
        } else {
            idx = static_cast<uint64_t>(m_generations.size());
            m_generations.push_back(0);
        }
        const auto entity = ((m_generations[idx] & GENRATION_MASK) << INDEX_BITS) | (idx & INDEX_MASK);
        return entity;
    }

    std::vector<EntityId> EntityManager::GetAllActiveEntities() const {
        auto entities = std::vector<EntityId>(m_generations.size());
        for (uint64_t i = 0; i < m_generations.size(); ++i) {
            entities[i] = (m_generations[i]&GENRATION_MASK) << INDEX_BITS | (i & INDEX_MASK);
        }
        return entities;
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
        return m_generations[idx] == gen;
    }

    void EntityManager::DestroyEntity(const EntityId entity) {
        if (!IsEntityAlive(entity)) {
            return;
        }
        const uint64_t idx = GetEntityIndex(entity);
        uint64_t gen = m_generations[idx];
        gen = (gen + 1u) & GENRATION_MASK;
        m_generations[idx] = gen;
        m_freeEntities.push_back(entity);
    }
} // namespace
