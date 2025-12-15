#include <utility>

#include "Ecs/ISystem.hpp"
#include "EngineBindToken.hpp"
#include "SystemWorld.hpp"
#include "World.hpp"

namespace Engine::Ecs {
    void ISystem::Bind(EngineBindToken, Input::IInput& input, SystemWorld& world, CommandEvent command_event) {
        m_input = &input;
        m_world = &world;
        m_command_event = std::move(command_event);
    }
}
