#include "InputReceiver.hpp"
#include "components/InputReceiver.hpp"

namespace Engine::Core::Systems {
    InputReceiver::InputReceiver() =default;

    InputReceiver::~InputReceiver() = default;

    void InputReceiver::Run(Ecs::World &world, float deltaTime) {
        auto receivers = world.GetComponentsOfType<Components::InputReceiver>();
        for (const auto &inputReceiver: receivers | std::views::keys) {
            inputReceiver->Input = nullptr;
        }
    }



} // namespace