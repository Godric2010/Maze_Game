#include <utility>

#include "Ecs/ISystem.hpp"
#include "EngineBindToken.hpp"
#include "SystemWorld.hpp"
#include "World.hpp"

namespace yarep::ecs {
    void ISystem::Bind(EngineBindToken, input::IInput& input, SystemWorld& world, CommandEvent command_event) {
        m_input = &input;
        m_world = &world;
        m_command_event = std::move(command_event);
    }
}
