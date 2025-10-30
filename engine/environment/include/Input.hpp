//
// Created by Sebastian Borsch on 15.08.25.
//

#pragma once
#include <unordered_set>
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
        friend class SDLInput;

    public:
        InputSnapshot() = default;

        ~InputSnapshot() = default;

        [[nodiscard]] bool IsKeyDown(const Key key) const { return m_keyPressedThisFrame.contains(key); };
        [[nodiscard]] bool IsKeyUp(const Key key) const { return m_keyReleasedThisFrame.contains(key); };
        [[nodiscard]] bool IsKeyHeld(const Key key) const { return m_keyHeldThisFrame.contains(key); };

        [[nodiscard]] bool IsMouseDown(const MouseButton button) const {
            return m_mousePressedThisFrame.contains(button);
        };

        [[nodiscard]] bool IsMouseUp(const MouseButton button) const {
            return m_mouseReleasedThisFrame.contains(button);
        };

        [[nodiscard]] bool IsMouseHeld(const MouseButton button) const {
            return m_mouseHeldThisFrame.contains(button);
        };

        [[nodiscard]] glm::vec2 GetMouseDelta() const { return m_mouseDelta; };

    private:
        std::unordered_set<Key> m_keyPressedThisFrame;
        std::unordered_set<Key> m_keyReleasedThisFrame;
        std::unordered_set<Key> m_keyHeldThisFrame;

        std::unordered_set<MouseButton> m_mousePressedThisFrame;
        std::unordered_set<MouseButton> m_mouseReleasedThisFrame;
        std::unordered_set<MouseButton> m_mouseHeldThisFrame;

        glm::vec2 m_mouseDelta{};
    };

    /**
     * @interface IInput
     *
     * An interface for handling input states and snapshots in an application. It provides
     * functionality to prepare input for a new frame and retrieve the current state of inputs.
     */
    class IInput {
    public:
        virtual ~IInput() = default;

        virtual void PrepareFrame() = 0;

        virtual void PumpInput() = 0;

        virtual AppEventsSnapshot *GetAppEventSnapshot() = 0;

        virtual InputSnapshot *GetInputSnapshot() = 0;
    };
}
