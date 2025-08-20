#pragma once
#include <vector>
#include "Entity.hpp"


namespace Engine::Ecs {
    /**
     * @brief Manages the creation, destruction, and querying of entity IDs.
     *
     * The EntityManager is responsible for generating unique entity IDs, tracking
     * their lifecycle, and reusing IDs when entities are destroyed. Each entity ID
     * contains an index and a generation, which allows managing the reuse of IDs
     * safely without collisions.
     */
    class EntityManager {
    public:
        EntityManager();

        ~EntityManager();

        EntityId GenerateEntity();

        void DestroyEntity(EntityId entity);

        [[nodiscard]] bool IsEntityAlive(EntityId entity) const;

    private:
        std::vector<uint64_t> m_generations;
        std::vector<uint64_t> m_freeEntities;
    };
} // namespace
