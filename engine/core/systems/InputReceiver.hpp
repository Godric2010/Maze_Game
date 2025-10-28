#pragma once
#include "Input.hpp"
#include "ISystem.hpp"

ECS_SYSTEM(InputReceiver, Input, [ENGINE])
namespace Engine::Core::Systems {
    class InputReceiver : public Ecs::IEngineSystem {
    public:
        InputReceiver();

        ~InputReceiver() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
       Environment::IInput* m_input = nullptr;

    };
} // namespace