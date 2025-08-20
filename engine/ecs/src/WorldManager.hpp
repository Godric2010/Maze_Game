#pragma once
#include "../include/IWorld.hpp"
#include "EntityManager.hpp"

namespace Engine::Ecs {
    class WorldManager final : public IWorld {
    public:
        WorldManager();

        ~WorldManager() override;

        EntityId CreateEntity() override;

        void DestroyEntity(EntityId entity) override;

    private:
        std::unique_ptr<EntityManager> m_entityManager;
    };
} // namespace
