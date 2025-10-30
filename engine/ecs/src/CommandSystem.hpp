//
// Created by Sebastian Borsch on 30.10.25.
//

#pragma once
#include <unordered_set>

#include "ISystem.hpp"

namespace Engine::Ecs {
    class CommandSystem : public IEngineSystem {
    public:
        explicit CommandSystem(const std::function<void(std::vector<std::any>)> &cb) {
            m_commands_callback = cb;
        }

        ~CommandSystem() override = default;

        void Initialize() override {
        };

        void Run(float delta_time) override {
            const auto commands = EcsWorld()->GetAllCommands();
            m_commands_callback(commands);
        }

    private:
        std::function<void(std::vector<std::any>)> m_commands_callback;
    };
}
