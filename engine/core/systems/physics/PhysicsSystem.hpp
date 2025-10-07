#pragma once
#include "Camera.hpp"
#include "Collider.hpp"
#include "ColliderCache.hpp"
#include "ISystem.hpp"
#include "MotionIntent.hpp"
#include "collision/CollisionQueryService.hpp"
#include "collision/IBroadphase.hpp"

namespace Engine::Core::Systems::Physics {
    ECS_SYSTEM(PhysicsSystem, Physics, [])

    class PhysicsSystem final : public Ecs::ISystem {
    public:
        PhysicsSystem();

        ~PhysicsSystem() override = default;

        void Initialize(Ecs::World *world, Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Ecs::World &world, float delta_time) override;

    private:
        const float m_epsilon = 1e-6f;

        std::unique_ptr<ColliderCache> m_collider_cache;
        std::unique_ptr<Engine::Physics::Collision::IBroadphase> m_broadphase;
        std::unique_ptr<Engine::Physics::Collision::ICollisionQueryService> m_collision_query_service;

        void BuildBoxCollider(Ecs::EntityId entity, Components::BoxCollider box_collider, glm::vec3 position) const;

        void BuildSphereCollider(Ecs::EntityId entity, Components::SphereCollider sphere_collider,
                                 glm::vec3 position) const;

        glm::vec3 IntentToDelta(const Components::MotionIntent *intent, const glm::vec3 &world_pos,
                                const glm::vec3 &world_rot, float delta_time) const;
    };
} // namespace
