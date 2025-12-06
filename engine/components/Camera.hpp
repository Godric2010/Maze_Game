//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once
#include <glm/glm.hpp>

namespace Engine::Components {
    struct Camera {
        float Width;
        float Height;
        float FieldOfView;
        float AspectRatio;
        float NearClip;
        float FarClip;
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
    };
}
