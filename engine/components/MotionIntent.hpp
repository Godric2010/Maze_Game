//
// Created by Sebastian Borsch on 20.09.25.
//

#pragma once
#include <glm/vec3.hpp>

namespace Engine::Components {
    struct MotionIntent {
        glm::vec3 translation{};
        glm::vec3 rotation{};
        glm::vec3 scale{};
        float speed_multiplier{};

        MotionIntent() {
            this->translation = glm::vec3(0.0f);
            this->rotation = glm::vec3(0.0f);
            this->scale = glm::vec3(1.0f);
            this->speed_multiplier = 1.0f;
        }
    };
}
