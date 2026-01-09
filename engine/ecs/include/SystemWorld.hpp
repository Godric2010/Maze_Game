//
// Created by Sebastian Borsch on 16.09.25.
//

#pragma once

#include <string>
#include <utility>
#include <vector>
#include "World.hpp"

namespace Engine::Ecs {
    class SystemWorld {
    public:
        explicit SystemWorld(World* ecs_world) {
            m_world = ecs_world;
        }

        ~SystemWorld() = default;

        [[nodiscard]] EntityId CreateEntity(const std::string& name) const { return m_world->CreateEntity(name); }

        void DestroyEntity(const EntityId& entity) const { return m_world->DestroyEntity(entity); }

        [[nodiscard]] EntityId GetEntityByName(const std::string& name) const {
            return m_world->GetEntityByName(name);
        }

        template<typename T>
        void AddComponent(const EntityId entity, T component) {
            m_world->AddComponent<T>(entity, component);
        }

        template<typename T>
        void RemoveComponent(const EntityId entity) const {
            m_world->RemoveComponent<T>(entity);
        }

        template<typename T>
        T* GetComponent(const EntityId entity) const { return m_world->GetComponent<T>(entity); }

        template<typename T>
        std::vector<std::pair<T*, EntityId> > GetComponentsOfType() { return m_world->GetComponentsOfType<T>(); }

    private:
        World* m_world;
    };
}
