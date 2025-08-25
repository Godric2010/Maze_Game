//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once

namespace Engine::Core::Components {
    struct Transform {
        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;
        glm::mat4 Matrix;
    };
}
