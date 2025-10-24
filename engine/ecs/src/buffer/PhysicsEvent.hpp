//
// Created by Sebastian Borsch on 24.10.25.
//

#pragma once
#include "../Entity.hpp"

namespace Engine::Ecs {
    enum class PhysicsEventType {
        OnCollisionEnter,
        OnCollisionExit,
        OnTriggerEnter,
        OnTriggerExit,
    };

    struct PhysicsEvent {
        PhysicsEventType type;
        EntityId target_entity;
        EntityId other_collider_entity;
    };
}
