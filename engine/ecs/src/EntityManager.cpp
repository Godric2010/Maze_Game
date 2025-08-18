#include "EntityManager.hpp"

namespace Engine::Ecs {

    EntityManager::EntityManager() {

    }

    EntityManager::~EntityManager() {

    }


    EntityId EntityManager::GenerateEntity() {
       return 1;
    }

    bool EntityManager::IsEntityAlive(EntityId entity) {
        return true;
    }

    void EntityManager::DestroyEntity(EntityId entity) {

    }



} // namespace