#pragma once
#include <any>
#include <vector>

namespace Engine::Ecs::Buffer {
    class SystemCommandQueue {
    public:
        SystemCommandQueue();

        ~SystemCommandQueue();

        void PushCommand(const std::any& cmd);

        std::vector<std::any> ConsumeAll();

    private:
        std::vector<std::any> m_commands{};
    };
} // namespace
#include "SystemCommandQueue.inl"
