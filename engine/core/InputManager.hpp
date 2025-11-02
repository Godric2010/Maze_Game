#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include "Input.hpp"

namespace Engine::Core {
    struct KeyBinding {
        std::string name;
        Environment::Key key;
        Environment::PressState press_state;
    };

    struct MouseKeyBinding {
        std::string name;
        Environment::MouseButton button;
        Environment::PressState press_state;
    };

    struct InputMap {
        std::string name;
        std::vector<KeyBinding> key_bindings;
        std::vector<MouseKeyBinding> mouse_bindings;
    };

    struct InputAction {
        std::string name;
    };

    struct InputBuffer {
        std::string active_map_name;
        std::vector<InputAction> actions;
        glm::vec2 mouse_position;
        glm::vec2 mouse_delta;

        [[nodiscard]] bool HasAction(const std::string &action_name) const {
            if (actions.empty())
                return false;
            for (const auto &[name]: actions) {
                if (name == action_name) {
                    return true;
                }
            }
            return false;
        }
    };


    class InputManager {
    public:
        explicit InputManager(std::unique_ptr<Environment::IInput> input_env);

        ~InputManager();

        void UpdateInput();

        [[nodiscard]] Environment::InputSnapshot *GetInputSnapshot() const;

        [[nodiscard]] Environment::AppEventsSnapshot *GetAppEventSnapshot() const;

        void AddInputMapping(const InputMap &input_map);

        void EnableInputMap(const std::string &input_map_name);

        [[nodiscard]] InputBuffer GetInput() const;

    private:
        void PopulateInputActions();

        std::unique_ptr<Environment::IInput> m_input_env;
        std::vector<InputMap> m_input_maps;
        int m_active_map_index = -1;

        InputBuffer m_input_buffer;
    };
} // namespace
