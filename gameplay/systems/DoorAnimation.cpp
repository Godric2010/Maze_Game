#include "DoorAnimation.hpp"

#include "Collider.hpp"

namespace gameplay::systems {
    void DoorAnimation::Initialize() {
        ISystem::Initialize();
    }

    void DoorAnimation::OnTriggerEnter(const yarep::ecs::EntityId& target, const yarep::ecs::EntityId& other) {
        CheckIfPlayerHasKey(target, other);
    }

    void DoorAnimation::OnTriggerExit(const yarep::ecs::EntityId& target, const yarep::ecs::EntityId& other) {
        const auto is_player = GameWorld()->GetComponent<components::Inventory>(target) != nullptr;
        const auto door_trigger = GameWorld()->GetComponent<components::DoorTrigger>(other);
        if (!is_player || door_trigger == nullptr) {
            return;
        }

        auto door = GameWorld()->GetComponent<components::Door>(door_trigger->door);
        if (door->current_state != components::Door::State::Closed) {
            door->current_state = components::Door::State::Closing;
        }
    }

    void DoorAnimation::Run(float delta_time) {
        auto doors = GameWorld()->GetComponentsOfType<components::Door>();
        for (const auto [door, entity]: doors) {
            auto door_transform = GameWorld()->GetComponent<yarep::components::Transform>(entity);
            if (door_transform == nullptr) {
                throw std::runtime_error("Door does not have a door transform");
            }

            auto door_position = door_transform->GetPosition();
            switch (door->current_state) {
                case components::Door::State::Opening:
                    if (door_position.y > m_door_open_position) {
                        door->current_state = components::Door::State::Opened;
                    }
                    door_position += glm::vec3(0, 1, 0) * delta_time * m_door_open_speed;
                    door_transform->SetPosition(door_position);
                    break;
                case components::Door::State::Closing:
                    if (door_position.y < m_door_close_position) {
                        door->current_state = components::Door::State::Closed;
                    }

                    if (GameWorld()->GetComponent<yarep::components::BoxCollider>(entity) == nullptr) {
                        GameWorld()->AddComponent<yarep::components::BoxCollider>(
                                entity,
                                m_disabled_box_colliders[entity]
                                );
                        m_disabled_box_colliders.erase(entity);
                    }

                    door_position += glm::vec3(0, -1, 0) * delta_time * m_door_open_speed;
                    door_transform->SetPosition(door_position);
                    break;
                case components::Door::State::Opened: {
                    const auto box_collider = GameWorld()->GetComponent<yarep::components::BoxCollider>(entity);
                    if (box_collider != nullptr) {
                        m_disabled_box_colliders[entity] = *box_collider;
                        GameWorld()->RemoveComponent<yarep::components::BoxCollider>(entity);
                    }
                    break;
                }
                case components::Door::State::Closed:
                    break;
            }
        }
    }

    void DoorAnimation::CheckIfPlayerHasKey(const yarep::ecs::EntityId target,
                                            const yarep::ecs::EntityId door_trigger_entity) {
        const auto player_inventory = GameWorld()->GetComponent<components::Inventory>(target);
        const auto door_trigger = GameWorld()->GetComponent<components::DoorTrigger>(door_trigger_entity);

        if (player_inventory == nullptr || door_trigger == nullptr) {
            return;
        }

        const auto door = GameWorld()->GetComponent<components::Door>(door_trigger->door);
        const bool door_unlocked = player_inventory->key_collected;
        m_key_item_detected = door_unlocked;
        door->current_state = door_unlocked ? components::Door::State::Opening : components::Door::State::Closed;
    }
} // namespace
