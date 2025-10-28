//
// Created by Sebastian Borsch on 28.10.25.
//

#pragma once
#include <glm/vec2.hpp>

namespace Engine::Core::Components::UI {
    struct RectTransform {
        glm::vec2 position;
        glm::vec2 size;
    };
}
