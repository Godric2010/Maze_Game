//
// Created by Sebastian Borsch on 16.09.25.
//

#pragma once
#include <memory>
#include "World.hpp"

namespace Engine::Core {
    class GameWorld {
    public:
        explicit GameWorld(Ecs::World *ecs_world) {
            m_world = ecs_world;
        }

        ~GameWorld() = default;

        [[nodiscard]] Ecs::EntityId CreateEntity() const { return m_world->CreateEntity(); }

        void DestroyEntity(const Ecs::EntityId &entity) const { return m_world->DestroyEntity(entity); }

        template<typename T>
        std::optional<std::reference_wrapper<T> > AddComponent(const Ecs::EntityId entity, T component) {
            return m_world->AddComponent<T>(entity, component);
        }

        template<typename T>
        [[nodiscard]] bool RemoveComponent(const Ecs::EntityId entity) const {
            return m_world->RemoveComponent<T>(entity);
        }

        template<typename T>
        T *GetComponent(const Ecs::EntityId entity) const { return m_world->GetComponent<T>(entity); }

        template<typename T>
        std::vector<std::pair<T *, Ecs::EntityId> > GetComponentsOfType() { return m_world->GetComponentsOfType<T>(); }

    private:
        Ecs::World* m_world;
    };
}
