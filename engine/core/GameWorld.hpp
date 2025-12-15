//
// Created by Sebastian Borsch on 16.09.25.
//

#pragma once

#include "World.hpp"

namespace Engine::Core {
    class GameWorld {
    public:
        explicit GameWorld(Ecs::World* ecs_world) {
            m_world = ecs_world;
        }

        ~GameWorld() = default;

        [[nodiscard]] Ecs::EntityId CreateEntity(const std::string& name) const { return m_world->CreateEntity(name); }

        void DestroyEntity(const Ecs::EntityId& entity) const { return m_world->DestroyEntity(entity); }

        [[nodiscard]] Ecs::EntityId GetEntityByName(const std::string& name) const {
            return m_world->GetEntityByName(name);
        }

        template<typename T>
        void AddComponent(const Ecs::EntityId entity, T component) {
            m_world->AddComponent<T>(entity, component);
        }

        template<typename T>
        void RemoveComponent(const Ecs::EntityId entity) const {
            m_world->RemoveComponent<T>(entity);
        }

        template<typename T>
        T* GetComponent(const Ecs::EntityId entity) const { return m_world->GetComponent<T>(entity); }

        template<typename T>
        std::vector<std::pair<T*, Ecs::EntityId> > GetComponentsOfType() { return m_world->GetComponentsOfType<T>(); }

    private:
        Ecs::World* m_world;
    };
}
