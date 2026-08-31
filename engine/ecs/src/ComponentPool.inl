#pragma once

#include "ComponentPool.hpp"
#include <limits>

namespace yarep::ecs {
    template<class T>
    ComponentPool<
        T>::ComponentPool(const std::size_t component_type_id) : m_none(std::numeric_limits<uint64_t>::max()),
                                                                     m_sparse_to_dense(1, m_none) {
        m_component_type_id = component_type_id;
    }

    template<class T>
    ComponentPool<T>::~ComponentPool() = default;

    template<class T>
    T &ComponentPool<T>::Add(const EntityId entity, T value) {
        if (entity == invalid_entity_id) {
            throw std::invalid_argument("Cannot add component with invalid EntityId");
        }
        const uint64_t idx = GetEntityIndex(entity);
        if (idx >= m_sparse_to_dense.size()) {
            m_sparse_to_dense.resize(idx + 1, m_none);
        }

        if (Contains(entity)) {
            return m_dense_components[m_sparse_to_dense[idx]];
        }

        const auto component_index = static_cast<uint64_t>(m_dense_components.size());
        m_dense_components.emplace_back(std::move(value));
        m_dense_entities.push_back(entity);
        m_sparse_to_dense[idx] = component_index;
        return m_dense_components.back();
    }

    template<class T>
    void ComponentPool<T>::Remove(EntityId entity) {
        if (!Contains(entity)) {
            return;
        }
        const uint64_t idx = GetEntityIndex(entity);
        uint64_t component_index = m_sparse_to_dense[idx];
        uint64_t last_component_index = m_dense_components.size() - 1;

        if (component_index != last_component_index) {
            std::swap(m_dense_components[component_index], m_dense_components[last_component_index]);
            std::swap(m_dense_entities[component_index], m_dense_entities[last_component_index]);

            const uint64_t swapped_index = GetEntityIndex(m_dense_entities[component_index]);
            m_sparse_to_dense[swapped_index] = component_index;
        }
        m_dense_components.pop_back();
        m_dense_entities.pop_back();
        m_sparse_to_dense[idx] = m_none;
    }

    template<class T>
    bool ComponentPool<T>::Contains(const EntityId entity) const {
        const uint64_t idx = GetEntityIndex(entity);
        return idx < m_sparse_to_dense.size() && m_sparse_to_dense[idx] != m_none;
    }


    template<class T>
    T *ComponentPool<T>::Get(const EntityId entity) {
        if (!Contains(entity)) {
            throw std::out_of_range("Component for entity does not exist");
        }
        const uint64_t idx = GetEntityIndex(entity);
        return &m_dense_components[m_sparse_to_dense[idx]];
    }

    template<class T>
    const T &ComponentPool<T>::Get(const EntityId entity) const {
        if (!Contains(entity)) {
            throw std::out_of_range("Component for entity does not exist");
        }
        const uint64_t idx = GetEntityIndex(entity);
        return m_dense_components[m_sparse_to_dense[idx]];
    }

    template<class T>
    std::size_t ComponentPool<T>::Count() const {
        return m_dense_components.size();
    }

    template<class T>
    template<class Fn>
    void ComponentPool<T>::ForEach(Fn &&fn) {
        for (size_t i = 0; i < m_dense_components.size(); i++) {
            fn(m_dense_entities[i], m_dense_components[i]);
        }
    }
} // namespace
