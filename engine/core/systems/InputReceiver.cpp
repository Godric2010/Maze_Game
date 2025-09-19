#include "InputReceiver.hpp"
#include "components/InputReceiver.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Core::Systems {
    InputReceiver::InputReceiver() = default;

    InputReceiver::~InputReceiver() = default;

    void InputReceiver::SetServices(Ecs::IServiceToEcsProvider *service_locator) {
        const auto i = service_locator->GetService<Environment::IInput>();
        m_input = i;
    }


    void InputReceiver::Run(Ecs::World &world, float delta_time) {
        std::cout<<"InputReceiver::Run"<<std::endl;
        m_input->PrepareFrame();
        m_input->PumpInput();
        const auto input_snapshot = m_input->GetInputSnapshot();
        auto receivers = world.GetComponentsOfType<Components::InputReceiver>();
        for (const auto &input_receiver: receivers | std::views::keys) {
            input_receiver->Input = input_snapshot;
        }
    }
} // namespace
