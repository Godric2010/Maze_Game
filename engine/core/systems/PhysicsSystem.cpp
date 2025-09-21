#include "PhysicsSystem.hpp"

#include "Collider.hpp"
#include "MotionIntent.hpp"
#include "Transform.hpp"

namespace Engine::Core::Systems {
    PhysicsSystem::PhysicsSystem() = default;

    void PhysicsSystem::SetServices(Ecs::IServiceToEcsProvider *service_locator) {
    }

    void PhysicsSystem::Run(Ecs::World &world, float delta_time) {
        const auto box_colliders = world.GetComponentsOfType<Components::BoxCollider>();
        const auto sphere_colliders = world.GetComponentsOfType<Components::SphereCollider>();

        const auto movable_objects = world.GetComponentsOfType<Components::MotionIntent>();
        for (const auto [motion_intent, entity]: movable_objects) {
            const auto transform = world.GetComponent<Components::Transform>(entity);

            transform->m_position = motion_intent->translation;
            transform->m_rotation = motion_intent->rotation;
            transform->m_scale = motion_intent->scale;
            transform->m_dirty = true;
        }
    }
} // namespace
