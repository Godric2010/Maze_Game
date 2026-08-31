#include "ItemSystem.hpp"
#include "../components/Inventory.hpp"
#include "../components/KeyItem.hpp"
#include "../components/ui/Image.hpp"

namespace gameplay::systems {
    ItemSystem::ItemSystem() = default;

    void ItemSystem::Initialize() {
    }

    void ItemSystem::Run(float delta_time) {
    }

    void ItemSystem::OnCollisionEnter(const yarep::Ecs::EntityId& target, const yarep::Ecs::EntityId& other) {
        CheckIfItemGotPickedUp(target, other);
    }

    void ItemSystem::CheckIfItemGotPickedUp(const yarep::Ecs::EntityId target_entity,
                                            const yarep::Ecs::EntityId potential_item_entity) const {
        const auto player_inventory = GameWorld()->GetComponent<components::Inventory>(target_entity);
        const auto is_key_item = GameWorld()->GetComponent<components::KeyItem>(potential_item_entity) != nullptr;

        if (player_inventory != nullptr && is_key_item) {
            player_inventory->key_collected = true;
            GameWorld()->DestroyEntity(potential_item_entity);
            const auto ui_entity = GameWorld()->GetEntityByName("KeyIndicator");
            if (ui_entity == yarep::Ecs::INVALID_ENTITY_ID) {
                return;
            }
            const auto image_ui = GameWorld()->GetComponent<yarep::Components::UI::Image>(ui_entity);
            image_ui->color = {0, 1, 0, 0.8f};
        }
    }
} // namespace
