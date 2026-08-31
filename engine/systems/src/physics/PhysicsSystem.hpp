#pragma once
#include <memory>

#include "Collider.hpp"
#include "IEngineSystem.hpp"
#include "collision/ColliderCache.hpp"
#include "collision/CollisionQueryService.hpp"
#include "collision/IBroadphase.hpp"
#include "collision/MoverSolver.hpp"

namespace yarep::Systems::Physics {
    ECS_SYSTEM(PhysicsSystem, Physics, TAGS(ENGINE), DEPENDENCIES())

    class PhysicsSystem final : public ecs::IEngineSystem {
    public:
        PhysicsSystem();

        ~PhysicsSystem() override = default;

        void Initialize() override;

        void Run(float fixed_delta_time) override;

    private:
        const float m_epsilon = 1e-12f;

        Transform::TransformCache* m_transform_cache = nullptr;
        std::unique_ptr<yarep::Physics::Collision::ColliderCache> m_collider_cache;
        std::unique_ptr<yarep::Physics::Collision::IBroadphase> m_broadphase;
        std::unique_ptr<yarep::Physics::Collision::ICollisionQueryService> m_collision_query_service;

        std::unordered_map<ecs::EntityId, ecs::EntityId> m_collided_entities;
        std::unordered_map<ecs::EntityId, std::unordered_set<ecs::EntityId> > m_triggered_entities;

        void BuildBoxCollider(ecs::EntityId entity, Components::BoxCollider box_collider, const glm::vec3& position,
                              const glm::vec3& rotation, const
                              glm::vec3& scale) const;

        void BuildSphereCollider(ecs::EntityId entity, Components::SphereCollider sphere_collider,
                                 glm::vec3 position) const;

        void RunBroadphase(ecs::EntityId target_entity, float radius, const glm::vec3& position, glm::vec3 move_delta,
                           std::vector<ecs::EntityId>& blocking_candidates,
                           std::vector<ecs::EntityId>& trigger_candidates) const;

        void PerformCollisionSweep(ecs::EntityId target_entity, glm::vec3 position,
                                   glm::vec3 move_delta, float radius,
                                   const std::vector<ecs::EntityId>& blocking_candidates,
                                   glm::vec3* final_position);

        void DetectTriggerInteractions(glm::vec3 final_position, float radius,
                                       ecs::EntityId target_entity,
                                       const std::vector<ecs::EntityId>& trigger_candidates);

        void RaiseCollisionEvents(ecs::EntityId target_entity,
                                  const yarep::Physics::Collision::MoverResult& mover_result);

        void RaiseTriggerEvents(ecs::EntityId target_entity,
                                std::unordered_set<ecs::EntityId>& trigger_entities);


        static yarep::Physics::Math::AABB BuildSweptAabb(const glm::vec3& pos, const glm::vec3& rest,
                                                          float radius) noexcept;
    };
} // namespace
