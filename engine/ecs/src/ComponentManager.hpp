//
// Created by Godri on 8/17/2025.
//

#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>

#include "../include/ComponentEventBus.hpp"
#include "ComponentPool.hpp"
#include "Entity.hpp"

namespace Engine::Ecs {
    using ComponentTypeId = std::size_t;

    class ComponentManager;

    struct ComponentMeta {
        std::size_t size;

        void (*add)(ComponentManager &, EntityId, const void *bytes);

        void (*set)(ComponentManager &, EntityId, const void *bytes);

        void (*remove)(ComponentManager &, EntityId);

        bool (*contains)(ComponentManager &, EntityId);

        void (*on_add_event)(ComponentEventBus &, EntityId, const void *) = nullptr;

        void (*on_remove_event)(ComponentEventBus &, EntityId) = nullptr;
    };


    /**
     * @class ComponentManager
     * @brief Manages components associated with entities in an Entity-Component-System (ECS) architecture.
     *
     * This class handles the addition, removal, and retrieval of components tied to entities. Components are
     * stored in specialized pools for efficient access and manipulation. ComponentManager ensures
     * type-safe operations and provides utilities to query component associations.
     */
    class ComponentManager {
    public:
        ComponentManager();

        ~ComponentManager();

        template<typename T>
        T &AddComponent(EntityId entity, T component);

        template<typename T>
        void RemoveComponent(EntityId entity);

        template<typename T>
        T *GetComponent(EntityId entity);

        template<typename T>
        const T &GetComponent(EntityId entity) const;

        template<typename T>
        bool HasComponent(EntityId entity) const;

        template<typename T>
        std::vector<EntityId> GetEntitiesWithComponent();

        template<typename T>
        ComponentTypeId RegisterType();

        template<typename T>
        ComponentTypeId GetComponentTypeId() const;

        void AddById(EntityId entity, ComponentTypeId component_type, const void *bytes);

        void SetById(EntityId entity, ComponentTypeId component_type, const void *bytes);

        void RemoveById(EntityId entity, ComponentTypeId component_type);

        bool ContainsById(EntityId entity, ComponentTypeId component_type);

        void OnDestroyEntity(EntityId entity) const;

        ComponentMeta GetComponentMeta(ComponentTypeId component_type_id) const;

        std::vector<ComponentMeta> GetAllComponentsOfEntity(EntityId entity) const;

    private:
        template<typename T>
        void CreatePool(ComponentTypeId component_type_id);

        template<typename T>
        TypedComponentPool<T> &GetPool();

        template<typename T>
        const TypedComponentPool<T> *GetPoolConst() const;


        mutable std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentPool> > m_pool;
        std::unordered_map<ComponentTypeId, ComponentMeta> m_component_meta;
        std::unordered_map<std::type_index, ComponentTypeId> m_type_index_to_id;
    };
} // ECS

#include "ComponentManager.inl"
