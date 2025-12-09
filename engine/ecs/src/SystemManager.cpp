#include "SystemManager.hpp"
#include <ostream>
#include <utility>
#include "CommandSystem.hpp"

namespace Engine::Ecs {
    SystemManager::SystemManager(const std::vector<SystemMeta>& system_metas,
                                 IServiceToEcsProvider* service_provider, Systems::CacheManager* cache_manager) {
        m_phase_order = std::vector{
            Phase::Input,
            Phase::Physics,
            Phase::Ui,
            Phase::Update,
            Phase::EngineEvents,
            Phase::LateUpdate,
            Phase::Commands,
            Phase::Render
        };

        m_system_metas = system_metas;
        m_service_provider = service_provider;
        m_cache_manager = cache_manager;
    }

    SystemManager::~SystemManager() {
        m_service_provider = nullptr;
        m_cache_manager = nullptr;
        m_phase_order.clear();
        m_system_metas.clear();
    }

    void SystemManager::RegisterSystems(World* world, Input::IInput* input) {
        m_game_world = std::make_unique<Core::GameWorld>(world);
        m_phase_map.clear();

        auto command_system = std::make_unique<CommandSystem>([this](const std::vector<std::any>& commands) {
                    this->RaiseCommandsEvent(commands);
                }
                );
        command_system->m_world = world;
        command_system->m_service_locator = m_service_provider;
        command_system->Initialize();
        m_phase_map[Phase::Commands].push_back(std::move(command_system));

        for (const auto& sys_meta: m_system_metas) {
            auto system = sys_meta.factory();
            system->m_game_world = m_game_world.get();
            system->m_input = input;

            if (std::ranges::find(sys_meta.tags, "ENGINE") != sys_meta.tags.end()) {
                if (const auto engine_sys = dynamic_cast<IEngineSystem*>(system.get())) {
                    engine_sys->m_service_locator = m_service_provider;
                    engine_sys->m_world = world;
                    engine_sys->m_cache_manager = m_cache_manager;
                }
            }
            system->Initialize();
            m_phase_map[sys_meta.phase].push_back(std::move(system));
        }
    }

    void SystemManager::RunSystems(const float delta_time) {
        for (const auto phase: m_phase_order) {
            RunPhase(phase, delta_time);
        }
    }

    void SystemManager::RegisterForSystemCommands(std::string subscriber_name,
                                                  std::function<void(std::vector<std::any>)> command_callback) {
        m_command_callback_subscriber.emplace(subscriber_name, std::move(command_callback));
    }

    void SystemManager::DeregisterForSystemCommands(const std::string& subscriber_name) {
        m_command_callback_subscriber.erase(subscriber_name);
    }


    void SystemManager::RunPhase(const Phase phase, const float delta_time) {
        for (const auto& system: m_phase_map[phase]) {
            system->Run(delta_time);
        }
    }

    void SystemManager::RaiseCommandsEvent(const std::vector<std::any>& commands) const {
        for (const auto& subscriber: m_command_callback_subscriber | std::ranges::views::values) {
            subscriber(commands);
        }
    }
} // namespace
