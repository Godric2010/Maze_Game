#pragma once
#include <vector>


namespace Engine::Ecs {
    using EntityId = uint64_t;
    inline constexpr EntityId INVALID_ENTITY_ID = 0;

    inline constexpr uint64_t INDEX_BITS = 40;
    inline constexpr uint64_t GENRATION_BITS = 24;
    /**
     * @brief A bitmask used to extract the index portion of an entity ID.
     *
     * This mask is used in conjunction with specific bitwise operations
     * to isolate the index part of the entity ID. The index identifies
     * the position of an entity in the internal data structures, such
     * as the `m_generations` vector.
     *
     * The mask is computed as `(1u << INDEX_BITS) - 1u`, where `INDEX_BITS`
     * determines the number of bits allocated for the index in the entity ID.
     */
    static uint64_t INDEX_MASK = (1u << INDEX_BITS) - 1u;

    /**
     * @brief A bitmask used to extract the generation portion of an entity ID.
     *
     * This mask is used to isolate the generation part of an entity ID during
     * operations such as creation, validation, and destruction of entities.
     * The generation helps in distinguishing new entities from previously
     * deleted ones that may be recycled.
     *
     * The mask is computed as `(1u << GENRATION_BITS) - 1u`, where `GENRATION_BITS`
     * specifies the number of bits reserved for the generation in the entity ID.
     */
    static uint64_t GENRATION_MASK = (1u << GENRATION_BITS) - 1u;

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

        static uint64_t GetEntityIndex(EntityId entity);

        static uint64_t GetEntityGenration(EntityId entity);

    private:
        std::vector<uint64_t> m_generations;
        std::vector<uint64_t> m_freeEntities;
    };
} // namespace
