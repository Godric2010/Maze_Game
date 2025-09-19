#include "../include/SystemManager.hpp"

#include <ostream>

namespace Engine::Ecs {
    SystemManager::SystemManager() {

        m_phase_order = std::vector{
            Phase::Input,
            Phase::Physics,
            Phase::Update,
            Phase::Commands,
            Phase::LateUpdate,
            Phase::Render
        };
    }

    SystemManager::~SystemManager() = default;

    void SystemManager::RegisterSystems(const std::vector<SystemMeta>& system_metas, IServiceToEcsProvider *service_provider) {
        for (const auto& sys_meta : system_metas) {
            auto system = sys_meta.factory();
            system->SetServices(service_provider);
            m_phase_map[sys_meta.phase].push_back(std::move(system));
        }
    }

    void SystemManager::RunSystems(World &world, const float delta_time) {
        for (const auto phase : m_phase_order) {
            RunPhase(phase, world, delta_time);
        }
    }


    void SystemManager::RunPhase(const Phase phase, World &world, const float delta_time) {
        for (const auto &system: m_phase_map[phase]) {
            system->Run(world, delta_time);
        }
    }
} // namespace
