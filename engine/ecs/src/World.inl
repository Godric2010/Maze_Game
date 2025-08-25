#pragma once
#include "World.hpp"
#include "ComponentManager.hpp"

namespace Engine::Ecs {
    struct World::WorldImpl {
        std::unique_ptr<EntityManager> EntityManager;
        std::unique_ptr<ComponentManager> ComponentManager;
    };

    inline World::World() : m_impl(std::make_unique<WorldImpl>()) {
        m_impl->EntityManager = std::make_unique<EntityManager>();
        m_impl->ComponentManager = std::make_unique<ComponentManager>();
    }

    inline World::~World() = default;

    inline EntityId World::CreateEntity() const {
        const auto entity = m_impl->EntityManager->GenerateEntity();
        return entity;
    }

    template<typename T>
    std::optional<std::reference_wrapper<T>> World::AddComponent(EntityId entity, T component) {
        if (!m_impl->EntityManager->IsEntityAlive(entity)) {
            return std::nullopt;
        }

        if (m_impl->ComponentManager->HasComponent<T>(entity)) {
            return *m_impl->ComponentManager->GetComponent<T>(entity);
        }

        return m_impl->ComponentManager->AddComponent(entity, component);
    }

    template<typename T>
    T *World::GetComponent(const EntityId entity) {
        if (!m_impl->EntityManager->IsEntityAlive(entity)) {
            return nullptr;
        }

        if (!m_impl->ComponentManager->HasComponent<T>(entity)) {
            return nullptr;
        }

        return m_impl->ComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    std::vector<std::pair<T *, EntityId> > World::GetComponentsOfType() {
        const auto entities = m_impl->EntityManager->GetAllActiveEntities();
        std::vector<std::pair<T*, EntityId> > components;
        for (const auto &entity : entities) {
            if (m_impl->ComponentManager->HasComponent<T>(entity)) {
                auto component = m_impl->ComponentManager->GetComponent<T>(entity);
                components.emplace_back(std::make_pair(component, entity));
            }
        }
        return components;
    }



    inline void World::DestroyEntity(const EntityId entity) const {
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
