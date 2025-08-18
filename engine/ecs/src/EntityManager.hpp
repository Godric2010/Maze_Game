#pragma once
#include <vector>


namespace Engine::Ecs {
    using EntityId = uint64_t;
    inline constexpr EntityId INVALID_ENTITY_ID = 0;

    inline constexpr uint64_t INDEX_BITS = 40;
    inline constexpr uint64_t GENRATION_BITS = 24;
    // inline constexpr uint64_t INDEX_MASK = (1u << 40) - 1u;


    class EntityManager {
    public:
        EntityManager();

        ~EntityManager();

        EntityId GenerateEntity();

        void DestroyEntity(EntityId entity);

        bool IsEntityAlive(EntityId entity);

    private:
        std::vector<EntityId> m_entities;
    };
} // namespace
