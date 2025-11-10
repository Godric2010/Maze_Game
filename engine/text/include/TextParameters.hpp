//
// Created by Sebastian Borsch on 09.11.25.
//

#pragma once
#include <optional>
#include "FontHandle.hpp"

namespace Engine::Text {
    struct TextParameters {
        FontHandle font;
        std::string text;
        std::optional<float> wrap_width = 0;
        bool pixel_snap = true;
    };
}
