#pragma once
#include <vector>
#include "EntityManager.hpp"

namespace Engine::Ecs {

    template<class T>
    class ComponentPool {
    public:
        ComponentPool();

        ~ComponentPool();

        T &Add(EntityId entity, T value);

        void Remove(EntityId entity);

        [[nodiscard]] bool Contains(EntityId entity) const;

        T &Get(EntityId entity);

        const T &Get(EntityId entity) const;

        template<class Fn>
        void ForEach(Fn &&fn);

        [[nodiscard]] std::size_t Count() const;

    private:
        const uint64_t m_none;
        std::vector<T> m_denseComponents;
        std::vector<EntityId> m_denseEntities;
        std::vector<uint64_t> m_sparseToDense;
    };
} // namespace
