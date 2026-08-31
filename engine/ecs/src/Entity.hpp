//
// Created by Sebastian Borsch on 20.08.25.
//

#pragma once
#include <cstdint>
#include "Ecs/Types.hpp"

namespace yarep::ecs {
    inline constexpr EntityId invalid_entity_id = 0;

    inline constexpr uint64_t index_bits = 40;
    inline constexpr uint64_t genration_bits = 24;
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
    static constexpr uint64_t index_mask = (1ull << index_bits) - 1u;

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
    static constexpr uint64_t genration_mask = (1u << genration_bits) - 1u;

    /**
     * Get the underlying index of the entity
     * @param entity The entity to extract the index from
     * @return The index of the entity
     */
    inline uint64_t GetEntityIndex(const EntityId entity) {
        return (entity & index_mask);
    }

    /**
     * Get the underlying generation of the entity
     * @param entity The entity to extract the generation from
     * @return The generation of the entity.
     */
    inline uint64_t GetEntityGenration(const EntityId entity) {
        return (entity >> index_bits) & genration_mask;
    }
}
