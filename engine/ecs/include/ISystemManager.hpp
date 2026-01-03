#pragma once
#include <any>
#include <functional>
#include <string>
#include <vector>
#include "World.hpp"
#include "Ecs/ISystem.hpp"
#include "Input/IInput.hpp"

namespace Engine::Ecs {
    enum class Phase {
        Input,
        Physics,
        Ui,
        Update,
        EngineEvents,
        LateUpdate,
        Commands,
        Render,
    };

    using SystemFactory = std::unique_ptr<ISystem>(*)();

    struct SystemMeta {
        std::string name;
        Phase phase;
        std::vector<std::string> tags;
        SystemFactory factory;
    };

    class ISystemManager {
    public:
        virtual ~ISystemManager() = default;

        virtual void RegisterSystems(World* world, Input::IInput* input) = 0;

        virtual void RunSystems(float delta_time) = 0;

        virtual void RegisterForSystemCommands(std::string subscriber_name,
                                               std::function<void(std::vector<std::any>)> command_callback) =0;

        virtual void DeregisterForSystemCommands(const std::string& subscriber_name) = 0;
    };
}
