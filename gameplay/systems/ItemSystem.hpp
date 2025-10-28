#pragma once
#include "SystemManager.hpp"

namespace Gameplay::Systems {
    ECS_SYSTEM(ItemSystem, Update, [])

    class ItemSystem : public Engine::Ecs::ISystem {
    public:
        ItemSystem();

        ~ItemSystem() override = default;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        void CheckIfItemGotPickedUp(Engine::Ecs::EntityId target_entity,
                                    Engine::Ecs::EntityId potential_item_entity) const;
    };
} // namespace
