#pragma once
#include <unordered_map>
#include <string>

#include "ISystem.hpp"

namespace Engine::Ecs {
    enum class Phase {
        Input,
        Physics,
        Update,
        Commands,
        LateUpdate,
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

        void RegisterSystems(const std::vector<SystemMeta>& system_metas, IServiceToEcsProvider *service_provider);

        void RunSystems(World& world, float delta_time);

    private:
        std::vector<Phase> m_phase_order;
        std::unordered_map<Phase, std::vector<std::unique_ptr<ISystem>>> m_phase_map;

        void RunPhase(Phase phase, World& world, float delta_time);
    };
} // namespace
