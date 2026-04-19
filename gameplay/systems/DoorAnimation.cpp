#include "DoorAnimation.hpp"

#include "Collider.hpp"

namespace Gameplay::Systems {
    void DoorAnimation::Initialize() {
        ISystem::Initialize();
    }

    void DoorAnimation::OnTriggerEnter(const Engine::Ecs::EntityId& target, const Engine::Ecs::EntityId& other) {
        CheckIfPlayerHasKey(target, other);
    }

    void DoorAnimation::OnTriggerExit(const Engine::Ecs::EntityId& target, const Engine::Ecs::EntityId& other) {
        const auto is_player = GameWorld()->GetComponent<Components::Inventory>(target) != nullptr;
        const auto door_trigger = GameWorld()->GetComponent<Components::DoorTrigger>(other);
        if (!is_player || door_trigger == nullptr) {
            return;
        }

        auto door = GameWorld()->GetComponent<Components::Door>(door_trigger->door);
        if (door->CurrentState != Components::Door::State::Closed) {
            door->CurrentState = Components::Door::State::Closing;
        }
    }

    void DoorAnimation::Run(float delta_time) {
        auto doors = GameWorld()->GetComponentsOfType<Components::Door>();
        for (const auto [door, entity]: doors) {
            auto door_transform = GameWorld()->GetComponent<Engine::Components::Transform>(entity);
            if (door_transform == nullptr) {
                throw std::runtime_error("Door does not have a door transform");
            }

            auto door_position = door_transform->GetPosition();
            switch (door->CurrentState) {
                case Components::Door::State::Opening:
                    if (door_position.y > m_door_open_position) {
                        door->CurrentState = Components::Door::State::Opened;
                    }
                    door_position += glm::vec3(0, 1, 0) * delta_time * m_door_open_speed;
                    door_transform->SetPosition(door_position);
                    break;
                case Components::Door::State::Closing:
                    if (door_position.y < m_door_close_position) {
                        door->CurrentState = Components::Door::State::Closed;
                    }

                    if (GameWorld()->GetComponent<Engine::Components::BoxCollider>(entity) == nullptr) {
                        GameWorld()->AddComponent<Engine::Components::BoxCollider>(
                                entity,
                                m_disabled_box_colliders[entity]
                                );
                        m_disabled_box_colliders.erase(entity);
                    }

                    door_position += glm::vec3(0, -1, 0) * delta_time * m_door_open_speed;
                    door_transform->SetPosition(door_position);
                    break;
                case Components::Door::State::Opened: {
                    const auto box_collider = GameWorld()->GetComponent<Engine::Components::BoxCollider>(entity);
                    if (box_collider != nullptr) {
                        m_disabled_box_colliders[entity] = *box_collider;
                        GameWorld()->RemoveComponent<Engine::Components::BoxCollider>(entity);
                    }
                    break;
                }
                case Components::Door::State::Closed:
                    break;
            }
        }
    }

    void DoorAnimation::CheckIfPlayerHasKey(const Engine::Ecs::EntityId target,
                                            const Engine::Ecs::EntityId door_trigger_entity) {
        const auto player_inventory = GameWorld()->GetComponent<Components::Inventory>(target);
        const auto door_trigger = GameWorld()->GetComponent<Components::DoorTrigger>(door_trigger_entity);

        if (player_inventory == nullptr || door_trigger == nullptr) {
            return;
        }

        const auto door = GameWorld()->GetComponent<Components::Door>(door_trigger->door);
        const bool door_unlocked = player_inventory->key_collected;
        m_key_item_detected = door_unlocked;
        door->CurrentState = door_unlocked ? Components::Door::State::Opening : Components::Door::State::Closed;
    }
} // namespace
