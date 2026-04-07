#pragma once
#include <vector>

#include "ISystemManager.hpp"

namespace Engine::Ecs {
    class SystemMetaSorter {
    public:
        static std::vector<SystemMeta> SortSystemMetasByPhaseAndDependencies(
                const std::vector<SystemMeta>& system_metas);

    private:
    };
} // namespace
