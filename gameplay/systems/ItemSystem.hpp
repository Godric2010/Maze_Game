#pragma once
#include "SystemManager.hpp"

namespace Gameplay::Systems {
    ECS_SYSTEM(ItemSystem, Update, [])

    class ItemSystem : public Engine::Ecs::ISystem {
    public:
        ItemSystem();

        ~ItemSystem() override = default;

        void Initialize(Engine::Ecs::World *world, Engine::Ecs::IServiceToEcsProvider *service_locator) override;

        void Run(Engine::Ecs::World &world, float delta_time) override;

    private:
        void CheckIfItemGotPickedUp(Engine::Ecs::World *world, Engine::Ecs::EntityId target_entity,
                                    Engine::Ecs::EntityId potential_item_entity);
    };
} // namespace
