#pragma once
#include <vector>

#include "../ComponentManager.hpp"
#include "../Entity.hpp"

namespace Engine::Ecs {

    enum class EcsEventType {
        CreateEntity,
        DestroyEntity,
        AddComponent,
        RemoveComponent,
        UpdateComponent,
    };

    struct EcsEvent {
        EcsEventType type;
        EntityId entity;

        ComponentTypeId component_type_id;
        std::vector<std::byte> payload;
    };

} // namespace