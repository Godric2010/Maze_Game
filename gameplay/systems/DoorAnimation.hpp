#pragma once
#include "Collider.hpp"
#include "SystemManager.hpp"
#include "../components/Inventory.hpp"
#include "../components/Door.hpp"
#include "../components/DoorTrigger.hpp"

namespace gameplay::systems {
    ECS_SYSTEM(DoorAnimation, Update, TAGS(), DEPENDENCIES())
    class DoorAnimation : public yarep::Ecs::ISystem {
    public:
        DoorAnimation() = default;

        ~DoorAnimation() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

        void OnTriggerEnter(const yarep::Ecs::EntityId& target, const yarep::Ecs::EntityId& other) override;

        void OnTriggerExit(const yarep::Ecs::EntityId& target, const yarep::Ecs::EntityId& other) override;

    private:
        bool m_key_item_detected = false;

        float m_door_open_speed = 0.5f;
        float m_door_open_position = 1.8f;
        float m_door_close_position = 0.0f;

        std::unordered_map<yarep::Ecs::EntityId, yarep::Components::BoxCollider> m_disabled_box_colliders;

        void CheckIfPlayerHasKey(const yarep::Ecs::EntityId target, const yarep::Ecs::EntityId door_trigger_entity);
    };
} // namespace
