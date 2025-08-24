#include "World.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

namespace Engine::Ecs {
    struct World::WorldImpl {
        std::unique_ptr<EntityManager> EntityManager;
        std::unique_ptr<ComponentManager> ComponentManager;
    };

    World::World() : m_impl(std::make_unique<WorldImpl>()) {
        m_impl->EntityManager = std::make_unique<EntityManager>();
        m_impl->ComponentManager = std::make_unique<ComponentManager>();
    }

    World::~World() = default;

    EntityId World::CreateEntity() const {
        const auto entity = m_impl->EntityManager->GenerateEntity();
        return entity;
    }

    template<typename T>
    bool World::AddComponent(EntityId entity, T component) {
        if (!m_impl->EntityManager->IsEntityAlive(entity)) {
            return false;
        }

        if (m_impl->ComponentManager->HasComponent<T>(entity)) {
            return true;
        }

        return m_impl->ComponentManager->AddComponent(entity, component);
    }

    template<typename T>
    T &World::GetComponent(const EntityId entity) {
        if (!m_impl->EntityManager->IsEntityAlive(entity)) {
            return nullptr;
        }

        if (!m_impl->ComponentManager->HasComponent<T>(entity)) {
            return nullptr;
        }

        return m_impl->ComponentManager->GetComponent<T>(entity);
    }


    void World::DestroyEntity(const EntityId entity) const {
        m_impl->ComponentManager->OnDestroyEntity(entity);
        m_impl->EntityManager->DestroyEntity(entity);
    }

    template<typename T>
    bool World::RemoveComponent(const EntityId entity) const {
        if (!m_impl->EntityManager->IsEntityAlive(entity)) {
            return false;
        }

        if (!m_impl->ComponentManager->HasComponent<T>(entity)) {
            return true;
        }

        m_impl->ComponentManager->RemoveComponent<T>(entity);
        return true;
    }
} // namespace
