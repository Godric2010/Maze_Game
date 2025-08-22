#pragma once
#include <vector>
#include "EntityManager.hpp"

namespace Engine::Ecs {
    template<class T>
    class ComponentPool;

    template<class T>
    concept Component = std::is_trivially_destructible_v<T>;

    struct IComponentPool {
        virtual ~IComponentPool() = default;

        virtual void Remove(EntityId entity) = 0;

        [[nodiscard]] virtual bool Contains(EntityId entity) const = 0;
    };

    template<Component T>
    class TypedComponentPool final : public IComponentPool {
    public:
        T &Add(EntityId entity, T value) { return m_pool.Add(entity, value); }

        void Remove(EntityId entity) override { return m_pool.Remove(entity); };

        bool Contains(EntityId entity) const override { return m_pool.Contains(entity); };

        T &Get(EntityId entity) { return m_pool.Get(entity); };

        const T &Get(EntityId entity) const { return m_pool.Get(entity); };

        template<class Fn>
        void ForEach(Fn &&fn) { m_pool.ForEach(std::forward<Fn>(fn)); };

        [[nodiscard]] std::size_t Count() const { return m_pool.Count(); };

    private:
        ComponentPool<T> m_pool;
    };

    template<class T>
    class ComponentPool {
    public:
        ComponentPool<T>();

        ~ComponentPool<T>();

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
#include "ComponentPool.inl"
