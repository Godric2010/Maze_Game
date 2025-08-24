#include "../include/SystemManager.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Ecs {
    SystemManager::SystemManager() {
        std::cout << "SystemManager::SystemManager()" << std::endl;
    }

    SystemManager::~SystemManager() {
    }

    void SystemManager::RegisterSystem(const SystemMeta& meta) {
        const auto system = meta.factory();
        std::cout << "Registering system " << meta.name << " - " << system << std::endl;
    }

} // namespace