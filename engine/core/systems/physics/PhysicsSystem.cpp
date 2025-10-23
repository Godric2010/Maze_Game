#include "PhysicsSystem.hpp"

#include <ostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include "Collider.hpp"
#include "Transform.hpp"
#include "../../../physics/src/collision/SpatialHashBroadphase.hpp"
#include "collision/MoverSolver.hpp"
#include "math/TypeUtils.hpp"

namespace Engine::Core::Systems::Physics {
    using namespace Engine::Physics;

    PhysicsSystem::PhysicsSystem() {
        m_collider_cache = std::make_unique<Collision::ColliderCache>();
        m_broadphase = std::make_unique<Collision::SpatialHashBroadphase>(2);
        m_collision_query_service = std::make_unique<Collision::CollisionQueryService>(
            *m_broadphase, *m_collider_cache);
    }

    void PhysicsSystem::Initialize(Ecs::World *world,
                                   Ecs::IServiceToEcsProvider *service_locator) {
        world->GetEventBus()->Subscribe<Components::BoxCollider>(
            [this, world](const Ecs::EntityId entity, const Components::BoxCollider &box_collider) {
                const Components::Transform *transform = world->GetComponent<Components::Transform>(entity);
                this->BuildBoxCollider(entity, box_collider, transform->GetPosition(), transform->GetRotation(),
                                       transform->GetScale());
            });

        world->GetEventBus()->Subscribe<Components::SphereCollider>(
            [this, world](const Ecs::EntityId entity, const Components::SphereCollider &sphere_collider) {
                const Components::Transform *transform = world->GetComponent<Components::Transform>(entity);
                this->BuildSphereCollider(entity, sphere_collider, transform->GetPosition());
            });
    }

    void PhysicsSystem::Run(Ecs::World &world, const float delta_time) {
        const auto movable_objects = world.GetComponentsOfType<Components::MotionIntent>();
       for (const auto [motion_intent, entity]: movable_objects) {
            const auto transform = world.GetComponent<Components::Transform>(entity);
            if (!transform) {
                continue;
            }

            glm::vec3 move_delta = IntentToDelta(motion_intent, transform->GetPosition(),
                                                 transform->GetRotation(), delta_time);
            const float length = length2(move_delta);

            if (length < m_epsilon) {
                transform->Set(transform->GetPosition(), motion_intent->rotation, motion_intent->scale);
                continue;
            }

            const auto it_sphere = m_collider_cache->sphere_colliders.find(entity);
            if (it_sphere == m_collider_cache->sphere_colliders.end()) {
                continue;
            }

            Collision::MoverInput input;
            input.position = transform->GetPosition();
            input.radius = it_sphere->second.radius;
            input.delta = move_delta;
            input.max_iterations = 3;

            const auto result = Collision::MoverSolver::Solve(input, *m_collision_query_service);
            transform->Set(result.new_position, motion_intent->rotation, transform->GetScale());
        }
    }

    void PhysicsSystem::BuildBoxCollider(Ecs::EntityId entity, const Components::BoxCollider box_collider,
                                         const glm::vec3 &position, const glm::vec3 &rotation,
                                         const glm::vec3 &scale) const {
        const auto obb = Math::Util::BuildWorldObb(position, rotation, box_collider.width, box_collider.height,
                                                   box_collider.depth);
        const auto aabb = Math::Util::ToTightAabb(obb);

        m_collider_cache->box_obbs.emplace(entity, obb);
        m_collider_cache->box_colliders.emplace(entity, aabb);
        if (box_collider.is_static) {
            m_collider_cache->static_colliders.emplace(entity);
            m_broadphase->Insert({entity, aabb, box_collider.is_static});
        }
    }

    void PhysicsSystem::BuildSphereCollider(Ecs::EntityId entity, const Components::SphereCollider sphere_collider,
                                            const glm::vec3 position) const {
        Math::Sphere sphere{};
        sphere.radius = sphere_collider.radius;
        sphere.center = position;

        m_collider_cache->sphere_colliders.emplace(entity, sphere);
        if (sphere_collider.is_static) {
            m_collider_cache->static_colliders.emplace(entity);
            const auto proxy_sphere = Math::Util::FromSphere(sphere);
            m_broadphase->Insert({entity, proxy_sphere, sphere_collider.is_static});
        }
    }

    glm::vec3 PhysicsSystem::IntentToDelta(const Components::MotionIntent *intent, const glm::vec3 &world_pos,
                                           const glm::vec3 &world_rot, const float delta_time) const {
        (void) world_pos;
        (void) world_rot;

        glm::vec3 local = intent->translation;
        local.y = 0.0f; // Vertical movement is not allowed

        const float local_length_sqr = length2(local);
        if (local_length_sqr < m_epsilon) {
            return glm::vec3(0.0f);
        }

        const glm::vec3 local_direction = (local_length_sqr > 1.0f) ? (local / std::sqrt(local_length_sqr)) : local;

        const float speed = 1.0f * (intent->speed_multiplier > 0.0f ? intent->speed_multiplier : 1.0f);
        return local_direction * speed * delta_time;
    }
} // namespace
