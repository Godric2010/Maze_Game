#pragma once

#include "ComponentPool.hpp"

namespace Engine::Ecs {
    template<class T>
    ComponentPool<T>::ComponentPool() : m_none(std::numeric_limits<uint64_t>::max()), m_sparseToDense(1, m_none) {
    }

    template<class T>
    ComponentPool<T>::~ComponentPool() = default;

    template<class T>
    T &ComponentPool<T>::Add(const EntityId entity, T value) {
        if (entity == INVALID_ENTITY_ID) {
            throw std::invalid_argument("Cannot add component with invalid EntityId");
        }
        const uint64_t idx = GetEntityIndex(entity);
        if (idx >= m_sparseToDense.size()) {
            m_sparseToDense.resize(idx + 1, m_none);
        }

        if (Contains(entity)) {
            return m_denseComponents[m_sparseToDense[idx]];
        }

        const auto componentIndex = static_cast<uint64_t>(m_denseComponents.size());
        m_denseComponents.emplace_back(std::move(value));
        m_denseEntities.push_back(entity);
        m_sparseToDense[idx] = componentIndex;
        return m_denseComponents.back();
    }

    template<class T>
    void ComponentPool<T>::Remove(EntityId entity) {
        if (!Contains(entity)) {
            return;
        }
        const uint64_t idx = GetEntityIndex(entity);
        uint64_t componentIndex = m_sparseToDense[idx];
        uint64_t lastComponentIndex = m_denseComponents.size() - 1;

        if (componentIndex != lastComponentIndex) {
            std::swap(m_denseComponents[componentIndex], m_denseComponents[lastComponentIndex]);
            std::swap(m_denseEntities[componentIndex], m_denseEntities[lastComponentIndex]);

            const uint64_t swappedIndex = GetEntityIndex(m_denseEntities[componentIndex]);
            m_sparseToDense[swappedIndex] = componentIndex;
        }
        m_denseComponents.pop_back();
        m_denseEntities.pop_back();
        m_sparseToDense[idx] = m_none;
    }

    template<class T>
    bool ComponentPool<T>::Contains(const EntityId entity) const {
        const uint64_t idx = GetEntityIndex(entity);
        return idx < m_sparseToDense.size() && m_sparseToDense[idx] != m_none;
    }


    template<class T>
    T *ComponentPool<T>::Get(const EntityId entity) {
        if (!Contains(entity)) {
            throw std::out_of_range("Component for entity does not exist");
        }
        const uint64_t idx = GetEntityIndex(entity);
        return &m_denseComponents[m_sparseToDense[idx]];
    }

    template<class T>
    const T &ComponentPool<T>::Get(const EntityId entity) const {
        if (!Contains(entity)) {
            throw std::out_of_range("Component for entity does not exist");
        }
        const uint64_t idx = GetEntityIndex(entity);
        return m_denseComponents[m_sparseToDense[idx]];
    }

    template<class T>
    std::size_t ComponentPool<T>::Count() const {
        return m_denseComponents.size();
    }

    template<class T>
    template<class Fn>
    void ComponentPool<T>::ForEach(Fn &&fn) {
        for (size_t i = 0; i < m_denseComponents.size(); i++) {
            fn(m_denseEntities[i], m_denseComponents[i]);
        }
    }
} // namespace
