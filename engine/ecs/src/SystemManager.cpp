#include "../include/SystemManager.hpp"

#include <ostream>
#include <utility>

#include "CommandSystem.hpp"

namespace Engine::Core {
    class GameWorld;
}

namespace Engine::Ecs {
    SystemManager::SystemManager() {
        m_phase_order = std::vector{
            Phase::Input,
            Phase::Physics,
            Phase::Update,
            Phase::EngineEvents,
            Phase::LateUpdate,
            Phase::Commands,
            Phase::Render
        };
    }

    SystemManager::~SystemManager() = default;

    void SystemManager::RegisterSystems(const std::vector<SystemMeta> &system_metas,
                                        World *world,
                                        IServiceToEcsProvider *service_provider, Core::GameWorld *game_world) {
        auto command_system = std::make_unique<CommandSystem>([this](const std::vector<std::any> &commands) {
            this->RaiseCommandsEvent(commands);
        });
        command_system->m_world = world;
        command_system->m_service_locator = service_provider;
        command_system->Initialize();
        m_phase_map[Phase::Commands].push_back(std::move(command_system));

        for (const auto &sys_meta: system_metas) {
            auto system = sys_meta.factory();
            system->m_game_world = game_world;

            if (std::ranges::find(sys_meta.tags, "ENGINE") != sys_meta.tags.end()) {
                if (const auto engine_sys = dynamic_cast<IEngineSystem *>(system.get())) {
                    engine_sys->m_service_locator = service_provider;
                    engine_sys->m_world = world;
                }
            }
            system->Initialize();
            m_phase_map[sys_meta.phase].push_back(std::move(system));
        }
    }

    void SystemManager::RunSystems(World &world, const float delta_time) {
        for (const auto phase: m_phase_order) {
            RunPhase(phase, world, delta_time);
        }
    }

    void SystemManager::RegisterForSystemCommands(std::function<void(std::vector<std::any>)> command_callback) {
        m_system_commands_subscriber.push_back(std::move(command_callback));
    }


    void SystemManager::RunPhase(const Phase phase, World &world, const float delta_time) {
        for (const auto &system: m_phase_map[phase]) {
            system->Run(delta_time);
        }
    }

    void SystemManager::RaiseCommandsEvent(const std::vector<std::any> &commands) const {
        for (const auto& subscriber: m_system_commands_subscriber) {
            subscriber(commands);
        }
    }
} // namespace
