#pragma once
#include "IEngineSystem.hpp"
#include "Ecs/Types.hpp"

namespace gameplay::systems {
    ECS_SYSTEM(ExitSystem, Update, TAGS(), DEPENDENCIES())

    class ExitSystem final : public yarep::ecs::ISystem {
    public:
        ExitSystem() = default;

        ~ExitSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

        void OnTriggerEnter(const yarep::ecs::EntityId& target, const yarep::ecs::EntityId& other) override;

        void OnTriggerExit(const yarep::ecs::EntityId& target, const yarep::ecs::EntityId& other) override;

    private:
        void CheckIfPlayerHasKeyToExit(yarep::ecs::EntityId target_entity,
                                       yarep::ecs::EntityId potential_exit_entity) const;
    };
} // namespace
