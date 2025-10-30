#include "ItemSystem.hpp"

#include <iostream>

#include "GameWorld.hpp"
#include "../components/Inventory.hpp"
#include "../components/KeyItem.hpp"
#include "ui/Image.hpp"

namespace Gameplay::Systems {
    ItemSystem::ItemSystem() = default;

    void ItemSystem::Initialize() {
        GameWorld()->SubscribeToPhysicsEvent(Engine::Ecs::PhysicsEventType::OnCollisionEnter,
                                             [this](const Engine::Ecs::EntityId target,
                                                    const Engine::Ecs::EntityId other) {
                                                 CheckIfItemGotPickedUp(target, other);
                                             });
    }

    void ItemSystem::Run(float delta_time) {
    }

    void ItemSystem::CheckIfItemGotPickedUp(const Engine::Ecs::EntityId target_entity,
                                            const Engine::Ecs::EntityId potential_item_entity) const {
        const auto player_inventory = GameWorld()->GetComponent<Components::Inventory>(target_entity);
        const auto is_key_item = GameWorld()->GetComponent<Components::KeyItem>(potential_item_entity) != nullptr;

        if (player_inventory != nullptr && is_key_item) {
            player_inventory->key_collected = true;
            GameWorld()->DestroyEntity(potential_item_entity);
            const auto ui_entity = GameWorld()->GetEntityByName("KeyIndicator");
            if (ui_entity == Engine::Ecs::INVALID_ENTITY_ID) {
                return;
            }
            const auto image_ui = GameWorld()->GetComponent<Engine::Core::Components::UI::Image>(ui_entity);
            image_ui->color = {0, 255, 0, 255};
        }
    }
} // namespace
