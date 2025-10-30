#include "InputManager.hpp"

namespace Engine::Core {
    InputManager::InputManager(std::unique_ptr<Environment::IInput> input_env) : m_input_snapshot(nullptr) {
        m_input_env = std::move(input_env);
    }

    InputManager::~InputManager() = default;

    void InputManager::UpdateInput() {
        m_input_env->PrepareFrame();
        m_input_env->PumpInput();

        m_input_snapshot = m_input_env->GetInputSnapshot();
    }

    Environment::InputSnapshot *InputManager::GetInputSnapshot() const {
        return m_input_snapshot;
    }

    Environment::AppEventsSnapshot *InputManager::GetAppEventSnapshot() const {
        return m_input_env->GetAppEventSnapshot();
    }
} // namespace
