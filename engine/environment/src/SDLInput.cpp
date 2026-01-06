#include "SDLInput.hpp"

namespace Engine::Environment {
    SDLInput::SDLInput(SDLWindow& window) {
        m_app_events.has_focus = true;
        m_app_events.is_closed = false;

        const auto window_context = window.GetWindowContext();
        const auto window_center = glm::vec2(window_context.width / 2, window_context.height / 2);
        SDL_WarpMouseInWindow(window_context.openGLContext.windowHandle,
                              static_cast<int>(window_center.x),
                              static_cast<int>(window_center.y)
                );
        SDL_ShowCursor(SDL_DISABLE);

        m_last_mouse_pos = window_center;

        m_poll = [&window, this]() {
            window.PollEvents([this](const SDL_Event& event) { this->ProcessInput(event); });
        };
    }

    SDLInput::~SDLInput() = default;

    void SDLInput::PrepareFrame() {
        m_keys_down.clear();
        m_keys_up.clear();
        m_buttons_down.clear();
        m_buttons_up.clear();
        m_current_mouse_delta = glm::vec2(0.0f, 0.0f);
    }

    void SDLInput::ShowMouseCursor(const bool visible) {
        SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
    }

    void SDLInput::PumpInput() {
        m_poll();
    }

    AppEventsSnapshot* SDLInput::GetAppEventSnapshot() {
        return &m_app_events;
    }

    InputSnapshot* SDLInput::GetInputSnapshot() {
        return new InputSnapshot(m_current_mouse_delta,
                                 m_last_mouse_pos,
                                 m_keys_down,
                                 m_keys_held_pressed,
                                 m_keys_up,
                                 m_buttons_down,
                                 m_buttons_held_pressed,
                                 m_buttons_up
                );
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


    void SDLInput::ProcessInput(const SDL_Event& event) {
        switch (event.type) {
            case SDL_QUIT:
                m_app_events.is_closed = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    m_app_events.has_focus = true;
                } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    m_app_events.has_focus = false;
                } else if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    m_app_events.is_closed = true;
                }
                break;
            case SDL_KEYDOWN: {
                if (event.key.repeat) { break; }
                const Key key_down = MapScancode(event.key.keysym.scancode);
                if (key_down == Key::Unknown) { break; }
                m_keys_down.insert(key_down);
                m_keys_held_pressed.insert(key_down);
            }
            break;
            case SDL_KEYUP: {
                if (event.key.repeat)
                    break;
                const Key key_up = MapScancode(event.key.keysym.scancode);
                if (key_up == Key::Unknown)
                    break;
                m_keys_up.insert(key_up);
                m_keys_held_pressed.erase(key_up);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if (event.key.repeat)
                    break;
                const MouseButton mouse_down = mapButton(event.key.keysym.scancode);
                if (mouse_down == MouseButton::Unknown)
                    break;
                m_buttons_down.insert(mouse_down);
                m_buttons_held_pressed.insert(mouse_down);
            }
            break;
            case SDL_MOUSEBUTTONUP: {
                const MouseButton mouse_up = mapButton(event.key.keysym.scancode);
                if (mouse_up == MouseButton::Unknown)
                    break;
                m_buttons_up.insert(mouse_up);
                m_buttons_held_pressed.erase(mouse_up);
            }
            break;
            case SDL_MOUSEMOTION: {
                const auto mouse_pos = glm::vec2(event.motion.x, event.motion.y);
                const glm::vec2 delta_mouse_pos = mouse_pos - m_last_mouse_pos;
                m_current_mouse_pos = mouse_pos;
                m_current_mouse_delta = delta_mouse_pos;
                m_last_mouse_pos = mouse_pos;
            }
            break;
            default:
                break;
        }
    }
} // namespace
