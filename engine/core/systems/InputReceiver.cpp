#include "InputReceiver.hpp"
#include "components/InputReceiver.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Core::Systems {
    InputReceiver::InputReceiver() = default;

    InputReceiver::~InputReceiver() = default;

    void InputReceiver::Initialize() {
        const auto i = ServiceLocator()->GetService<Environment::IInput>();
        m_input = i;
    }


    void InputReceiver::Run(float delta_time) {
        m_input->PrepareFrame();
        m_input->PumpInput();
        const auto input_snapshot = m_input->GetInputSnapshot();
        auto receivers = EcsWorld()->GetComponentsOfType<Components::InputReceiver>();
        for (const auto &input_receiver: receivers | std::views::keys) {
            input_receiver->Input = input_snapshot;
        }
    }
} // namespace
