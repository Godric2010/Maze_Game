//
// Created by Sebastian Borsch on 18.08.25.
//

#pragma once
#include <memory>

#include "PhysicsEventBus.hpp"
#include "../src/buffer/EcsEvent.hpp"
#include "../src/buffer/PhysicsEvent.hpp"
#include "../src/buffer/EventBuffer.hpp"

namespace Engine::Ecs {
    class World {
    public:
        explicit World();

        ~World();

        [[nodiscard]] EntityId CreateEntity() const;

        void DestroyEntity(EntityId entity) const;

        template<typename T>
        void AddComponent(EntityId entity, T component);

        template<typename T>
        void RemoveComponent(EntityId entity) const;

        void ApplyCommands() const;

        template<typename T>
        T *GetComponent(EntityId entity);

        template<typename T>
        std::vector<std::pair<T *, EntityId> > GetComponentsOfType();

        [[nodiscard]] ComponentEventBus *GetEventBus() const {
            return m_component_event_bus.get();
        }

        [[nodiscard]] const ComponentEventBus *GetEventBusConst() const {
            return m_component_event_bus.get();
        }

        [[nodiscard]] PhysicsEventBus *GetPhysicsEventBus() const {
            return m_physics_event_bus.get();
        }

        Buffer::EventBuffer<PhysicsEvent> *GetPhysicsEventBuffer() const {
            return m_physics_event_buffer.get();
        }

    private:
        struct WorldImpl;
        std::unique_ptr<WorldImpl> m_impl;
        std::unique_ptr<Buffer::EventBuffer<EcsEvent> > m_ecs_event_buffer;
        std::unique_ptr<Buffer::EventBuffer<PhysicsEvent> > m_physics_event_buffer;
        std::unique_ptr<ComponentEventBus> m_component_event_bus;
        std::unique_ptr<PhysicsEventBus> m_physics_event_bus;
    };
}

#include "../src/World.inl"
