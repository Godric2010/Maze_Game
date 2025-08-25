//
// Created by Godri on 8/17/2025.
//

#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>

#include "ComponentPool.hpp"
#include "Entity.hpp"

namespace Engine::Ecs {
	class ComponentManager {
	public:
		ComponentManager();
		~ComponentManager();

		template<typename T>
		T& AddComponent(EntityId entity, T component);

		template<typename T>
		void RemoveComponent(EntityId entity);

		template<typename T>
		T *GetComponent(EntityId entity);

		template<typename T>
		const T& GetComponent(EntityId entity) const;

		template<typename T>
		bool HasComponent(EntityId entity) const;

		template<typename T>
		std::vector<EntityId> GetEntitiesWithComponent();

		void OnDestroyEntity(EntityId entity) const;

	private:
		mutable std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_pool;

		template<typename T>
		TypedComponentPool<T>& getPool();
		template<typename T>
		const TypedComponentPool<T> *getPoolConst() const;
	};
} // ECS

#include "ComponentManager.inl"