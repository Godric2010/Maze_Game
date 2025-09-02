#include "InputReceiver.hpp"
#include "components/InputReceiver.hpp"

namespace Engine::Core::Systems {
    InputReceiver::InputReceiver() = default;

    InputReceiver::~InputReceiver() = default;

    void InputReceiver::SetServices(Ecs::IServiceToEcsProvider *serviceLocator) {
        const auto i = serviceLocator->GetService<Environment::IInput>();
        m_input = i;
    }


    void InputReceiver::Run(Ecs::World &world, float deltaTime) {
        m_input->PrepareFrame();
        m_input->PumpInput();
        const auto inputSnapshot = m_input->GetInputSnapshot();
        auto receivers = world.GetComponentsOfType<Components::InputReceiver>();
        for (const auto &inputReceiver: receivers | std::views::keys) {
            inputReceiver->Input = inputSnapshot;
        }
    }
} // namespace
