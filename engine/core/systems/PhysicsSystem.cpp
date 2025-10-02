#include "PhysicsSystem.hpp"

#include "Collider.hpp"
#include "Transform.hpp"
#include "../../physics/src/collision/SpatialHashBroadphase.hpp"

namespace Engine::Core::Systems {
    PhysicsSystem::PhysicsSystem() {
        m_collider_cache = std::make_unique<ColliderCache>();
        m_broadphase = std::make_unique<Physics::Collision::SpatialHashBroadphase>(2);
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
                const Components::Transform *transform = world->GetComponent<Components::Transform>(entity);
                this->BuildSphereCollider(entity, sphere_collider, transform->GetPosition());
            });
    }

    void PhysicsSystem::Run(Ecs::World &world, float delta_time) {
        const auto movable_objects = world.GetComponentsOfType<Components::MotionIntent>();
        for (const auto [motion_intent, entity]: movable_objects) {
            auto swept_area = UpdateAndGetSweptArea(entity, motion_intent);
            std::vector<Ecs::EntityId> collision_candidates;
            m_broadphase->QueryAABB(swept_area, collision_candidates, nullptr);

            // Remove target entity from possible colliders
            // TODO: This is very ugly but works for now. In the future, this should be handled via the layering system
            auto it = std::ranges::find(collision_candidates, entity);
            if (it != collision_candidates.end()) {
                collision_candidates.erase(it);
            }



            const auto transform = world.GetComponent<Components::Transform>(entity);

            transform->m_position = motion_intent->translation;
            transform->m_rotation = motion_intent->rotation;
            transform->m_scale = motion_intent->scale;
            transform->m_dirty = true;
        }
    }

    Physics::Math::AABB PhysicsSystem::UpdateAndGetSweptArea(const Ecs::EntityId entity,
                                                             const Components::MotionIntent *motion_intent) const {
        if (m_collider_cache->static_colliders.contains(entity)) {
            throw std::runtime_error("PhysicsSystem: Static colliders are not allowed to move!");
        }

        if (m_collider_cache->sphere_colliders.contains(entity)) {
            auto &sphere = m_collider_cache->sphere_colliders.at(entity);
            sphere.center = motion_intent->translation;
            m_broadphase->Update(entity, Physics::Collision::IBroadphase::FromSphere(sphere));

            return {sphere.center - sphere.radius, sphere.center + sphere.radius};
        }

        throw std::runtime_error("PhysicsSystem: Tried to move collider that is not supported in broadphase!");
    }


    void PhysicsSystem::BuildBoxCollider(Ecs::EntityId entity, const Components::BoxCollider box_collider,
                                         const glm::vec3 position) const {
        Physics::Math::AABB aabb{};
        aabb.min = glm::vec3(position.x - box_collider.width / 2, position.y - box_collider.height / 2,
                             position.z - box_collider.depth / 2);
        aabb.max = glm::vec3(position.x + box_collider.width / 2, position.y + box_collider.height / 2,
                             position.z + box_collider.depth / 2);

        m_collider_cache->box_colliders.emplace(entity, aabb);
        if (box_collider.is_static) {
            m_collider_cache->static_colliders.emplace(entity);
        }
        m_broadphase->Insert({entity, aabb, box_collider.is_static});
    }

    void PhysicsSystem::BuildSphereCollider(Ecs::EntityId entity, const Components::SphereCollider sphere_collider,
                                            const glm::vec3 position) const {
        Physics::Math::Sphere sphere{};
        sphere.radius = sphere_collider.radius;
        sphere.center = position;

        m_collider_cache->sphere_colliders.emplace(entity, sphere);
        if (sphere_collider.is_static) {
            m_collider_cache->static_colliders.emplace(entity);
        }

        const auto proxy_sphere = Physics::Collision::IBroadphase::FromSphere(sphere);
        m_broadphase->Insert({entity, proxy_sphere, sphere_collider.is_static});
    }
} // namespace
