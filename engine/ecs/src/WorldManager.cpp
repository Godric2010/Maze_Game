#include "WorldManager.hpp"

namespace Engine::Ecs {
    WorldManager::WorldManager() {
        m_entityManager = std::make_unique<EntityManager>();
    }

    WorldManager::~WorldManager() = default;

    EntityId WorldManager::CreateEntity() {
        const EntityId entity = m_entityManager->GenerateEntity();
        return entity;
    }

    void WorldManager::DestroyEntity(const EntityId entity) {
        m_entityManager->DestroyEntity(entity);
    }
} // namespace
