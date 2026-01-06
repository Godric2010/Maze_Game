#include "InputManager.hpp"

#include <stdexcept>

namespace Engine::Input {
    InputManager::InputManager(std::unique_ptr<Environment::IEnvInput> env_input) {
        m_input_env = std::move(env_input);

        const InputMap ui_input_map{
            .name = "UIInputMap",
            .mouse_bindings = {
                MouseKeyBinding{
                    .name = "UiButtonDown",
                    .button = MouseButton::Left,
                    .press_state = PressState::Down
                },
                MouseKeyBinding{
                    .name = "UiButtonUp",
                    .button = MouseButton::Left,
                    .press_state = PressState::Up
                },
            },
        };
        m_input_maps.push_back(ui_input_map);
    }

    InputManager::~InputManager() = default;

    void InputManager::UpdateInput() {
        m_input_env->PrepareFrame();
        m_input_env->PumpInput();
        PopulateInputActions();
    }


    Environment::AppEventsSnapshot* InputManager::GetAppEventSnapshot() {
        return m_input_env->GetAppEventSnapshot();
    }

    void InputManager::AddInputMapping(const InputMap& input_map) {
        for (const auto& input_map_name: m_input_maps) {
            if (input_map_name.name == input_map.name) {
                throw std::runtime_error("Same input map has been registered already!");
            }
        }
        m_input_maps.push_back(input_map);
    }

    void InputManager::EnableInputMap(const std::string& input_map_name) {
        for (int i = 0; i < m_input_maps.size(); ++i) {
            if (m_input_maps.at(i).name == input_map_name && !m_active_map_indices.contains(i)) {
                m_active_map_indices.insert(i);
                return;
            }
        }
    }

    void InputManager::DisableInputMap(const std::string& input_map_name) {
        for (int i = 0; i < m_input_maps.size(); ++i) {
            if (m_input_maps.at(i).name == input_map_name && m_active_map_indices.contains(i)) {
                m_active_map_indices.erase(i);
                return;
            }
        }
        throw std::runtime_error("Input map name '" + input_map_name + "' not found.");
    }

    void InputManager::SetMouseVisibility(const bool visible) {
        m_input_env->ShowMouseCursor(visible);
    }


    InputBuffer InputManager::GetInput() {
        return m_input_buffer;
    }

    void InputManager::PopulateInputActions() {
        m_input_buffer = InputBuffer();

        for (const auto map_index: m_active_map_indices) {
            const auto [map_name, key_bindings, mouse_bindings] = m_input_maps.at(map_index);
            m_input_buffer.active_map_names.emplace(map_name);
            const auto snapshot = m_input_env->GetInputSnapshot();
            if (snapshot == nullptr) {
                throw std::runtime_error("Input snapshot not found.");
            }

            m_input_buffer.mouse_delta = snapshot->GetMouseDelta();
            m_input_buffer.mouse_position = snapshot->GetMousePosition();

            for (const auto& [key_binding_name, key, press_state]: key_bindings) {
                if (snapshot->IsKeyInState(m_mapping.keys[key], m_mapping.press_state[press_state])) {
                    m_input_buffer.actions.push_back(InputAction{.name = key_binding_name});
                }
            }

            for (const auto& [mouse_binding_name, button, press_state]: mouse_bindings) {
                if (snapshot->IsMouseButtonInState(m_mapping.mouse[button], m_mapping.press_state[press_state])) {
                    m_input_buffer.actions.push_back(InputAction{.name = mouse_binding_name});
                }
            }
        }
    }
} // namespace
