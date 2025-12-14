#pragma once
#include <any>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "ISystem.hpp"
#include "../../core/GameWorld.hpp"
#include "../../systems/src/CacheManager.hpp"

namespace Engine::Input {
    class IInput;
}


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

    class SystemManager {
    public:
        SystemManager(const std::vector<SystemMeta>& system_metas, IServiceToEcsProvider* service_provider,
                      Systems::ICacheManager* cache_manager);

        ~SystemManager();

        void RegisterSystems(World* world, Input::IInput* input);

        void RunSystems(float delta_time);

        void RegisterForSystemCommands(std::string subscriber_name,
                                       std::function<void(std::vector<std::any>)> command_callback);

        void DeregisterForSystemCommands(const std::string& subscriber_name);

    private:
        std::unique_ptr<Core::GameWorld> m_game_world;
        std::vector<SystemMeta> m_system_metas;
        IServiceToEcsProvider* m_service_provider;
        Systems::CacheManager* m_cache_manager;

        std::vector<Phase> m_phase_order;
        std::unordered_map<Phase, std::vector<std::unique_ptr<ISystem> > > m_phase_map;
        std::unordered_map<std::string, std::function<void(std::vector<std::any>)> > m_command_callback_subscriber;

        void RunPhase(Phase phase, float delta_time);

        void RaiseCommandsEvent(const std::vector<std::any>& commands) const;
    };
} // namespace
