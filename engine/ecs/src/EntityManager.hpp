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

        /**
         * Reserve an entity and make it available for use. At this point, it is inactive and cannot be queried.
         * @return The EntityID of the newly reserved entity.
         */
        EntityId ReserveEntity();

        /**
         * Commit the entity to the world and enable it. It is now available for queries in world.
         * @param entity The entity to commit.
         */
        void CommitEntity(EntityId entity);

        /**
         * Get all entities that are alive in the world. Pending and inactive entities are not included.
         * @return
         */
        [[nodiscard]] std::vector<EntityId> GetAllActiveEntities() const;

        /**
         * Destroy an entity. Remove it from all pending and alive lists and free the index for further usage in another generation.
         * @param entity
         */
        void DestroyEntity(EntityId entity);

        /**
         * Check if an entity is alive in the world.
         * @param entity The entity to check.
         * @return True if the entity is alive and available for queries. False if it's pending or not existing.
         */
        [[nodiscard]] bool IsEntityAlive(EntityId entity) const;

        /**
         * Check if an entity is pending and waiting to be committed.
         * @param entity The entity to check.
         * @return True if the entity has status pending. False if it is alive or not existing.
         */
        [[nodiscard]] bool IsEntityPending(EntityId entity) const;

    private:

        /**
         * Contains the generation of each entity index, independent of their 'alive' and 'pending' status.
         */
        std::vector<uint32_t> m_generations;
        /**
         * Contains all entity indices that are free and available for reuse.
         */
        std::vector<uint64_t> m_free_entity_indices;

        /**
         * Contains all entity indices with the information if they are alive or not.
         */
        std::vector<uint8_t> m_alive_entities;

        /**
         * Contains all entity indices with the information if they are pending or not.
         */
        std::vector<uint8_t> m_pending_entities;
        uint64_t m_alive_count;

        void EnsureCapacity(uint64_t idx);
    };
} // namespace
