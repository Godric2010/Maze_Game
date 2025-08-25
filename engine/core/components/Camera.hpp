//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once

namespace Engine::Core::Components {
    struct Camera {
        float Width;
        float Height;
        float FieldOfView;
        float AspectRatio;
        float NearClip;
        float FarClip;
    };
}
