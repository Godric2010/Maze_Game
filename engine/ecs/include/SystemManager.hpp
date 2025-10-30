#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <any>

#include "ISystem.hpp"

namespace Engine::Core {
    class GameWorld;
}

namespace Engine::Ecs {
    enum class Phase {
        Input,
        Physics,
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

    class SystemManager {
    public:
        SystemManager();

        ~SystemManager();

        void RegisterSystems(const std::vector<SystemMeta> &system_metas, World *world,
                             IServiceToEcsProvider *service_provider, Core::GameWorld *game_world);

        void RunSystems(World &world, float delta_time);

        void RegisterForSystemCommands(std::function<void(std::vector<std::any>)> command_callback);

    private:
        std::vector<Phase> m_phase_order;
        std::unordered_map<Phase, std::vector<std::unique_ptr<ISystem> > > m_phase_map;
        std::vector<std::function<void(std::vector<std::any>)> > m_system_commands_subscriber;

        void RunPhase(Phase phase, World &world, float delta_time);

        void RaiseCommandsEvent(const std::vector<std::any> &commands) const;
    };
} // namespace
