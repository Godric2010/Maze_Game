//
// Created by Sebastian Borsch on 15.08.25.
//

#pragma once
#include <unordered_set>
#include <utility>
#include <vector>
#include <glm/vec2.hpp>

namespace Engine::Environment {
    enum class Key {
        Unknown,
        W,
        A,
        S,
        D,
        Enter,
        Esc,
        Space,
        ShiftLeft,
    };

    enum class MouseButton {
        Unknown,
        Left,
        Right,
        Middle,
    };

    enum class PressState {
        Down,
        Up,
        Pressed,
    };

    struct AppEventsSnapshot {
        bool is_closed;
        bool has_focus;
    };

    class InputSnapshot {
    public:
        InputSnapshot(glm::vec2 mouse_delta, glm::vec2 mouse_pos, std::unordered_set<Key> keys_down,
                      std::unordered_set<Key> keys_held, std::unordered_set<Key> keys_up,
                      std::unordered_set<MouseButton> mouse_down, std::unordered_set<MouseButton> mouse_held,
                      std::unordered_set<MouseButton> mouse_up) {
            m_mouse_delta = mouse_delta;
            m_mouse_pos = mouse_pos;
            m_key_down_this_frame = std::move(keys_down);
            m_key_held_this_frame = std::move(keys_held);
            m_key_up_this_frame = std::move(keys_up);
            m_mouse_held_this_frame = std::move(mouse_held);
            m_mouse_down_this_frame = std::move(mouse_down);
            m_mouse_up_this_frame = std::move(mouse_up);
        }

        ~InputSnapshot() = default;

        [[nodiscard]] bool IsKeyDown(const Key key) const { return m_key_down_this_frame.contains(key); };
        [[nodiscard]] bool IsKeyUp(const Key key) const { return m_key_up_this_frame.contains(key); };
        [[nodiscard]] bool IsKeyHeld(const Key key) const { return m_key_held_this_frame.contains(key); };

        [[nodiscard]] bool IsKeyInState(const Key key, const PressState state) const {
            switch (state) {
                case PressState::Down:
                    return m_key_down_this_frame.contains(key);
                case PressState::Up:
                    return m_key_up_this_frame.contains(key);
                case PressState::Pressed:
                    return m_key_held_this_frame.contains(key);
            }
            return false;
        }

        [[nodiscard]] bool IsMouseButtonInState(const MouseButton button, PressState state) const {
            switch (state) {
                case PressState::Down:
                    return m_mouse_down_this_frame.contains(button);
                case PressState::Up:
                    return m_mouse_up_this_frame.contains(button);
                case PressState::Pressed:
                    return m_mouse_held_this_frame.contains(button);
            }
            return false;
        }

        [[nodiscard]] bool IsMouseDown(const MouseButton button) const {
            return m_mouse_down_this_frame.contains(button);
        };

        [[nodiscard]] bool IsMouseUp(const MouseButton button) const {
            return m_mouse_up_this_frame.contains(button);
        };

        [[nodiscard]] bool IsMouseHeld(const MouseButton button) const {
            return m_mouse_held_this_frame.contains(button);
        };

        [[nodiscard]] glm::vec2 GetMousePosition() const { return m_mouse_pos; }
        [[nodiscard]] glm::vec2 GetMouseDelta() const { return m_mouse_delta; };

    private:
        std::unordered_set<Key> m_key_down_this_frame;
        std::unordered_set<Key> m_key_up_this_frame;
        std::unordered_set<Key> m_key_held_this_frame;

        std::unordered_set<MouseButton> m_mouse_down_this_frame;
        std::unordered_set<MouseButton> m_mouse_up_this_frame;
        std::unordered_set<MouseButton> m_mouse_held_this_frame;

        glm::vec2 m_mouse_delta{};
        glm::vec2 m_mouse_pos{};
    };

    /**
     * @interface IEnvInput
     *
     * An interface for handling input states and snapshots in an application. It provides
     * functionality to prepare input for a new frame and retrieve the current state of inputs.
     */
    class IEnvInput {
    public:
        virtual ~IEnvInput() = default;

        virtual void PrepareFrame() = 0;

        virtual void PumpInput() = 0;

        virtual void ShowMouseCursor(bool visible) = 0;

        virtual AppEventsSnapshot* GetAppEventSnapshot() = 0;

        virtual InputSnapshot* GetInputSnapshot() = 0;
    };
}
