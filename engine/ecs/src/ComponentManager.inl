//
// Created by Godri on 8/17/2025.
//

#include <ranges>

#include "ComponentManager.hpp"

namespace Engine::Ecs {
    inline ComponentManager::ComponentManager() = default;

    inline ComponentManager::~ComponentManager() = default;

    template<typename T>
    T &ComponentManager::AddComponent(EntityId entity, T component) {
        return getPool<T>().Add(entity, std::move(component));
    }

    template<typename T>
    void ComponentManager::RemoveComponent(EntityId entity) {
        return getPool<T>().Remove(entity);
    }

    template<typename T>
    T &ComponentManager::GetComponent(EntityId entity) {
        return getPool<T>().Get(entity);
    }

    template<typename T>
    const T &ComponentManager::GetComponent(EntityId entity) const {
        return getPool<T>().Get(entity);
    }

    template<typename T>
    bool ComponentManager::HasComponent(EntityId entity) const {
        return getPoolConst<T>().Contains(entity);
    }

    template<typename T>
    std::vector<EntityId> ComponentManager::GetEntitiesWithComponent() {
        auto &pool = getPool<T>();
        std::vector<EntityId> entities;
        entities.reserve(pool.Count());
        pool.ForEach([&](const EntityId entity, T val) {
            entities.push_back(entity);
        });
        return entities;
    }

    inline void ComponentManager::OnDestroyEntity(const EntityId entity) const {
        for (const auto &component_pool: m_pool | std::views::values) {
            if (component_pool->Contains(entity)) {
                component_pool->Remove(entity);
            }
        }
    }

    template<typename T>
    TypedComponentPool<T> &ComponentManager::getPool() {
        auto key = std::type_index(typeid(T));
        auto it = m_pool.find(key);
        if (it == m_pool.end()) {
            it = m_pool.emplace(key, std::make_unique<TypedComponentPool<T> >()).first;
        }
        return *static_cast<TypedComponentPool<T> *>(it->second.get());
    }

    template<typename T>
    const TypedComponentPool<T> &ComponentManager::getPoolConst() const {
        auto key = std::type_index(typeid(T));
        auto it = m_pool.find(key);
        if (it == m_pool.end()) {
            throw std::runtime_error("No such component");
        }

        return *static_cast<const TypedComponentPool<T> *>(it->second.get());
    }
} // ECS
