#pragma once
#include <unordered_map>
#include <string>

#include "ISystem.hpp"

namespace Engine::Ecs {
    enum class Phase {
        Input,
        Physics,
        Update,
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

        void RegisterSystem(const SystemMeta& meta);

        void RunSystems(World& world, float deltaTime);

    private:
        std::vector<Phase> m_phaseOrder;
        std::unordered_map<Phase, std::vector<std::unique_ptr<ISystem>>> m_phaseMap;

        void RunPhase(Phase phase, World& world, float deltaTime);
    };
} // namespace
