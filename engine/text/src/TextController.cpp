#include "../include/TextController.hpp"
#include <spdlog/spdlog.h>
#include "FileReader.hpp"

namespace Engine::Text {
    TextController::TextController() {
        m_font_manager = std::make_unique<FontRasterization::FontManager>(m_font_library);
    }

    FontHandle TextController::LoadFont(std::string font_name, const int pixel_size) const {
        spdlog::info("Loading font {}", font_name);
        const auto font_data = Environment::FileReader::LoadBinaryFromFile("fonts/" + font_name);
        if (!font_data.Ok()) {
            spdlog::error("Failed to load font file {}. Error: {}", font_name, font_data.error.message);
            throw std::runtime_error("Failed to load font " + font_name);
        }
        auto handle = m_font_manager->LoadFont(font_data.value.data, font_data.value.size, font_data.value.name,
                                               pixel_size);
        return handle;
    }
} // namespace
