#pragma once
#include "World.hpp"

namespace Engine::SceneManagement {
    class SceneWorld {
    public:
        explicit SceneWorld(Ecs::World& world) : m_world(world) {
        }

        ~SceneWorld() = default;

        [[nodiscard]] Ecs::EntityId CreateEntity(const std::string& name) const {
            return m_world.CreateEntity(name);
        };

        void DestroyEntity(const Ecs::EntityId entity) const {
            m_world.DestroyEntity(entity);
        };

        [[nodiscard]] Ecs::EntityId GetEntityByName(const std::string& name) const {
            return m_world.GetEntityByName(name);
        };

        template<typename T>
        void AddComponent(Ecs::EntityId entity, T component) {
            m_world.AddComponent(entity, component);
        }

        template<typename T>
        void RemoveComponent(const Ecs::EntityId entity) const {
            m_world.RemoveComponent<T>(entity);
        }

        template<typename T>
        T* GetComponent(const Ecs::EntityId entity) const {
            return m_world.GetComponent<T>(entity);
        }

        template<typename T>
        std::vector<std::pair<T*, Ecs::EntityId>> GetComponentsOfType() const {
            return m_world.GetComponentsOfType<T>();
        }

    private:
        Ecs::World& m_world;
    };
} // namespace
