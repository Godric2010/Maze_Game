#include "ItemSystem.hpp"

#include <iostream>

#include "Inventory.hpp"
#include "KeyItem.hpp"

namespace Gameplay {
    ItemSystem::ItemSystem() = default;

    void ItemSystem::Initialize(Engine::Ecs::World *world, Engine::Ecs::IServiceToEcsProvider *service_locator) {
        world->GetPhysicsEventBus()->SubscribeToOnCollisionEnter(
            [this, world](const Engine::Ecs::EntityId target, const Engine::Ecs::EntityId other) {
                CheckIfItemGotPickedUp(world, target, other);
            });
    }

    void ItemSystem::Run(Engine::Ecs::World &world, float delta_time) {
    }

    void ItemSystem::CheckIfItemGotPickedUp(Engine::Ecs::World *world, const Engine::Ecs::EntityId target_entity,
                                            const Engine::Ecs::EntityId potential_item_entity) {
        const auto player_inventory = world->GetComponent<Inventory>(target_entity);
        const auto is_key_item = world->GetComponent<KeyItem>(potential_item_entity) != nullptr;

        if (player_inventory != nullptr && is_key_item) {
            player_inventory->key_collected = true;
            world->DestroyEntity(potential_item_entity);
        }
    }
} // namespace
