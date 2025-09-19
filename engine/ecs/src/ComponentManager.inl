//
// Created by Godri on 8/17/2025.
//
#pragma once

#include <ranges>

namespace Engine::Ecs {
    inline ComponentManager::ComponentManager() = default;

    inline ComponentManager::~ComponentManager() = default;

    template<typename T>
    T &ComponentManager::AddComponent(EntityId entity, T component) {
        return GetPool<T>().Add(entity, std::move(component));
    }

    template<typename T>
    void ComponentManager::RemoveComponent(EntityId entity) {
        return GetPool<T>().Remove(entity);
    }

    template<typename T>
    T *ComponentManager::GetComponent(EntityId entity) {
        return GetPool<T>().Get(entity);
    }

    template<typename T>
    const T &ComponentManager::GetComponent(EntityId entity) const {
        return GetPool<T>().Get(entity);
    }

    template<typename T>
    bool ComponentManager::HasComponent(EntityId entity) const {
        auto pool = GetPoolConst<T>();
        if (pool == nullptr) {
            return false;
        }
        return pool->Contains(entity);
    }

    template<typename T>
    std::vector<EntityId> ComponentManager::GetEntitiesWithComponent() {
        auto &pool = GetPool<T>();
        std::vector<EntityId> entities;
        entities.reserve(pool.Count());
        pool.ForEach([&](const EntityId entity, T val) {
            entities.push_back(entity);
        });
        return entities;
    }

    template<typename T>
    ComponentTypeId ComponentManager::RegisterType() {
        const auto id = typeid(T).hash_code();
        if (m_component_meta.contains(id)) {
            return id;
        }

        m_component_meta.emplace(id, ComponentMeta{
                                     sizeof(T),
                                     [](ComponentManager &cm, EntityId entity, const void *p) {
                                         const T &val = *static_cast<const T *>(p);
                                         cm.GetPool<T>().Add(entity, val);
                                     },
                                     [](ComponentManager &cm, EntityId entity, const void *p) {
                                         const T &val = *static_cast<const T *>(p);
                                         auto *t = cm.GetPool<T>().Get(entity);
                                         if (t) {
                                             *t = val;
                                         } else {
                                             cm.GetPool<T>().Add(entity, val);
                                         }
                                     },
                                     [](ComponentManager &cm, EntityId entity) {
                                         cm.GetPool<T>().Remove(entity);
                                     },
                                     [](ComponentManager &cm, EntityId entity) {
                                         return cm.GetPool<T>().Contains(entity);
                                     },
                                 });
        m_id_to_index.emplace(id, std::type_index(typeid(T)));
        return id;
    }

    template<typename T>
    ComponentTypeId ComponentManager::GetComponentTypeId() const {
        const auto id = typeid(T).hash_code();
        if (m_component_meta.contains(id)) {
            return id;
        }
        throw std::invalid_argument("ComponentManager::GetComponentTypeId: Component Type is not registered");
    }


    inline void ComponentManager::AddById(const EntityId entity, const ComponentTypeId component_type,
                                          const void *bytes) {
        const auto it = m_component_meta.find(component_type);
        if (it == m_component_meta.end()) {
            return;
        }
        it->second.add(*this, entity, bytes);
    }

    inline void ComponentManager::SetById(const EntityId entity, const ComponentTypeId component_type,
                                          const void *bytes) {
        const auto it = m_component_meta.find(component_type);
        if (it == m_component_meta.end()) {
            return;
        }
        it->second.set(*this, entity, bytes);
    }

    inline void ComponentManager::RemoveById(const EntityId entity, const ComponentTypeId component_type) {
        const auto it = m_component_meta.find(component_type);
        if (it == m_component_meta.end()) {
            return;
        }
        it->second.remove(*this, entity);
    }

    inline bool ComponentManager::ContainsById(const EntityId entity, const ComponentTypeId component_type) {
        const auto it = m_component_meta.find(component_type);
        if (it == m_component_meta.end()) {
            return false;
        }
        return it->second.contains(*this, entity);
    }


    inline void ComponentManager::OnDestroyEntity(const EntityId entity) const {
        for (const auto &component_pool: m_pool | std::views::values) {
            if (component_pool->Contains(entity)) {
                component_pool->Remove(entity);
            }
        }
    }

    template<typename T>
    TypedComponentPool<T> &ComponentManager::GetPool() {
        auto key = std::type_index(typeid(T));
        auto it = m_pool.find(key);
        if (it == m_pool.end()) {
            it = m_pool.emplace(key, std::make_unique<TypedComponentPool<T> >()).first;
        }
        return *static_cast<TypedComponentPool<T> *>(it->second.get());
    }

    template<typename T>
    const TypedComponentPool<T> *ComponentManager::GetPoolConst() const {
        const auto key = std::type_index(typeid(T));
        const auto it = m_pool.find(key);
        if (it == m_pool.end()) {
            return nullptr;
        }

        return static_cast<const TypedComponentPool<T> *>(it->second.get());
    }
} // ECS
