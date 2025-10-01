//
// Created by Sebastian Borsch on 18.08.25.
//

#pragma once
#include <memory>

#include "../src/CommandBuffer.hpp"
#include "../src/EntityManager.hpp"

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
            T* GetComponent(EntityId entity);

            template<typename T>
            std::vector<std::pair<T *, EntityId>> GetComponentsOfType();

            ComponentEventBus *GetEventBus() const {
                return m_component_event_bus.get();
            }

            const ComponentEventBus *GetEventBusConst() const {
                return m_component_event_bus.get();
            }

        private:
            struct WorldImpl;
            std::unique_ptr<WorldImpl> m_impl;
            std::unique_ptr<CommandBuffer> m_command_buffer;
            std::unique_ptr<ComponentEventBus> m_component_event_bus;
    };
}

#include "../src/World.inl"
