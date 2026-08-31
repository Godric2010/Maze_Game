#pragma once
#include "SystemManager.hpp"

namespace gameplay::systems {
    ECS_SYSTEM(ItemSystem, Update, TAGS(), DEPENDENCIES())

    class ItemSystem : public yarep::Ecs::ISystem {
    public:
        ItemSystem();

        ~ItemSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

        void OnCollisionEnter(const yarep::Ecs::EntityId& target, const yarep::Ecs::EntityId& other) override;

    private:
        void CheckIfItemGotPickedUp(yarep::Ecs::EntityId target_entity,
                                    yarep::Ecs::EntityId potential_item_entity) const;
    };
} // namespace
