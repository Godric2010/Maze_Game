#pragma once
#include "Input.hpp"

namespace Engine::Core {
    class InputManager {
    public:
        explicit InputManager(std::unique_ptr<Environment::IInput> input_env);

        ~InputManager();

        void UpdateInput();

        [[nodiscard]] Environment::InputSnapshot *GetInputSnapshot() const;

        [[nodiscard]] Environment::AppEventsSnapshot *GetAppEventSnapshot() const;

    private:
        std::unique_ptr<Environment::IInput> m_input_env;
        Environment::InputSnapshot *m_input_snapshot;
    };
} // namespace
