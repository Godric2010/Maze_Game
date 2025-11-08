#pragma once
#include <vector>
#include <memory>
#include <set>
#include <stdexcept>

#include "IInput.hpp"
#include "EnvInput.hpp"

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
        std::set<std::string> active_map_names;
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

        [[nodiscard]] bool IsMapActive(const std::string &map_name) const {
            return active_map_names.contains(map_name);
        }
    };


    class InputManager : public IInput {
    public:
        explicit InputManager(std::unique_ptr<Environment::IEnvInput> input_env);

        ~InputManager() override;

        void UpdateInput();

        [[nodiscard]] Environment::InputSnapshot *GetInputSnapshot() const;

        [[nodiscard]] Environment::AppEventsSnapshot *GetAppEventSnapshot() const;

        void AddInputMapping(const InputMap &input_map);

        void EnableInputMap(const std::string &input_map_name) override;

        void DisableInputMap(const std::string &input_map_name) override;

        void SetMouseVisibility(bool visible) override;

        [[nodiscard]] InputBuffer GetInput() const;

    private:
        void PopulateInputActions();

        std::unique_ptr<Environment::IEnvInput> m_input_env;
        std::vector<InputMap> m_input_maps;
        std::set<int> m_active_map_indices;

        InputBuffer m_input_buffer;
    };
} // namespace
