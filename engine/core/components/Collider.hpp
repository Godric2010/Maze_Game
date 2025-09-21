//
// Created by Sebastian Borsch on 20.09.25.
//

#pragma once

namespace Engine::Core::Components{

    struct SphereCollider {
        float radius;
    };

    struct BoxCollider {
        float width;
        float height;
        float depth;
    };
}