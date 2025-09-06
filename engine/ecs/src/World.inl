#pragma once
#include "World.hpp"
#include "ComponentManager.hpp"

namespace Engine::Ecs {
    struct World::WorldImpl {
        std::unique_ptr<EntityManager> WorldEntityManager;
        std::unique_ptr<ComponentManager> WorldComponentManager;
    };

    inline World::World() : m_impl(std::make_unique<WorldImpl>()) {
        m_impl->WorldEntityManager = std::make_unique<EntityManager>();
        m_impl->WorldComponentManager = std::make_unique<ComponentManager>();
    }

    inline World::~World() = default;

    inline EntityId World::CreateEntity() const {
        const auto entity = m_impl->WorldEntityManager->GenerateEntity();
        return entity;
    }

    template<typename T>
    std::optional<std::reference_wrapper<T>> World::AddComponent(EntityId entity, T component) {
        if (!m_impl->WorldEntityManager->IsEntityAlive(entity)) {
            return std::nullopt;
        }

        if (m_impl->WorldComponentManager->HasComponent<T>(entity)) {
            return *m_impl->WorldComponentManager->GetComponent<T>(entity);
        }

        return m_impl->WorldComponentManager->AddComponent(entity, component);
    }

    template<typename T>
    T *World::GetComponent(const EntityId entity) {
        if (!m_impl->WorldEntityManager->IsEntityAlive(entity)) {
            return nullptr;
        }

        if (!m_impl->WorldComponentManager->HasComponent<T>(entity)) {
            return nullptr;
        }

        return m_impl->WorldComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    std::vector<std::pair<T *, EntityId> > World::GetComponentsOfType() {
        const auto entities = m_impl->WorldEntityManager->GetAllActiveEntities();
        std::vector<std::pair<T*, EntityId> > components;
        for (const auto &entity : entities) {
            if (m_impl->WorldComponentManager->HasComponent<T>(entity)) {
                auto component = m_impl->WorldComponentManager->GetComponent<T>(entity);
                components.emplace_back(std::make_pair(component, entity));
            }
        }
        return components;
    }



    inline void World::DestroyEntity(const EntityId entity) const {
        m_impl->WorldComponentManager->OnDestroyEntity(entity);
        m_impl->WorldEntityManager->DestroyEntity(entity);
    }

    template<typename T>
    bool World::RemoveComponent(const EntityId entity) const {
        if (!m_impl->WorldEntityManager->IsEntityAlive(entity)) {
            return false;
        }

        if (!m_impl->WorldComponentManager->HasComponent<T>(entity)) {
            return true;
        }

        m_impl->WorldComponentManager->RemoveComponent<T>(entity);
        return true;
    }
} // namespace
