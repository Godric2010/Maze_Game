//
// Created by Sebastian Borsch on 20.09.25.
//

#pragma once

namespace Engine::Core::Components{

    struct SphereCollider {
        bool is_static;
        float radius;
    };

    struct BoxCollider {
        bool is_static;
        float width;
        float height;
        float depth;
    };
}