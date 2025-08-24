//
// Created by Sebastian Borsch on 18.08.25.
//

#pragma once
#include "../src/EntityManager.hpp"

namespace Engine::Ecs {
    class World {
    public:
        World();
        ~World();

        [[nodiscard]] EntityId CreateEntity() const;

        template<typename T>
        bool AddComponent(EntityId entity, T component);

        template<typename T>
        T& GetComponent(EntityId entity);

        void DestroyEntity(EntityId entity) const;

        template<typename T>
        [[nodiscard]] bool RemoveComponent(EntityId entity) const;

    private:
        struct WorldImpl;
        std::unique_ptr<WorldImpl> m_impl;
    };
}
