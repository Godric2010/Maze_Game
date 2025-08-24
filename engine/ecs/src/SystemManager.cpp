#include "../include/SystemManager.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Ecs {
    SystemManager::SystemManager() {

        m_phaseOrder = std::vector{
            Phase::Input,
            Phase::Physics,
            Phase::Update,
            Phase::LateUpdate,
            Phase::Render
        };
    }

    SystemManager::~SystemManager() {
    }

    void SystemManager::RegisterSystem(const SystemMeta &meta) {
        auto system = meta.factory();
        m_phaseMap[meta.phase].push_back(std::move(system));
    }

    void SystemManager::RunSystems(World &world, const float deltaTime) {
        for (const auto phase : m_phaseOrder) {
            RunPhase(phase, world, deltaTime);
        }
    }


    void SystemManager::RunPhase(const Phase phase, World &world, const float deltaTime) {
        for (const auto &system: m_phaseMap[phase]) {
            system->Run(world, deltaTime);
        }
    }
} // namespace
