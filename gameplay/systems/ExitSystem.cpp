#include "ExitSystem.hpp"

#include <iostream>

#include "SystemWorld.hpp"
#include "../commands/LevelFinished.hpp"
#include "../components/Exit.hpp"
#include "../components/Inventory.hpp"

namespace gameplay::systems {
    void ExitSystem::Initialize() {
    }

    void ExitSystem::Run(float delta_time) {
    }

    void ExitSystem::OnTriggerEnter(const yarep::ecs::EntityId& target, const yarep::ecs::EntityId& other) {
        CheckIfPlayerHasKeyToExit(target, other);
        std::cout << "Entered trigger " << other << std::endl;
    }

    void ExitSystem::OnTriggerExit(const yarep::ecs::EntityId& target, const yarep::ecs::EntityId& other) {
        std::cout << "Exited trigger " << other << std::endl;
    }


    void ExitSystem::CheckIfPlayerHasKeyToExit(const yarep::ecs::EntityId target_entity,
                                               const yarep::ecs::EntityId potential_exit_entity) const {
        const auto inventory = GameWorld()->GetComponent<components::Inventory>(target_entity);
        if (!inventory || !GameWorld()->GetComponent<components::Exit>(potential_exit_entity))
            return;

        if (inventory->key_collected) {
            std::cout << "Exit successful! You win!!!!" << std::endl;
            const auto level_finished = commands::LevelFinished();
            SendCommand(level_finished);
        } else {
            std::cout << "Exit failed! You need the key to exit!" << std::endl;
        }
    }
} // namespace
