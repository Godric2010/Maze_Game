//
// Created by Godri on 8/17/2025.
//
#pragma once

#include <ranges>
#include "../include/ComponentEventBus.hpp"

namespace Engine::Ecs
{
    inline ComponentManager::ComponentManager() = default;

    inline ComponentManager::~ComponentManager() = default;

    template <typename T>
    T& ComponentManager::AddComponent(EntityId entity, T component)
    {
        return GetPool<T>().Add(entity, std::move(component));
    }

    template <typename T>
    void ComponentManager::RemoveComponent(EntityId entity)
    {
        return GetPool<T>().Remove(entity);
    }

    template <typename T>
    T* ComponentManager::GetComponent(EntityId entity)
    {
        return GetPool<T>().Get(entity);
    }

    template <typename T>
    const T& ComponentManager::GetComponent(EntityId entity) const
    {
        return GetPool<T>().Get(entity);
    }

    template <typename T>
    bool ComponentManager::HasComponent(EntityId entity) const
    {
        auto pool = GetPoolConst<T>();
        if (pool == nullptr)
        {
            return false;
        }
        return pool->Contains(entity);
    }


    template <typename T>
    std::vector<EntityId> ComponentManager::GetEntitiesWithComponent()
    {
        auto& pool = GetPool<T>();
        std::vector<EntityId> entities;
        entities.reserve(pool.Count());
        pool.ForEach([&](const EntityId entity, T val)
        {
            entities.push_back(entity);
        });
        return entities;
    }

    template <typename T>
    ComponentTypeId ComponentManager::RegisterType()
    {
        const auto id = TypeId<T>();
        if (m_component_meta.contains(id))
        {
            return id;
        }
  
        CreatePool<T>(id);

        m_component_meta.emplace(id, ComponentMeta{
                                     sizeof(T),
                                     [](ComponentManager& cm, EntityId entity, const void* p)
                                     {
                                         const T& val = *static_cast<const T*>(p);
                                         cm.GetPool<T>().Add(entity, val);
                                     },
                                     [](ComponentManager& cm, EntityId entity, const void* p)
                                     {
                                         const T& val = *static_cast<const T*>(p);
                                         auto* t = cm.GetPool<T>().Get(entity);
                                         if (t)
                                         {
                                             *t = val;
                                         }
                                         else
                                         {
                                             cm.GetPool<T>().Add(entity, val);
                                         }
                                     },
                                     [](ComponentManager& cm, EntityId entity)
                                     {
                                         cm.GetPool<T>().Remove(entity);
                                     },
                                     [](ComponentManager& cm, EntityId entity)
                                     {
                                         return cm.GetPool<T>().Contains(entity);
                                     },
                                     [](ComponentEventBus& event_bus, EntityId entity, const void* data)
                                     {
                                         event_bus.RaiseAddComponentEvent<T>(entity, *static_cast<const T*>(data));
                                     },
                                     [](ComponentEventBus& event_bus, EntityId entity)
                                     {
                                         event_bus.RaiseRemoveComponentEvent<T>(entity);
                                     }
                                 });
        return id;
    }

    template <typename T>
    ComponentTypeId ComponentManager::GetComponentTypeId() const
    {
        const auto id = TypeId<T>();
        if (m_component_meta.contains(id))
        {
            return id;
        }
        throw std::invalid_argument("ComponentManager::GetComponentTypeId: Component Type is not registered");
    }


    inline void ComponentManager::AddById(const EntityId entity, const ComponentTypeId component_type,
                                          const void* bytes)
    {
        const auto it = m_component_meta.find(component_type);
        if (it == m_component_meta.end())
        {
            return;
        }
        it->second.add(*this, entity, bytes);
    }

    inline void ComponentManager::SetById(const EntityId entity, const ComponentTypeId component_type,
                                          const void* bytes)
    {
        const auto it = m_component_meta.find(component_type);
        if (it == m_component_meta.end())
        {
            return;
        }
        it->second.set(*this, entity, bytes);
    }

    inline void ComponentManager::RemoveById(const EntityId entity, const ComponentTypeId component_type)
    {
        const auto it = m_component_meta.find(component_type);
        if (it == m_component_meta.end())
        {
            return;
        }
        it->second.remove(*this, entity);
    }

    inline bool ComponentManager::ContainsById(const EntityId entity, const ComponentTypeId component_type)
    {
        const auto it = m_component_meta.find(component_type);
        if (it == m_component_meta.end())
        {
            return false;
        }
        return it->second.contains(*this, entity);
    }


    inline void ComponentManager::OnDestroyEntity(const EntityId entity) const
    {
        for (const auto& component_pool : m_pools)
        {
            if (!component_pool->Contains(entity))
            {
                continue;
            }
            component_pool->Remove(entity);
        }
    }

    inline ComponentMeta ComponentManager::GetComponentMeta(const ComponentTypeId component_type_id) const
    {
        return m_component_meta.at(component_type_id);
    }

    template <typename T>
    void ComponentManager::CreatePool(ComponentTypeId id)
    {
        if (m_pools.size() <= id)
        {
            m_pools.resize(id + 1);
        }
        if (!m_pools[id])
        {
            m_pools[id] = std::make_unique<TypedComponentPool<T>>(id);
        }
    }

    template <typename T>
    TypedComponentPool<T>& ComponentManager::GetPool()
    {
        const auto component_id = TypeId<T>();
        if (m_pools.size() <= component_id)
        {
            throw std::invalid_argument("ComponentManager::GetPool: Component Pool does not exist");
        }

        auto& component_pool = m_pools[component_id];
        return *static_cast<TypedComponentPool<T>*>(component_pool.get());
    }

    template <typename T>
    const TypedComponentPool<T>* ComponentManager::GetPoolConst() const
    {
        const auto component_id = TypeId<T>();
        if (m_pools.size() <= component_id)
        {
            return nullptr;
        }
        auto& component_pool = m_pools[component_id];
        return static_cast<const TypedComponentPool<T>*>(component_pool.get());
    }


    inline std::vector<ComponentMeta> ComponentManager::GetAllComponentsOfEntity(EntityId entity) const
    {
        std::vector<ComponentMeta> components;
        for (const auto& pool : m_pools)
        {
            if (!pool->Contains(entity))
            {
                continue;
            }
            auto component_id = pool->GetComponentTypeId();
            components.push_back(m_component_meta.at(component_id));
        }
        return components;
    }

    inline ComponentTypeId ComponentManager::NextComponentTypeId()
    {
        static ComponentTypeId next = 0;
        return next++;
    }

    template <class T>
    ComponentTypeId ComponentManager::TypeId()
    {
        static ComponentTypeId id = NextComponentTypeId();
        return id;
    }
} // ECS
