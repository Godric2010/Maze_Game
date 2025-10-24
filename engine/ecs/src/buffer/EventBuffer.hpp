#pragma once
#include <vector>

#include "ComponentEventBus.hpp"

namespace Engine::Ecs::Buffer {
    template<typename T>
    class EventBuffer {
    public:
        EventBuffer();

        ~EventBuffer();

        void EnqueueEvent(T event);

        void ClearEvents();

        [[nodiscard]] const std::vector<T> &Get() const;

    private:
        std::vector<T> m_event_buffer;
    };
} // namespace
#include "EventBuffer.inl"
