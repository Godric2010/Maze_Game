//
// Created by Sebastian Borsch on 23.11.25.
//

#pragma once
#include <string>
#include <unordered_map>

namespace Engine::Core {
    struct DebugViewData {
    public:
        DebugViewData() = default;

        ~DebugViewData() = default;

        std::unordered_map<uint8_t, std::string> &GetData() {
            return m_debug_data;
        };

        void SetFps(const float fps) {
            m_debug_data[1] = "FPS: " + std::to_string(static_cast<int>(fps));
        }

        void SetDrawCalls(const uint32_t draw_calls) {
            m_debug_data[2] = "Draw calls: " + std::to_string(draw_calls);
        }

    private:
        std::unordered_map<uint8_t, std::string> m_debug_data;
    };
}
