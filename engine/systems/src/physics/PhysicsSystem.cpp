#include "../../include/PhysicsSystem.hpp"

#include <ostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include "../../components/Collider.hpp"
#include "../../components/Transform.hpp"
#include "../../physics/src/collision/SpatialHashBroadphase.hpp"
#include "collision/CollisionUtils.hpp"
#include "collision/MoverSolver.hpp"
#include "math/TypeUtils.hpp"

namespace Engine::Systems::Physics {
    using namespace Engine::Physics;

    PhysicsSystem::PhysicsSystem() {
        m_collider_cache = std::make_unique<Collision::ColliderCache>();
        m_broadphase = std::make_unique<Collision::SpatialHashBroadphase>(2);
        m_collision_query_service = std::make_unique<Collision::CollisionQueryService>(
                *m_broadphase,
                *m_collider_cache
                );
    }

    void PhysicsSystem::Initialize() {
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::BoxCollider>(
                [this](const Ecs::EntityId entity, const Components::BoxCollider& box_collider) {
                    const Components::Transform* transform = EcsWorld()->GetComponent<Components::Transform>(entity);
                    this->BuildBoxCollider(entity,
                                           box_collider,
                                           transform->GetPosition(),
                                           transform->GetRotation(),
                                           transform->GetScale()
                            );
                }
                );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::BoxCollider>(
                [this](const Ecs::EntityId entity) {
                    this->m_collider_cache->box_colliders.erase(entity);
                }
                );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::SphereCollider>(
                [this](const Ecs::EntityId entity, const Components::SphereCollider& sphere_collider) {
                    const Components::Transform* transform = EcsWorld()->GetComponent<Components::Transform>(entity);
                    this->BuildSphereCollider(entity, sphere_collider, transform->GetPosition());
                }
                );
    }

    void PhysicsSystem::Run(const float delta_time) {
        const auto movable_objects = EcsWorld()->GetComponentsOfType<Components::MotionIntent>();
        for (const auto [motion_intent, entity]: movable_objects) {
            const auto transform = EcsWorld()->GetComponent<Components::Transform>(entity);
            if (!transform) {
                continue;
            }

            auto position = transform->GetPosition();
            glm::vec3 move_delta;
            if (!TryGetMoveDelta(position,
                                 transform->GetRotation(),
                                 motion_intent,
                                 delta_time,
                                 &move_delta
                    )) {
                transform->SetRotation(motion_intent->rotation).SetScale(motion_intent->scale);
                return;
            }
            std::vector<Ecs::EntityId> blocking_candidates;
            std::vector<Ecs::EntityId> trigger_candidates;
            const auto it_sphere = m_collider_cache->sphere_colliders.find(entity);
            if (it_sphere == m_collider_cache->sphere_colliders.end()) {
                continue;
            }
            const float radius = it_sphere->second.world_sphere.radius;

            RunBroadphase(entity, radius, position, move_delta, blocking_candidates, trigger_candidates);

            glm::vec3 final_position;
            PerformCollisionSweep(entity, position, move_delta, radius, blocking_candidates, &final_position);
            DetectTriggerInteractions(final_position, radius, entity, trigger_candidates);

            transform->SetPosition(final_position).SetRotation(motion_intent->rotation);
        }
    }

