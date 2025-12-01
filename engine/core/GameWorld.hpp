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


        void SubscribeToPhysicsEvent(const Ecs::PhysicsEventType event_type,
                                     const std::function<void(Ecs::EntityId, Ecs::EntityId)>& event_func) const {
            const auto physics_event_bus = m_world->GetPhysicsEventBus();
            switch (event_type) {
                case Ecs::PhysicsEventType::OnCollisionEnter:
                    physics_event_bus->SubscribeToOnCollisionEnter(event_func);
                    return;
                case Ecs::PhysicsEventType::OnCollisionExit:
                    physics_event_bus->SubscribeToOnCollisionExit(event_func);
                    return;
                case Ecs::PhysicsEventType::OnTriggerEnter:
                    physics_event_bus->SubscribeToOnTriggerEnter(event_func);
                    return;
                case Ecs::PhysicsEventType::OnTriggerExit:
                    physics_event_bus->SubscribeToOnTriggerExit(event_func);
                    return;
            }
        }

        template<typename T>
        void PushCommand(T cmd) {
            m_world->PushCommand(cmd);
        }

    private:
        Ecs::World* m_world;
    };
}
