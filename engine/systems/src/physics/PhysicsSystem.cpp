#include "PhysicsSystem.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <memory>
// #include <spdlog/spdlog.h>

#include "Collider.hpp"
#include "Rigidbody.hpp"
#include "TransformComponent.hpp"
#include "collision/BroadphaseBuilder.hpp"
#include "collision/IBroadphase.hpp"
#include "collision/CollisionUtils.hpp"
#include "collision/MoverSolver.hpp"
#include "math/TypeUtils.hpp"

namespace yarep::systems::physics
{
    using namespace yarep::physics;

    PhysicsSystem::PhysicsSystem()
    {
        m_collider_cache = std::make_unique<collision::ColliderCache>();
        m_broadphase = collision::BroadphaseBuilder::BuildBroadphase(2);
        m_collision_query_service = std::make_unique<collision::CollisionQueryService>(
            *m_broadphase,
            *m_collider_cache
        );
    }

    void PhysicsSystem::Initialize()
    {
        m_transform_cache = Cache()->GetTransformCache();

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::BoxCollider>(
            [this](const ecs::EntityId entity, const components::BoxCollider& box_collider)
            {
                const components::TransformComponent* transform = EcsWorld()->GetComponent<components::TransformComponent>(entity);
                this->BuildBoxCollider(entity,
                                       box_collider,
                                       transform->GetPosition(),
                                       transform->GetRotation(),
                                       transform->GetScale()
                );
            }
        );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<components::BoxCollider>(
            [this](const ecs::EntityId entity)
            {
                this->m_collider_cache->box_colliders.erase(entity);
            }
        );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::SphereCollider>(
            [this](const ecs::EntityId entity, const components::SphereCollider& sphere_collider)
            {
                const components::TransformComponent* transform = EcsWorld()->GetComponent<components::TransformComponent>(entity);
                this->BuildSphereCollider(entity, sphere_collider, transform->GetPosition());
            }
        );
    }

    void PhysicsSystem::Run(const float fixed_delta_time)
    {
        const auto movable_objects = EcsWorld()->GetComponentsOfType<components::Rigidbody>();
        for (const auto [rigidbody, entity] : movable_objects)
        {
            auto transform = EcsWorld()->GetComponent<components::TransformComponent>(entity);
            if (transform == nullptr)
            {
                throw std::runtime_error("A moveable object without a transform component is impossible to handle!");
            }
            auto velocity = rigidbody->GetVelocity();
            if (glm::length2(velocity) < m_epsilon)
            {
                continue;
            }

            const glm::vec3 old_position = transform->GetPosition();
            glm::vec3 move_delta = velocity * fixed_delta_time;

            if (length2(move_delta) < m_epsilon)
            {
                continue;
            }
            const auto it_sphere = m_collider_cache->sphere_colliders.find(entity);
            if (it_sphere == m_collider_cache->sphere_colliders.end())
            {
                continue;
            }
            const float radius = it_sphere->second.world_sphere.radius;
            it_sphere->second.world_sphere.center = old_position;

            std::vector<ecs::EntityId> blocking_candidates;
            std::vector<ecs::EntityId> trigger_candidates;
            RunBroadphase(entity, radius, old_position, move_delta, blocking_candidates, trigger_candidates);

            glm::vec3 final_position;
            PerformCollisionSweep(entity, old_position, move_delta, radius, blocking_candidates, &final_position);
            DetectTriggerInteractions(final_position, radius, entity, trigger_candidates);

            if (rigidbody->IsVelocityFixed())
            {
                constexpr auto zero_velocity = glm::vec3(0);
                rigidbody->SetVelocity(zero_velocity);
            }
            transform->SetPosition(final_position);
        }
    }

