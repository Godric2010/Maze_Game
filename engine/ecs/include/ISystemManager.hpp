#pragma once
#include <any>
#include <functional>
#include <string>
#include <vector>
#include "World.hpp"
#include "Ecs/ISystem.hpp"
#include "Input/IInput.hpp"

namespace Engine::Ecs
{
    enum class Phase
    {
        Input = 0,
        Physics = 1,
        Ui = 2,
        Update = 3,
        EngineEvents = 4,
        LateUpdate = 5,
        Commands = 6,
        Render = 7,
    };

    using SystemFactory = std::unique_ptr<ISystem>(*)();

    struct SystemMeta
    {
        std::string name;
        Phase phase;
        std::vector<std::string> tags;
        std::vector<std::string> dependencies;
        SystemFactory factory;
    };

    class ISystemManager
    {
        public:
            virtual ~ISystemManager() = default;

            virtual void RegisterSystems(World* world, Input::IInput* input) = 0;

            virtual void PreFixed(float delta_time) = 0;
            virtual void FixedUpdateSystems(float fixed_dt) = 0;

            virtual void UpdateSystems(float delta_time) = 0;

            virtual void RegisterForSystemCommands(std::string subscriber_name,
                                                   std::function<void(std::vector<std::any>)> command_callback) =0;

            virtual void DeregisterForSystemCommands(const std::string& subscriber_name) = 0;
    };
}
