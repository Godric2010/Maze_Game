#include "PhysicsSystem.hpp"

#include "Collider.hpp"
#include "MotionIntent.hpp"
#include "Transform.hpp"

namespace Engine::Core::Systems {
    PhysicsSystem::PhysicsSystem() {
        m_collider_cache = std::make_unique<ColliderCache>();
    }

    void PhysicsSystem::Initialize(Ecs::World *world,
                                   Ecs::IServiceToEcsProvider *service_locator) {
        world->GetEventBus()->Subscribe<Components::BoxCollider>(
            [this, world](const Ecs::EntityId entity, const Components::BoxCollider &box_collider) {
                const Components::Transform *transform = world->GetComponent<Components::Transform>(entity);
                this->BuildBoxCollider(entity, box_collider, transform->GetPosition());
            });

        world->GetEventBus()->Subscribe<Components::SphereCollider>(
            [this, world](const Ecs::EntityId entity, const Components::SphereCollider &sphere_collider) {
                Components::Transform *transform = world->GetComponent<Components::Transform>(entity);
                this->BuildSphereCollider(entity, sphere_collider, transform->GetPosition());
            });
    }

    void PhysicsSystem::Run(Ecs::World &world, float delta_time) {
        const auto movable_objects = world.GetComponentsOfType<Components::MotionIntent>();
        for (const auto [motion_intent, entity]: movable_objects) {
            const auto transform = world.GetComponent<Components::Transform>(entity);

            transform->m_position = motion_intent->translation;
            transform->m_rotation = motion_intent->rotation;
            transform->m_scale = motion_intent->scale;
            transform->m_dirty = true;
        }
    }

    void PhysicsSystem::BuildBoxCollider(Ecs::EntityId entity, const Components::BoxCollider box_collider,
                                         const glm::vec3 position) const {
        Physics::AABB aabb;
        aabb.min = glm::vec3(position.x - box_collider.width / 2, position.y - box_collider.height / 2,
                             position.z - box_collider.depth / 2);
        aabb.max = glm::vec3(position.x + box_collider.width / 2, position.y + box_collider.height / 2,
                             position.z + box_collider.depth / 2);

        m_collider_cache->box_colliders.emplace(entity, aabb);
        if (box_collider.is_static) {
            m_collider_cache->static_colliders.emplace(entity);
        }
    }

    void PhysicsSystem::BuildSphereCollider(Ecs::EntityId entity, const Components::SphereCollider sphere_collider,
        const glm::vec3 position) const {

        Physics::Sphere sphere;
        sphere.radius = sphere_collider.radius;
        sphere.center = position;

        m_collider_cache->sphere_colliders.emplace(entity, sphere);
        if (sphere_collider.is_static) {
            m_collider_cache->static_colliders.emplace(entity);
        }
    }


} // namespace
