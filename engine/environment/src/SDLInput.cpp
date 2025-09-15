#include "SDLInput.hpp"

namespace Engine::Environment {
    SDLInput::SDLInput(SDLWindow &window) {
        m_inputSnapshot = {};
        m_appEvents.HasFocus = true;
        m_appEvents.IsClosed = false;

        const auto windowContext = window.GetWindowContext();
        const auto windowCenter = glm::vec2(windowContext.width / 2, windowContext.height / 2);
        SDL_WarpMouseInWindow(windowContext.openGLContext.windowHandle,
                              static_cast<int>(windowCenter.x),
                              static_cast<int>(windowCenter.y));
        SDL_ShowCursor(SDL_DISABLE);

        m_lastMousePos = windowCenter;

        Poll = [&window, this]() {
            window.PollEvents([this](const SDL_Event &event) { this->ProcessInput(event); });
        };
    }

    SDLInput::~SDLInput() = default;

    void SDLInput::PrepareFrame() {

        m_inputSnapshot.m_keyPressedThisFrame.clear();
        m_inputSnapshot.m_keyReleasedThisFrame.clear();
        m_inputSnapshot.m_mousePressedThisFrame.clear();
        m_inputSnapshot.m_mouseReleasedThisFrame.clear();
        m_inputSnapshot.m_mouseDelta = glm::vec2(0.0f);
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

    inline Key mapScancode(SDL_Scancode scancode) {
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
                m_appEvents.IsClosed = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    m_appEvents.HasFocus = true;
                } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    m_appEvents.HasFocus = false;
                } else if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    m_appEvents.IsClosed = true;
                }
                break;
            case SDL_KEYDOWN: {
                if (event.key.repeat) { break; }
                const Key keyDown = mapScancode(event.key.keysym.scancode);
                if (keyDown == Key::Unknown) { break; }
                m_inputSnapshot.m_keyPressedThisFrame.insert(keyDown);
                m_inputSnapshot.m_keyHeldThisFrame.insert(keyDown);
            }
            break;
            case SDL_KEYUP: {
                if (event.key.repeat)break;
                const Key keyUp = mapScancode(event.key.keysym.scancode);
                if (keyUp == Key::Unknown) break;
                m_inputSnapshot.m_keyReleasedThisFrame.insert(keyUp);
                m_inputSnapshot.m_keyHeldThisFrame.erase(keyUp);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if (event.key.repeat)break;
                const MouseButton mouseDown = mapButton(event.key.keysym.scancode);
                if (mouseDown == MouseButton::Unknown) break;
                m_inputSnapshot.m_mousePressedThisFrame.insert(mouseDown);
                m_inputSnapshot.m_mouseHeldThisFrame.insert(mouseDown);
            }
            break;
            case SDL_MOUSEBUTTONUP: {
                const MouseButton mouseUp = mapButton(event.key.keysym.scancode);
                if (mouseUp == MouseButton::Unknown) break;
                m_inputSnapshot.m_mouseReleasedThisFrame.insert(mouseUp);
                m_inputSnapshot.m_mouseHeldThisFrame.insert(mouseUp);
            }
            break;
            case SDL_MOUSEMOTION: {
                const auto mousePos = glm::vec2(event.motion.x, event.motion.y);
                const glm::vec2 deltaMousePos = mousePos - m_lastMousePos;
                m_inputSnapshot.m_mouseDelta = deltaMousePos;
                m_lastMousePos = mousePos;
            }
            break;
            default:
                break;
        }
    }
} // namespace
