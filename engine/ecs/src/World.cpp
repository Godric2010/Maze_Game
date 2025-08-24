#include "World.hpp"
#include "ComponentManager.hpp"

namespace Engine::Ecs {
    struct World::WorldImpl {
        std::unique_ptr<EntityManager> m_entityManager;
        std::unique_ptr<ComponentManager> m_componentManager;

        WorldImpl() {
            m_entityManager = std::make_unique<EntityManager>();
            m_componentManager = std::make_unique<ComponentManager>();
        }
    };

    World::World() : m_impl(std::make_unique<WorldImpl>()) {
        m_impl->m_entityManager = std::make_unique<EntityManager>();
        m_impl->m_componentManager = std::make_unique<ComponentManager>();
    }

    World::~World() = default;

    EntityId World::CreateEntity() const {
        const auto entity = m_impl->m_entityManager->GenerateEntity();
        return entity;
    }

    template<typename T>
    bool World::AddComponent(EntityId entity, T component) {
        if (!m_impl->m_entityManager->IsEntityAlive(entity)) {
            return false;
        }

        if (m_impl->m_componentManager->HasComponent<T>(entity)) {
            return true;
        }

        return m_impl->m_componentManager->AddComponent(entity, component);
    }

    template<typename T>
    T &World::GetComponent(const EntityId entity) {
        if (!m_impl->m_entityManager->IsEntityAlive(entity)) {
            return nullptr;
        }

        if (!m_impl->m_componentManager->HasComponent<T>(entity)) {
            return nullptr;
        }

        return m_impl->m_componentManager->GetComponent<T>(entity);
    }


    void World::DestroyEntity(const EntityId entity) const {
        m_impl->m_componentManager->OnDestroyEntity(entity);
        m_impl->m_entityManager->DestroyEntity(entity);
    }

    template<typename T>
    bool World::RemoveComponent(const EntityId entity) const {
        if (!m_impl->m_entityManager->IsEntityAlive(entity)) {
            return false;
        }

        if (!m_impl->m_componentManager->HasComponent<T>(entity)) {
            return true;
        }

        m_impl->m_componentManager->RemoveComponent<T>(entity);
        return true;
    }
} // namespace
