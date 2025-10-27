#include "PhysicsSystem.hpp"

#include <ostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include "Collider.hpp"
#include "Transform.hpp"
#include "../../../physics/src/collision/SpatialHashBroadphase.hpp"
#include "collision/CollisionUtils.hpp"
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
        world->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::BoxCollider>(
            [this, world](const Ecs::EntityId entity, const Components::BoxCollider &box_collider) {
                const Components::Transform *transform = world->GetComponent<Components::Transform>(entity);
                this->BuildBoxCollider(entity, box_collider, transform->GetPosition(), transform->GetRotation(),
                                       transform->GetScale());
            });

        world->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::BoxCollider>(
            [this](const Ecs::EntityId entity) {
                this->m_collider_cache->box_colliders.erase(entity);
            }
        );

        world->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::SphereCollider>(
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
            float radius = it_sphere->second.world_sphere.radius;

            glm::vec3 position = transform->GetPosition();
            Math::AABB swept_aabb = BuildSweptAabb(position, move_delta, radius);
            std::vector<Ecs::EntityId> candidates;
            m_broadphase->QueryAabb(swept_aabb, candidates, nullptr);

            std::vector<Ecs::EntityId> blocking_candidates;
            std::vector<Ecs::EntityId> trigger_candidates;
            blocking_candidates.reserve(candidates.size());
            trigger_candidates.reserve(candidates.size());

            for (auto id: candidates) {
                if (id == entity) continue;

                if (auto it_box = m_collider_cache->box_colliders.find(id);
                    it_box != m_collider_cache->box_colliders.end()) {
                    if (it_box->second.is_trigger) {
                        trigger_candidates.push_back(id);
                    } else {
                        blocking_candidates.push_back(id);
                    }
                    continue;
                }

                if (auto it_sphere = m_collider_cache->sphere_colliders.find(id);
                    it_sphere != m_collider_cache->sphere_colliders.end()) {
                    if (it_sphere->second.is_trigger) {
                        trigger_candidates.push_back(id);
                    } else {
                        blocking_candidates.push_back(id);
                    }
                    continue;
                }
            }

            Collision::MoverInput input;
            input.position = position;
            input.radius = radius;
            input.delta = move_delta;
            input.max_iterations = 3;

            const auto result = Collision::MoverSolver::Solve(input, *m_collision_query_service, blocking_candidates);
            glm::vec3 final_position = result.new_position;

            if (result.collided) {
                world.GetPhysicsEventBuffer()->EnqueueEvent(Ecs::PhysicsEvent{
                    .type = Ecs::PhysicsEventType::OnCollisionEnter,
                    .target_entity = entity,
                    .other_collider_entity = result.hit_entity.value()
                });
            }

            std::unordered_set<Ecs::EntityId> current_inside;
            current_inside.reserve(trigger_candidates.size());

            for (auto id: trigger_candidates) {
                if (auto it_box = m_collider_cache->box_colliders.find(id);
                    it_box != m_collider_cache->box_colliders.end()) {
                    if (CheckOverlapSphereWithBox(it_box->second, final_position, radius)) {
                        // current_inside.insert(id);
                        world.GetPhysicsEventBuffer()->EnqueueEvent(Ecs::PhysicsEvent{
                            .type = Ecs::PhysicsEventType::OnTriggerEnter, .target_entity = entity,
                            .other_collider_entity = id,
                        });
                    }
                    continue;
                }
                if (auto it_sphere = m_collider_cache->sphere_colliders.find(id);
                    it_sphere != m_collider_cache->sphere_colliders.end()) {
                    if (CheckOverlapSphereWithSphere(it_sphere->second, final_position, radius)) {
                        // current_inside.insert(id);
                        world.GetPhysicsEventBuffer()->EnqueueEvent(Ecs::PhysicsEvent{
                            .type = Ecs::PhysicsEventType::OnTriggerEnter, .target_entity = entity,
                            .other_collider_entity = id,
                        });
                    }
                    continue;
                }
            }

            transform->Set(result.new_position, motion_intent->rotation, transform->GetScale());
        }
    }

    void PhysicsSystem::BuildBoxCollider(Ecs::EntityId entity, const Components::BoxCollider box_collider,
                                         const glm::vec3 &position, const glm::vec3 &rotation,
                                         const glm::vec3 &scale) const {
        const auto obb = Math::Util::BuildWorldObb(position, rotation, box_collider.width, box_collider.height,
                                                   box_collider.depth);
        const auto aabb = Math::Util::ToTightAabb(obb);

        Collision::BoxColliderInfo info{};
        info.world_box = aabb;
        info.world_obb = obb;
        info.is_static = box_collider.is_static;
        info.is_trigger = box_collider.is_trigger;

        m_collider_cache->box_colliders.emplace(entity, info);
        if (box_collider.is_static) {
            m_broadphase->Insert({entity, aabb, box_collider.is_static});
        }
    }

    void PhysicsSystem::BuildSphereCollider(Ecs::EntityId entity, const Components::SphereCollider sphere_collider,
                                            const glm::vec3 position) const {
        Math::Sphere sphere{};
        sphere.radius = sphere_collider.radius;
        sphere.center = position;

        Collision::SphereColliderInfo info{};
        info.world_sphere = sphere;
        info.is_static = sphere_collider.is_static;
        info.is_trigger = sphere_collider.is_trigger;

        m_collider_cache->sphere_colliders.emplace(entity, info);
        if (sphere_collider.is_static) {
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

    Math::AABB PhysicsSystem::BuildSweptAabb(const glm::vec3 &pos, const glm::vec3 &rest, const float radius) noexcept {
        const glm::vec3 p0 = pos;
        const glm::vec3 p1 = pos + rest;

        const glm::vec3 min = glm::min(p0, p1) - glm::vec3(radius);
        const glm::vec3 max = glm::max(p0, p1) + glm::vec3(radius);
        return {min, max};
    }
} // namespace
