#include "ExitSystem.hpp"

#include <iostream>

#include "../components/Exit.hpp"
#include "../components/Inventory.hpp"

namespace Gameplay::Systems {
    void ExitSystem::Initialize(Engine::Ecs::World *world, Engine::Ecs::IServiceToEcsProvider *service_locator) {
        world->GetPhysicsEventBus()->SubscribeToOnTriggerEnter(
            [world](const Engine::Ecs::EntityId target, const Engine::Ecs::EntityId other) {
                std::cout << "Entered trigger " << other << std::endl;
                CheckIfPlayerHasKeyToExit(world, target, other);
            });
        world->GetPhysicsEventBus()->SubscribeToOnTriggerExit(
            [](Engine::Ecs::EntityId target, const Engine::Ecs::EntityId other) {
                std::cout << "Exited trigger " << other << std::endl;
            }
        );
    }

    void ExitSystem::Run(Engine::Ecs::World &world, float delta_time) {
    }

    void ExitSystem::CheckIfPlayerHasKeyToExit(Engine::Ecs::World *world, const Engine::Ecs::EntityId target_entity,
                                               const Engine::Ecs::EntityId potential_exit_entity) {
        const auto inventory = world->GetComponent<Components::Inventory>(target_entity);
        if (!inventory || !world->GetComponent<Components::Exit>(potential_exit_entity))
            return;

        if (inventory->key_collected) {
            std::cout << "Exit successful! You win!!!!" << std::endl;
        } else {
            std::cout << "Exit failed! You need the key to exit!" << std::endl;
        }
    }
} // namespace
