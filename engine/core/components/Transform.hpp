//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once

#include <glm/glm.hpp>

namespace Engine::Core::Components {
    struct Transform {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 matrix;

        Transform() {
            position = glm::vec3(0.0f);
            rotation = glm::vec3(0.0f);
            scale = glm::vec3(1.0f);
            matrix = glm::mat4(1.0f);
        }

        Transform(const glm::vec3 position, const glm::vec3 rotation) {
            this->position = position;
            this->rotation = rotation;
            this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            matrix = glm::mat4(1.0f);
        }

        Transform(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale) {
            this->position = position;
            this->rotation = rotation;
            this->scale = scale;
            matrix = glm::mat4(1.0f);
        }
    };
}
