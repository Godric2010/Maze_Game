//
// Created by Sebastian Borsch on 15.12.25.
//

#pragma once
#include <any>
#include <functional>
#include "Types.hpp"

namespace Engine::Input {
    class IInput;
}

namespace Engine::Ecs {
#define ECS_SYSTEM(name, phase, tags)

    class SystemWorld;
    struct EngineBindToken;

    class ISystem {
    public:
        using CommandEvent = std::function<void(const std::any& command)>;

        virtual ~ISystem() = default;

        void Bind(EngineBindToken, Input::IInput& input, SystemWorld& world, CommandEvent command_event);

        virtual void Initialize() {
        }

        virtual void Run(float delta_time) = 0;

        virtual void OnCollisionEnter(const EntityId& target, const EntityId& other) {
        }

        virtual void OnCollisionExit(const EntityId& target, const EntityId& other) {
        }

        virtual void OnTriggerEnter(const EntityId& target, const EntityId& other) {
        }

        virtual void OnTriggerExit(const EntityId& target, const EntityId& other) {
        }

    protected:
        [[nodiscard]] SystemWorld* GameWorld() const { return m_world; }
        [[nodiscard]] Input::IInput* Input() const { return m_input; }

        void SendCommand(const std::any& command) const {
            m_command_event(command);
        }

    private:
        Input::IInput* m_input = nullptr;
        SystemWorld* m_world = nullptr;
        CommandEvent m_command_event{};
    };
}
