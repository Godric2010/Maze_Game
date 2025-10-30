#pragma once
#include <any>
#include <vector>

namespace Engine::Ecs::Buffer {
    class SystemCommandQueue {
    public:
        SystemCommandQueue();

        ~SystemCommandQueue();

        template<typename T>
        void PushCommand(T cmd);

        std::vector<std::any> ConsumeAll();

    private:
        std::vector<std::any> m_commands{};
    };
} // namespace
#include "SystemCommandQueue.inl"
