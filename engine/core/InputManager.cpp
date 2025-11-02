#include "InputManager.hpp"

namespace Engine::Core {
    InputManager::InputManager(std::unique_ptr<Environment::IInput> input_env) {
        m_input_env = std::move(input_env);
    }

    InputManager::~InputManager() = default;

    void InputManager::UpdateInput() {
        m_input_env->PrepareFrame();
        m_input_env->PumpInput();
        PopulateInputActions();
    }

    Environment::InputSnapshot *InputManager::GetInputSnapshot() const {
        return m_input_env->GetInputSnapshot();
    }

    Environment::AppEventsSnapshot *InputManager::GetAppEventSnapshot() const {
        return m_input_env->GetAppEventSnapshot();
    }

    void InputManager::AddInputMapping(const InputMap &input_map) {
        m_input_maps.push_back(input_map);
    }

    void InputManager::EnableInputMap(const std::string &input_map_name) {
        for (int i = 0; i < m_input_maps.size(); ++i) {
            if (m_input_maps.at(i).name == input_map_name) {
                m_active_map_index = i;
                return;
            }
        }
        throw std::runtime_error("Input map name '" + input_map_name + "' not found.");
    }

    InputBuffer InputManager::GetInput() const {
        return m_input_buffer;
    }

    void InputManager::PopulateInputActions() {
        m_input_buffer = InputBuffer();
        const auto [map_name, key_bindings, mouse_bindings] = m_input_maps.at(m_active_map_index);
        m_input_buffer.active_map_name = map_name;
        const auto snapshot = m_input_env->GetInputSnapshot();
        if (snapshot == nullptr) {
            throw std::runtime_error("Input snapshot not found.");
        }

        m_input_buffer.mouse_delta = snapshot->GetMouseDelta();
        m_input_buffer.mouse_position = snapshot->GetMousePosition();

        for (const auto &[key_binding_name, key, press_state]: key_bindings) {
            if (snapshot->IsKeyInState(key, press_state)) {
                m_input_buffer.actions.push_back(InputAction{.name = key_binding_name});
            }
        }

        for (const auto &[mouse_binding_name, button, press_state]: mouse_bindings) {
            if (snapshot->IsMouseButtonInState(button, press_state)) {
                m_input_buffer.actions.push_back(InputAction{.name = mouse_binding_name});
            }
        }
    }
} // namespace
