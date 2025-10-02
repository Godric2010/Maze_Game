#pragma once
#include "Collider.hpp"
#include "ColliderCache.hpp"
#include "ISystem.hpp"
#include "MotionIntent.hpp"
#include "collision/IBroadphase.hpp"

namespace Engine::Core::Systems {
    ECS_SYSTEM(PhysicsSystem, Physics, [])

    class PhysicsSystem final : public Ecs::ISystem {
    public:
        PhysicsSystem();

        ~PhysicsSystem() override = default;

        void Initialize(Ecs::World *world, Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Ecs::World &world, float delta_time) override;

    private:
        std::unique_ptr<ColliderCache> m_collider_cache;
        std::unique_ptr<Physics::Collision::IBroadphase> m_broadphase;

        Physics::Math::AABB UpdateAndGetSweptArea(Ecs::EntityId entity,
                                               const Components::MotionIntent *motion_intent) const;

        void BuildBoxCollider(Ecs::EntityId entity, Components::BoxCollider box_collider, glm::vec3 position) const;

        void BuildSphereCollider(Ecs::EntityId entity, Components::SphereCollider sphere_collider, glm::vec3 position) const;
    };
} // namespace
