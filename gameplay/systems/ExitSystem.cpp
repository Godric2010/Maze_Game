#include "ExitSystem.hpp"

#include <iostream>

#include "GameWorld.hpp"
#include "../components/Exit.hpp"
#include "../components/Inventory.hpp"

namespace Gameplay::Systems {
    void ExitSystem::Initialize() {
        GameWorld()->SubscribeToPhysicsEvent(Engine::Ecs::PhysicsEventType::OnTriggerEnter,
                                             [this](const Engine::Ecs::EntityId target,
                                                    const Engine::Ecs::EntityId other) {
                                                 std::cout << "Entered trigger " << other << std::endl;
                                                 CheckIfPlayerHasKeyToExit(target, other);
                                             });
        GameWorld()->SubscribeToPhysicsEvent(Engine::Ecs::PhysicsEventType::OnTriggerExit,
                                             [](Engine::Ecs::EntityId target, const Engine::Ecs::EntityId other) {
                                                 std::cout << "Exited trigger " << other << std::endl;
                                             }
        );
    }

    void ExitSystem::Run(float delta_time) {
    }

    void ExitSystem::CheckIfPlayerHasKeyToExit(const Engine::Ecs::EntityId target_entity,
                                               const Engine::Ecs::EntityId potential_exit_entity) const {
        const auto inventory = GameWorld()->GetComponent<Components::Inventory>(target_entity);
        if (!inventory || !GameWorld()->GetComponent<Components::Exit>(potential_exit_entity))
            return;

        if (inventory->key_collected) {
            std::cout << "Exit successful! You win!!!!" << std::endl;
            const auto quit_command = Commands::QuitCommand();
            GameWorld()->PushCommand(quit_command);
        } else {
            std::cout << "Exit failed! You need the key to exit!" << std::endl;
        }
    }
} // namespace
