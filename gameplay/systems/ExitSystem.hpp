#pragma once
#include "IEngineSystem.hpp"
#include "Ecs/Types.hpp"

namespace gameplay::systems {
    ECS_SYSTEM(ExitSystem, Update, TAGS(), DEPENDENCIES())

    class ExitSystem final : public yarep::Ecs::ISystem {
    public:
        ExitSystem() = default;

        ~ExitSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

        void OnTriggerEnter(const yarep::Ecs::EntityId& target, const yarep::Ecs::EntityId& other) override;

        void OnTriggerExit(const yarep::Ecs::EntityId& target, const yarep::Ecs::EntityId& other) override;

    private:
        void CheckIfPlayerHasKeyToExit(yarep::Ecs::EntityId target_entity,
                                       yarep::Ecs::EntityId potential_exit_entity) const;
    };
} // namespace