    void PhysicsSystem::BuildBoxCollider(ecs::EntityId entity, const components::BoxCollider box_collider,
                                         const glm::vec3& position, const glm::vec3& rotation,
                                         const glm::vec3& scale) const
    {
        const auto obb = math::util::BuildWorldObb(position,
                                                   rotation,
                                                   box_collider.width,
                                                   box_collider.height,
                                                   box_collider.depth
        );
        const auto aabb = math::util::ToTightAabb(obb);

        collision::BoxColliderInfo info{};
        info.world_box = aabb;
        info.world_obb = obb;
        info.is_static = box_collider.is_static;
        info.is_trigger = box_collider.is_trigger;

        m_collider_cache->box_colliders.emplace(entity, info);
        if (box_collider.is_static)
        {
            m_broadphase->Insert({entity, aabb, box_collider.is_static});
        }
    }

    void PhysicsSystem::BuildSphereCollider(ecs::EntityId entity, const components::SphereCollider sphere_collider,
                                            const glm::vec3 position) const
    {
        math::Sphere sphere{};
        sphere.radius = sphere_collider.radius;
        sphere.center = position;

        collision::SphereColliderInfo info{};
        info.world_sphere = sphere;
        info.is_static = sphere_collider.is_static;
        info.is_trigger = sphere_collider.is_trigger;

        m_collider_cache->sphere_colliders.emplace(entity, info);
        if (sphere_collider.is_static)
        {
            const auto proxy_sphere = math::util::FromSphere(sphere);
            m_broadphase->Insert({entity, proxy_sphere, sphere_collider.is_static});
        }
    }

    void PhysicsSystem::RunBroadphase(const ecs::EntityId target_entity, const float radius, const glm::vec3& position,
                                      const glm::vec3 move_delta,
                                      std::vector<ecs::EntityId>& blocking_candidates,
                                      std::vector<ecs::EntityId>& trigger_candidates) const
    {
        std::vector<ecs::EntityId> candidates;
        m_collision_query_service->QuerySphereSweep(position, move_delta, radius, candidates, nullptr);


        blocking_candidates.reserve(candidates.size());
        trigger_candidates.reserve(candidates.size());

        for (auto id : candidates)
        {
            if (id == target_entity)
                continue;

            if (auto it_box = m_collider_cache->box_colliders.find(id);
                it_box != m_collider_cache->box_colliders.end())
            {
                if (it_box->second.is_trigger)
                {
                    trigger_candidates.push_back(id);
                }
                else
                {
                    blocking_candidates.push_back(id);
                }
                continue;
            }

            if (auto it_sphere = m_collider_cache->sphere_colliders.find(id);
                it_sphere != m_collider_cache->sphere_colliders.end())
            {
                if (it_sphere->second.is_trigger)
                {
                    trigger_candidates.push_back(id);
                }
                else
                {
                    blocking_candidates.push_back(id);
                }
                continue;
            }
        }
    }

    void PhysicsSystem::PerformCollisionSweep(const ecs::EntityId target_entity,
                                              const glm::vec3 position, const glm::vec3 move_delta, const float radius,
                                              const std::vector<ecs::EntityId>& blocking_candidates,
                                              glm::vec3* final_position)
    {
        spdlog::debug("Performing collision sweep");
        collision::MoverInput input;
        input.position = position;
        input.radius = radius;
        input.delta = move_delta;
        input.max_iterations = 3;

        const auto result = collision::MoverSolver::Solve(input, *m_collision_query_service, blocking_candidates);
        *final_position = result.new_position;

        RaiseCollisionEvents(target_entity, result);
    }

