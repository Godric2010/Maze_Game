#include "ItemSystem.hpp"

#include <iostream>

namespace Gameplay {
    ItemSystem::ItemSystem() = default;

    void ItemSystem::Initialize(Engine::Ecs::World *world, Engine::Ecs::IServiceToEcsProvider *service_locator) {
        world->GetPhysicsEventBus()->SubscribeToOnCollisionEnter(
            [this](const Engine::Ecs::EntityId target, const Engine::Ecs::EntityId other) {
                CheckIfItemGotPickedUp(target, other);
            });
    }

    void ItemSystem::Run(Engine::Ecs::World &world, float delta_time) {
    }

    void ItemSystem::CheckIfItemGotPickedUp(const Engine::Ecs::EntityId target_entity,
                                            const Engine::Ecs::EntityId potential_item_entity) {
        std::cout << "Collision Enter with target " << target_entity << " with collider " << potential_item_entity <<
                "." << std::endl;
    }
} // namespace
