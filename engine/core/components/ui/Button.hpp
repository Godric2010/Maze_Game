//
// Created by Sebastian Borsch on 02.11.25.
//

#pragma once
#include <string>
#include <glm/vec4.hpp>

namespace Engine::Core::Components::UI {
    struct Button {
        std::string name;
        glm::vec4 default_color;
        glm::vec4 highlight_color;
        glm::vec4 click_color;
        glm::vec4 active_color;
    };
}