    void PhysicsSystem::RaiseCollisionEvents(const ecs::EntityId target_entity,
                                             const collision::MoverResult& mover_result)
    {
        ecs::PhysicsEvent event{};
        event.target_entity = target_entity;

        if (!mover_result.collided && m_collided_entities[target_entity] != ecs::invalid_entity_id)
        {
            event.other_collider_entity = m_collided_entities[target_entity];
            event.type = ecs::PhysicsEventType::OnCollisionExit;
            EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(event);
            m_collided_entities[target_entity] = ecs::invalid_entity_id;
            return;
        }

        if (mover_result.collided)
        {
            const auto other_collider = mover_result.hit_entity.value();
            if (m_collided_entities[target_entity] == other_collider)
            {
                return;
            }
            if (m_collided_entities[target_entity] != ecs::invalid_entity_id)
            {
                event.other_collider_entity = m_collided_entities[target_entity];
                event.type = ecs::PhysicsEventType::OnCollisionExit;
                EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(event);
            }

            event.other_collider_entity = other_collider;
            event.type = ecs::PhysicsEventType::OnCollisionEnter;
            EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(event);
            m_collided_entities[target_entity] = event.other_collider_entity;
        }
    }

    void PhysicsSystem::DetectTriggerInteractions(const glm::vec3 final_position,
                                                  const float radius, const ecs::EntityId target_entity,
                                                  const std::vector<ecs::EntityId>& trigger_candidates)
    {
        std::unordered_set<ecs::EntityId> current_inside;
        current_inside.reserve(trigger_candidates.size());

        for (auto id : trigger_candidates)
        {
            if (auto it_box = m_collider_cache->box_colliders.find(id);
                it_box != m_collider_cache->box_colliders.end())
            {
                if (CheckOverlapSphereWithBox(it_box->second, final_position, radius))
                {
                    current_inside.insert(id);
                }
                continue;
            }
            if (auto it_sphere = m_collider_cache->sphere_colliders.find(id);
                it_sphere != m_collider_cache->sphere_colliders.end())
            {
                if (CheckOverlapSphereWithSphere(it_sphere->second, final_position, radius))
                {
                    current_inside.insert(id);
                }
                continue;
            }
        }

        RaiseTriggerEvents(target_entity, current_inside);
    }

    void PhysicsSystem::RaiseTriggerEvents(ecs::EntityId target_entity,
                                           std::unordered_set<ecs::EntityId>& trigger_entities)
    {
        if (!m_triggered_entities.contains(target_entity))
        {
            ecs::PhysicsEvent event{};
            event.type = ecs::PhysicsEventType::OnTriggerEnter;
            event.target_entity = target_entity;

            for (const auto id : trigger_entities)
            {
                event.other_collider_entity = id;
                EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(event);
            }
            m_triggered_entities.emplace(target_entity, trigger_entities);
            return;
        }
        std::unordered_set<ecs::EntityId> exited_triggers;
        for (const auto id : m_triggered_entities[target_entity])
        {
            if (!trigger_entities.contains(id))
            {
                exited_triggers.insert(id);
                EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(ecs::PhysicsEvent{
                        ecs::PhysicsEventType::OnTriggerExit,
                        target_entity,
                        id
                    }
                );
            }
        }

        std::unordered_set<ecs::EntityId> entered_triggers;
        for (const auto id : trigger_entities)
        {
            if (!m_triggered_entities[target_entity].contains(id))
            {
                entered_triggers.insert(id);
                EcsWorld()->GetPhysicsEventBuffer()->EnqueueEvent(ecs::PhysicsEvent{
                        ecs::PhysicsEventType::OnTriggerEnter,
                        target_entity,
                        id
                    }
                );
            }
        }

        for (const auto id : exited_triggers)
        {
            m_triggered_entities[target_entity].erase(id);
        }
        for (const auto id : entered_triggers)
        {
            m_triggered_entities[target_entity].emplace(id);
        }
    }


    math::AABB PhysicsSystem::BuildSweptAabb(const glm::vec3& pos, const glm::vec3& rest, const float radius) noexcept
    {
        const glm::vec3 p0 = pos;
        const glm::vec3 p1 = pos + rest;

        const glm::vec3 min = glm::min(p0, p1) - glm::vec3(radius);
        const glm::vec3 max = glm::max(p0, p1) + glm::vec3(radius);
        return {min, max};
    }
} // namespace
