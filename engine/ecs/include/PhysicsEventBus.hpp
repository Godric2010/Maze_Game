//
// Created by Sebastian Borsch on 24.10.25.
//

#pragma once
#include <functional>

#include "../src/Entity.hpp"

namespace Engine::Ecs {

    class PhysicsEventBus {
        using CollisionEventFunc = std::function<void(EntityId, EntityId)>;

    public:
        PhysicsEventBus() {
            m_on_collision_enter_funcs = std::vector<CollisionEventFunc>();
            m_on_collision_exit_funcs = std::vector<CollisionEventFunc>();
            m_on_trigger_enter_funcs = std::vector<CollisionEventFunc>();
            m_on_trigger_exit_funcs = std::vector<CollisionEventFunc>();
        }

        ~PhysicsEventBus() = default;

        void SubscribeToOnCollisionEnter(std::function<void(EntityId, EntityId)> collision_enter_func) {
            m_on_collision_enter_funcs.emplace_back(std::move(collision_enter_func));
        }

        void SubscribeToOnCollisionExit(std::function<void(EntityId, EntityId)> collision_exit_func) {
            m_on_collision_exit_funcs.emplace_back(std::move(collision_exit_func));
        }

        void SubscribeToOnTriggerEnter(std::function<void(EntityId, EntityId)> trigger_enter_func) {
            m_on_trigger_enter_funcs.emplace_back(std::move(trigger_enter_func));
        }

        void SubscribeToOnTriggerExit(std::function<void(EntityId, EntityId)> trigger_exit_func) {
            m_on_trigger_exit_funcs.emplace_back(std::move(trigger_exit_func));
        }

        void RaiseOnCollisionEnter(const EntityId target_entity, const EntityId collider_entity) const {
            for (const auto &on_collision_enter_func: m_on_collision_enter_funcs) {
                on_collision_enter_func(target_entity, collider_entity);
            }
        }

        void RaiseOnCollisionExit(const EntityId target_entity, const EntityId collider_entity) const {
            for (const auto &on_collision_exit_func: m_on_collision_exit_funcs) {
                on_collision_exit_func(target_entity, collider_entity);
            }
        };

        void RaiseOnTriggerEnter(const EntityId target_entity, const EntityId collider_entity) const {
            for (const auto &on_trigger_enter_func: m_on_trigger_enter_funcs) {
                on_trigger_enter_func(target_entity, collider_entity);
            }
        };

        void RaiseOnTriggerExit(const EntityId target_entity, const EntityId collider_entity) const {
            for (const auto &on_trigger_exit_func: m_on_trigger_exit_funcs) {
                on_trigger_exit_func(target_entity, collider_entity);
            }
        };

    private:
        std::vector<CollisionEventFunc> m_on_collision_enter_funcs;
        std::vector<CollisionEventFunc> m_on_collision_exit_funcs;
        std::vector<CollisionEventFunc> m_on_trigger_enter_funcs;
        std::vector<CollisionEventFunc> m_on_trigger_exit_funcs;
    };
}
