#include "PhysicsSystem.hpp"

#include <iostream>
#include <ostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>

#include "Collider.hpp"
#include "Transform.hpp"
#include "../../physics/src/collision/SpatialHashBroadphase.hpp"
#include "math/Resolve.hpp"
#include "math/Sweep.hpp"

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
        std::vector<Ecs::EntityId> collision_candidates;
        const auto movable_objects = world.GetComponentsOfType<Components::MotionIntent>();
        for (const auto [motion_intent, entity]: movable_objects) {
            const auto transform = world.GetComponent<Components::Transform>(entity);

            glm::vec3 position = transform->GetPosition();
            glm::vec3 move_delta = IntentToDelta(motion_intent, position, transform->GetRotation(), delta_time);
            const float length = glm::length(move_delta);

            if (length < m_epsilon) {
                transform->Set(position, motion_intent->rotation, motion_intent->scale);
                continue;
            }

            const auto it_sphere = m_collider_cache->sphere_colliders.find(entity);
            if (it_sphere == m_collider_cache->sphere_colliders.end()) {
                continue;
            }
            const float radius = it_sphere->second.radius;
            auto sphere = Physics::Math::Sphere(position,radius);

            m_broadphase->Update(entity, Physics::Collision::IBroadphase::FromSphere(sphere));
            glm::vec3 rest = move_delta;
            for (int it = 0; it < 3 && glm::length(rest) > m_epsilon; ++it) {
                auto swept_area = UpdateAndGetSweptArea(entity, position, rest);

                collision_candidates.clear();
                m_broadphase->QueryAABB(swept_area, collision_candidates, nullptr);

                float best_toi = glm::length(rest) + 1.f;
                glm::vec3 best_n(0);

                for (Ecs::EntityId candidate_entity: collision_candidates) {
                    const auto it_wall = m_collider_cache->box_colliders.find(candidate_entity);
                    if (it_wall == m_collider_cache->box_colliders.end()) {
                        continue;
                    }

                    const auto moving_sphere = Physics::Math::Sphere(position,radius);
                    auto hit = Physics::Math::Sweep(moving_sphere, rest, it_wall->second);
                    if (hit.hit && hit.time_of_impact < best_toi) {
                        best_toi = hit.time_of_impact;
                        best_n = hit.normal;
                    }
                }
                if (best_toi <= glm::length(rest)) {
                    glm::vec3 dir = glm::normalize(rest);
                    position += dir * best_toi;
                    glm::vec3 remaining = rest - dir * best_toi;
                    rest = Physics::Math::Slide(remaining, best_n);
                } else {
                    position += rest;
                }
            }

            transform->Set(position, motion_intent->rotation, motion_intent->scale);
        }
    }

    Physics::Math::AABB PhysicsSystem::UpdateAndGetSweptArea(const Ecs::EntityId entity,
                                                             const glm::vec3 &position, const glm::vec3 &intent) const {
        if (m_collider_cache->static_colliders.contains(entity)) {
            throw std::runtime_error("PhysicsSystem: Static colliders are not allowed to move!");
        }

        const auto it_sphere = m_collider_cache->sphere_colliders.find(entity);
        if (it_sphere != m_collider_cache->sphere_colliders.end()) {
            const float radius = it_sphere->second.radius;
            const glm::vec3 pos0 = position;
            const glm::vec3 pos1 = position + intent;

            const glm::vec3 min = glm::min(pos0, pos1) - glm::vec3(radius);
            const glm::vec3 max = glm::max(pos0, pos1) + glm::vec3(radius);
            return Physics::Math::AABB(min, max);
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
            m_broadphase->Insert({entity, aabb, box_collider.is_static});
        }
    }

    void PhysicsSystem::BuildSphereCollider(Ecs::EntityId entity, const Components::SphereCollider sphere_collider,
                                            const glm::vec3 position) const {
        Physics::Math::Sphere sphere{};
        sphere.radius = sphere_collider.radius;
        sphere.center = position;

        m_collider_cache->sphere_colliders.emplace(entity, sphere);
        if (sphere_collider.is_static) {
            m_collider_cache->static_colliders.emplace(entity);
            const auto proxy_sphere = Physics::Collision::IBroadphase::FromSphere(sphere);
            m_broadphase->Insert({entity, proxy_sphere, sphere_collider.is_static});
        }
    }

    glm::vec3 PhysicsSystem::IntentToDelta(const Components::MotionIntent *intent, const glm::vec3 &world_pos,
                                           const glm::vec3 &world_rot, const float delta_time) const {
        (void) world_pos;
        (void) world_rot;

        glm::vec3 local = intent->translation;
        local.y = 0.0f; // Vertical movement is not allowed

        const float local_length_sqr = glm::length2(local);
        if (local_length_sqr < m_epsilon) {
            return glm::vec3(0.0f);
        }

        const glm::vec3 local_direction = (local_length_sqr > 1.0f) ? (local / std::sqrt(local_length_sqr)) : local;

        const float speed = 1.0f * (intent->speed_multiplier > 0.0f ? intent->speed_multiplier : 1.0f);
        return local_direction * speed * delta_time;
    }
} // namespace
