#include "EventBuffer.hpp"

namespace Engine::Ecs::Buffer {
    template<typename T>
    EventBuffer<T>::EventBuffer() = default;

    template<typename T>
    EventBuffer<T>::~EventBuffer() = default;

    template<typename T>
    void EventBuffer<T>::EnqueueEvent(T event) {
        m_event_buffer.emplace_back(std::move(event));
    }

    template<typename T>
    void EventBuffer<T>::ClearEvents() {
        m_event_buffer.clear();
    }

    template<typename T>
    const std::vector<T> &EventBuffer<T>::Get() const {
        return m_event_buffer;
    }
} // namespace
