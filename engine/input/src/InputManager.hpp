#pragma once
#include <set>

#include "InputMapping.hpp"
#include "../include/IInputManager.hpp"
#include "Input/InputBuffer.hpp"
#include "Input/InputTypes.hpp"

namespace Engine::Input {
    class InputManager : public IInputManager {
    public:
        explicit InputManager(std::unique_ptr<Environment::IEnvInput> env_input);

        ~InputManager() override;

        void UpdateInput() override;

        Environment::AppEventsSnapshot* GetAppEventSnapshot() override;

        void AddInputMapping(const InputMap& input_map) override;

        void EnableInputMap(const std::string& input_map_name) override;

        void DisableInputMap(const std::string& input_map_name) override;

        void SetMouseVisibility(bool visible) override;

        [[nodiscard]] InputBuffer GetInput() override;

    private:
        void PopulateInputActions();

        std::unique_ptr<Environment::IEnvInput> m_input_env;
        std::vector<InputMap> m_input_maps;
        std::set<int> m_active_map_indices;

        InputBuffer m_input_buffer;
        InputMapping m_mapping{};
    };
} // namespace
