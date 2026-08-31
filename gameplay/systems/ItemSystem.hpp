#pragma once
#include "SystemManager.hpp"

namespace gameplay::systems {
    ECS_SYSTEM(ItemSystem, Update, TAGS(), DEPENDENCIES())

    class ItemSystem : public yarep::ecs::ISystem {
    public:
        ItemSystem();

        ~ItemSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

        void OnCollisionEnter(const yarep::ecs::EntityId& target, const yarep::ecs::EntityId& other) override;

    private:
        void CheckIfItemGotPickedUp(yarep::ecs::EntityId target_entity,
                                    yarep::ecs::EntityId potential_item_entity) const;
    };
} // namespace
