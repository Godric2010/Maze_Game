#pragma once
#include "World.hpp"

namespace yarep::scene_management {
    class SceneWorld {
    public:
        explicit SceneWorld(ecs::World& world) : m_world(world) {
        }

        ~SceneWorld() = default;

        [[nodiscard]] ecs::EntityId CreateEntity(const std::string& name) const {
            return m_world.CreateEntity(name);
        };

        void DestroyEntity(const ecs::EntityId entity) const {
            m_world.DestroyEntity(entity);
        };

        [[nodiscard]] ecs::EntityId GetEntityByName(const std::string& name) const {
            return m_world.GetEntityByName(name);
        };

        template<typename T>
        void AddComponent(ecs::EntityId entity, T component) {
            m_world.AddComponent(entity, component);
        }

        template<typename T>
        void RemoveComponent(const ecs::EntityId entity) const {
            m_world.RemoveComponent<T>(entity);
        }

        template<typename T>
        T* GetComponent(const ecs::EntityId entity) const {
            return m_world.GetComponent<T>(entity);
        }

        template<typename T>
        std::vector<std::pair<T*, ecs::EntityId>> GetComponentsOfType() const {
            return m_world.GetComponentsOfType<T>();
        }

    private:
        ecs::World& m_world;
    };
} // namespace
