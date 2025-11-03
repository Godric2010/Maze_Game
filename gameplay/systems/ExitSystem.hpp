#pragma once
#include "ISystem.hpp"
#include "../commands/QuitCommand.hpp"

namespace Gameplay::Systems {
    ECS_SYSTEM(ExitSystem, Update, [])

    class ExitSystem final : public Engine::Ecs::ISystem {
    public:
        ExitSystem() = default;

        ~ExitSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        void CheckIfPlayerHasKeyToExit(Engine::Ecs::EntityId target_entity,
                                       Engine::Ecs::EntityId potential_exit_entity) const;
    };
} // namespace