    void PhysicsSystem::BuildBoxCollider(Ecs::EntityId entity, const Components::BoxCollider box_collider,
                                         const glm::vec3& position, const glm::vec3& rotation,
                                         const glm::vec3& scale) const {
        const auto obb = Math::Util::BuildWorldObb(position,
                                                   rotation,
                                                   box_collider.width,
                                                   box_collider.height,
                                                   box_collider.depth
                );
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

    bool PhysicsSystem::TryGetMoveDelta(const glm::vec3& position, const glm::vec3& rotation,
                                        const Components::MotionIntent* intent, const float dt,
                                        glm::vec3* move_delta) const {
        const auto delta = IntentToDelta(intent, position, rotation, dt);
        const float length = length2(delta);

        if (length < m_epsilon) {
            return false;
        }

        *move_delta = delta;
        return true;
    }

    void PhysicsSystem::RunBroadphase(const Ecs::EntityId target_entity, const float radius, const glm::vec3& position,
                                      const glm::vec3 move_delta,
                                      std::vector<Ecs::EntityId>& blocking_candidates,
                                      std::vector<Ecs::EntityId>& trigger_candidates) const {
        std::vector<Ecs::EntityId> candidates;
        m_collision_query_service->QuerySphereSweep(position, move_delta, radius, candidates, nullptr);


        blocking_candidates.reserve(candidates.size());
        trigger_candidates.reserve(candidates.size());

        for (auto id: candidates) {
            if (id == target_entity)
                continue;

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
    }

    void PhysicsSystem::PerformCollisionSweep(const Ecs::EntityId target_entity,
                                              const glm::vec3 position, const glm::vec3 move_delta, const float radius,
                                              const std::vector<Ecs::EntityId>& blocking_candidates,
                                              glm::vec3* final_position) {
        Collision::MoverInput input;
        input.position = position;
        input.radius = radius;
        input.delta = move_delta;
        input.max_iterations = 3;

        const auto result = Collision::MoverSolver::Solve(input, *m_collision_query_service, blocking_candidates);
        *final_position = result.new_position;

        RaiseCollisionEvents(target_entity, result);
    }

    void PhysicsSystem::RaiseCollisionEvents(const Ecs::EntityId target_entity,
                                             const Collision::MoverResult& mover_result) {
        Ecs::PhysicsEvent event{};
        event.target_entity = target_entity;

        if (!mover_result.collided && m_collided_entities[target_entity] != Ecs::INVALID_ENTITY_ID) {
            event.other_collider_entity = m_collided_entities[target_entity];
            event.type = Ecs::PhysicsEventType::OnCollisionExit;
            EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(event);
            m_collided_entities[target_entity] = Ecs::INVALID_ENTITY_ID;
            return;
        }

        if (mover_result.collided) {
            const auto other_collider = mover_result.hit_entity.value();
            if (m_collided_entities[target_entity] == other_collider) {
                return;
            }
            if (m_collided_entities[target_entity] != Ecs::INVALID_ENTITY_ID) {
                event.other_collider_entity = m_collided_entities[target_entity];
                event.type = Ecs::PhysicsEventType::OnCollisionExit;
                EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(event);
            }

            event.other_collider_entity = other_collider;
            event.type = Ecs::PhysicsEventType::OnCollisionEnter;
            EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(event);
            m_collided_entities[target_entity] = event.other_collider_entity;
        }
    }

    void PhysicsSystem::DetectTriggerInteractions(const glm::vec3 final_position,
                                                  const float radius, const Ecs::EntityId target_entity,
                                                  const std::vector<Ecs::EntityId>& trigger_candidates) {
        std::unordered_set<Ecs::EntityId> current_inside;
        current_inside.reserve(trigger_candidates.size());

        for (auto id: trigger_candidates) {
            if (auto it_box = m_collider_cache->box_colliders.find(id);
                it_box != m_collider_cache->box_colliders.end()) {
                if (CheckOverlapSphereWithBox(it_box->second, final_position, radius)) {
                    current_inside.insert(id);
                }
                continue;
            }
            if (auto it_sphere = m_collider_cache->sphere_colliders.find(id);
                it_sphere != m_collider_cache->sphere_colliders.end()) {
                if (CheckOverlapSphereWithSphere(it_sphere->second, final_position, radius)) {
                    current_inside.insert(id);
                }
                continue;
            }
        }

        RaiseTriggerEvents(target_entity, current_inside);
    }

    void PhysicsSystem::RaiseTriggerEvents(Ecs::EntityId target_entity,
                                           std::unordered_set<Ecs::EntityId>& trigger_entities) {
        if (!m_triggered_entities.contains(target_entity)) {
            Ecs::PhysicsEvent event{};
            event.type = Ecs::PhysicsEventType::OnTriggerEnter;
            event.target_entity = target_entity;

            for (const auto id: trigger_entities) {
                event.other_collider_entity = id;
                EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(event);
            }
            m_triggered_entities.emplace(target_entity, trigger_entities);
            return;
        }
        std::unordered_set<Ecs::EntityId> exited_triggers;
        for (const auto id: m_triggered_entities[target_entity]) {
            if (!trigger_entities.contains(id)) {
                exited_triggers.insert(id);
                EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(Ecs::PhysicsEvent{
                            Ecs::PhysicsEventType::OnTriggerExit,
                            target_entity,
                            id
                        }
                        );
            }
        }

        std::unordered_set<Ecs::EntityId> entered_triggers;
        for (const auto id: trigger_entities) {
            if (!m_triggered_entities[target_entity].contains(id)) {
                entered_triggers.insert(id);
                EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(Ecs::PhysicsEvent{
                            Ecs::PhysicsEventType::OnTriggerEnter,
                            target_entity,
                            id
                        }
                        );
            }
        }

        for (const auto id: exited_triggers) {
            m_triggered_entities[target_entity].erase(id);
        }
        for (const auto id: entered_triggers) {
            m_triggered_entities[target_entity].emplace(id);
        }
    }


    glm::vec3 PhysicsSystem::IntentToDelta(const Components::MotionIntent* intent, const glm::vec3& world_pos,
                                           const glm::vec3& world_rot, const float delta_time) const {
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

    Math::AABB PhysicsSystem::BuildSweptAabb(const glm::vec3& pos, const glm::vec3& rest, const float radius) noexcept {
        const glm::vec3 p0 = pos;
        const glm::vec3 p1 = pos + rest;

        const glm::vec3 min = glm::min(p0, p1) - glm::vec3(radius);
        const glm::vec3 max = glm::max(p0, p1) + glm::vec3(radius);
        return {min, max};
    }
} // namespace
