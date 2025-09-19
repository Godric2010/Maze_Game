//
// Created by Godri on 8/17/2025.
//

#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>

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

    private:
        template<typename T>
        TypedComponentPool<T> &GetPool();

        template<typename T>
        const TypedComponentPool<T> *GetPoolConst() const;


        mutable std::unordered_map<std::type_index, std::unique_ptr<IComponentPool> > m_pool;
        std::unordered_map<ComponentTypeId, ComponentMeta> m_component_meta;
        std::unordered_map<ComponentTypeId, std::type_index> m_id_to_index;
    };
} // ECS

#include "ComponentManager.inl"
