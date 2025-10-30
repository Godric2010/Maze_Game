#pragma once
namespace Engine::Ecs::Buffer {
    inline SystemCommandQueue::SystemCommandQueue() = default;

    inline SystemCommandQueue::~SystemCommandQueue() = default;

    template<typename T>
    void SystemCommandQueue::PushCommand(T cmd) {
        m_commands.emplace_back(std::make_any<T>(std::move(cmd)));
    }

    inline std::vector<std::any> SystemCommandQueue::ConsumeAll() {
        std::vector<std::any> result;
        if (m_commands.empty()) {
            return result;
        }
        result.swap(m_commands);
        return result;
    }
} // namespace
