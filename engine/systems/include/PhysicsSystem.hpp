#pragma once
#include <memory>
#include "ISystem.hpp"
#include "../../components/Collider.hpp"
#include "../../components/MotionIntent.hpp"
#include "collision/ColliderCache.hpp"
#include "collision/CollisionQueryService.hpp"
#include "collision/IBroadphase.hpp"
#include "collision/MoverSolver.hpp"

namespace Engine::Systems::Physics {
    ECS_SYSTEM(PhysicsSystem, Physics, [ENGINE])

    class PhysicsSystem final : public Ecs::IEngineSystem {
    public:
        PhysicsSystem();

        ~PhysicsSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        const float m_epsilon = 1e-6f;

        std::unique_ptr<Engine::Physics::Collision::ColliderCache> m_collider_cache;
        std::unique_ptr<Engine::Physics::Collision::IBroadphase> m_broadphase;
        std::unique_ptr<Engine::Physics::Collision::ICollisionQueryService> m_collision_query_service;

        std::unordered_map<Ecs::EntityId, Ecs::EntityId> m_collided_entities;
        std::unordered_map<Ecs::EntityId, std::unordered_set<Ecs::EntityId> > m_triggered_entities;

        void BuildBoxCollider(Ecs::EntityId entity, Components::BoxCollider box_collider, const glm::vec3& position,
                              const glm::vec3& rotation, const
                              glm::vec3& scale) const;

        void BuildSphereCollider(Ecs::EntityId entity, Components::SphereCollider sphere_collider,
                                 glm::vec3 position) const;

        bool TryGetMoveDelta(const glm::vec3& position, const glm::vec3& rotation,
                             const Components::MotionIntent* intent, float dt, glm::vec3* move_delta) const;

        void RunBroadphase(Ecs::EntityId target_entity, float radius, const glm::vec3& position, glm::vec3 move_delta,
                           std::vector<Ecs::EntityId>& blocking_candidates,
                           std::vector<Ecs::EntityId>& trigger_candidates) const;

        void PerformCollisionSweep(Ecs::EntityId target_entity, glm::vec3 position,
                                   glm::vec3 move_delta, float radius,
                                   const std::vector<Ecs::EntityId>& blocking_candidates,
                                   glm::vec3* final_position);

        void DetectTriggerInteractions(glm::vec3 final_position, float radius,
                                       Ecs::EntityId target_entity,
                                       const std::vector<Ecs::EntityId>& trigger_candidates);

        void RaiseCollisionEvents(Ecs::EntityId target_entity,
                                  const Engine::Physics::Collision::MoverResult& mover_result);

        void RaiseTriggerEvents(Ecs::EntityId target_entity,
                                std::unordered_set<Ecs::EntityId>& trigger_entities);

        glm::vec3 IntentToDelta(const Components::MotionIntent* intent, const glm::vec3& world_pos,
                                const glm::vec3& world_rot, float delta_time) const;

        static Engine::Physics::Math::AABB BuildSweptAabb(const glm::vec3& pos, const glm::vec3& rest,
                                                          float radius) noexcept;
    };
} // namespace
