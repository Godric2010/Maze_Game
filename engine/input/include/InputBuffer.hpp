#pragma once
#include <set>
#include <string>
#include <glm/vec2.hpp>

#include "Input/InputTypes.hpp"

namespace Engine::Input {
    struct InputBuffer {
        std::set<std::string> active_map_names;
        std::vector<InputAction> actions;
        glm::vec2 mouse_position;
        glm::vec2 mouse_delta;

        [[nodiscard]] bool HasAction(const std::string& action_name) const {
            if (actions.empty())
                return false;
            for (const auto& [name]: actions) {
                if (name == action_name) {
                    return true;
                }
            }
            return false;
        }

        [[nodiscard]] bool IsMapActive(const std::string& map_name) const {
            return active_map_names.contains(map_name);
        }
    };
}
