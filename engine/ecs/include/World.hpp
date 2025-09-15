//
// Created by Sebastian Borsch on 18.08.25.
//

#pragma once
#include <memory>
#include <optional>

#include "Transform.hpp"
#include "../src/EntityManager.hpp"

namespace Engine::Ecs {

	class World {
		public:
			explicit World();

			~World();

			[[nodiscard]] EntityId CreateEntity() const;

			void DestroyEntity(EntityId entity) const;

			template<typename T>
			std::optional<std::reference_wrapper<T>> AddComponent(EntityId entity, T component);

			template<typename T>
			[[nodiscard]] bool RemoveComponent(EntityId entity) const;

			template<typename T>
			T* GetComponent(EntityId entity);

			template<typename T>
			std::vector<std::pair<T *, EntityId>> GetComponentsOfType();

		private:
			struct WorldImpl;
			std::unique_ptr<WorldImpl> m_impl;
	};
}

#include "../src/World.inl"
