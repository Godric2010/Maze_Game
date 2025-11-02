#include "SDLInput.hpp"

namespace Engine::Environment {
    SDLInput::SDLInput(SDLWindow &window) {
        m_inputSnapshot = {};
        m_appEvents.has_focus = true;
        m_appEvents.is_closed = false;

        const auto window_context = window.GetWindowContext();
        const auto window_center = glm::vec2(window_context.width / 2, window_context.height / 2);
        SDL_WarpMouseInWindow(window_context.openGLContext.windowHandle,
                              static_cast<int>(window_center.x),
                              static_cast<int>(window_center.y));
        SDL_ShowCursor(SDL_DISABLE);

        m_lastMousePos = window_center;

        Poll = [&window, this]() {
            window.PollEvents([this](const SDL_Event &event) { this->ProcessInput(event); });
        };
    }

    SDLInput::~SDLInput() = default;

    void SDLInput::PrepareFrame() {
        m_inputSnapshot.m_key_pressed_this_frame.clear();
        m_inputSnapshot.m_key_released_this_frame.clear();
        m_inputSnapshot.m_mouse_pressed_this_frame.clear();
        m_inputSnapshot.m_mouse_released_this_frame.clear();
        m_inputSnapshot.m_mouse_delta = glm::vec2(0.0f);
    }

    void SDLInput::ShowMouseCursor(const bool visible) {
        SDL_ShowCursor(visible ? SDL_DISABLE : SDL_ENABLE);
    }

    void SDLInput::PumpInput() {
        Poll();
    }

    AppEventsSnapshot *SDLInput::GetAppEventSnapshot() {
        return &m_appEvents;
    }

    InputSnapshot *SDLInput::GetInputSnapshot() {
        return &m_inputSnapshot;
    }

    inline Key MapScancode(const SDL_Scancode scancode) {
        switch (scancode) {
            case SDL_SCANCODE_ESCAPE:
                return Key::Esc;
            case SDL_SCANCODE_KP_ENTER:
                return Key::Enter;
            case SDL_SCANCODE_SPACE:
                return Key::Space;
            case SDL_SCANCODE_LSHIFT:
                return Key::ShiftLeft;
            case SDL_SCANCODE_W:
                return Key::W;
            case SDL_SCANCODE_A:
                return Key::A;
            case SDL_SCANCODE_D:
                return Key::D;
            case SDL_SCANCODE_S:
                return Key::S;
            default:
                return Key::Unknown;
        }
    }

    inline MouseButton mapButton(const uint8_t button) {
        switch (button) {
            case SDL_BUTTON_LEFT:
                return MouseButton::Left;
            case SDL_BUTTON_RIGHT:
                return MouseButton::Right;
            case SDL_BUTTON_MIDDLE:
                return MouseButton::Middle;
            default:
                return MouseButton::Unknown;
        }
    }


    void SDLInput::ProcessInput(const SDL_Event &event) {
        switch (event.type) {
            case SDL_QUIT:
                m_appEvents.is_closed = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    m_appEvents.has_focus = true;
                } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    m_appEvents.has_focus = false;
                } else if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    m_appEvents.is_closed = true;
                }
                break;
            case SDL_KEYDOWN: {
                if (event.key.repeat) { break; }
                const Key key_down = MapScancode(event.key.keysym.scancode);
                if (key_down == Key::Unknown) { break; }
                m_inputSnapshot.m_key_pressed_this_frame.insert(key_down);
                m_inputSnapshot.m_key_held_this_frame.insert(key_down);
            }
            break;
            case SDL_KEYUP: {
                if (event.key.repeat)break;
                const Key key_up = MapScancode(event.key.keysym.scancode);
                if (key_up == Key::Unknown) break;
                m_inputSnapshot.m_key_released_this_frame.insert(key_up);
                m_inputSnapshot.m_key_held_this_frame.erase(key_up);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if (event.key.repeat)break;
                const MouseButton mouse_down = mapButton(event.key.keysym.scancode);
                if (mouse_down == MouseButton::Unknown) break;
                m_inputSnapshot.m_mouse_pressed_this_frame.insert(mouse_down);
                m_inputSnapshot.m_mouse_held_this_frame.insert(mouse_down);
            }
            break;
            case SDL_MOUSEBUTTONUP: {
                const MouseButton mouse_up = mapButton(event.key.keysym.scancode);
                if (mouse_up == MouseButton::Unknown) break;
                m_inputSnapshot.m_mouse_released_this_frame.insert(mouse_up);
                m_inputSnapshot.m_mouse_held_this_frame.insert(mouse_up);
            }
            break;
            case SDL_MOUSEMOTION: {
                const auto mouse_pos = glm::vec2(event.motion.x, event.motion.y);
                const glm::vec2 delta_mouse_pos = mouse_pos - m_lastMousePos;
                m_inputSnapshot.m_mouse_pos = mouse_pos;
                m_inputSnapshot.m_mouse_delta = delta_mouse_pos;
                m_lastMousePos = mouse_pos;
            }
            break;
            default:
                break;
        }
    }
} // namespace
