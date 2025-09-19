#include "CommandBuffer.hpp"

namespace Engine::Ecs {
    CommandBuffer::CommandBuffer() = default;

    CommandBuffer::~CommandBuffer() = default;

    void CommandBuffer::Enqueue(Command command) {
        m_commands.emplace_back(std::move(command));
    }

    void CommandBuffer::Clear() {
        m_commands.clear();
    }

    const std::vector<Command> &CommandBuffer::GetCommands() const {
        return m_commands;
    }


} // namespace