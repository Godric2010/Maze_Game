#pragma once
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

    private:
    };
} // namespace
