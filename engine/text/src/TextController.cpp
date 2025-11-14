#include "../include/TextController.hpp"
#include <spdlog/spdlog.h>
#include "FileReader.hpp"

namespace Engine::Text {
    TextController::TextController() {
        m_font_manager = std::make_unique<FontManager>();
        m_utf8_decoder = std::make_unique<Utf8Decoder>();
        m_layout_engine = std::make_unique<LayoutEngine>();
        m_text_mesh_builder = std::make_unique<TextMeshBuilder>();
    }

    FontHandle TextController::LoadFont(std::string font_name, const int pixel_size) const {
        spdlog::info("Loading font {}", font_name);
        const auto font_handle = FontManager::GenerateFontHandle(font_name, pixel_size);
        if (m_font_manager->HasFont(font_handle)) {
            spdlog::info("Font {} has already loaded", font_name);
            return font_handle;
        }

        // Load font data from file...
        const auto font_data = Environment::FileReader::LoadBinaryFromFile("fonts/" + font_name);
        if (!font_data.Ok()) {
            spdlog::error("Failed to load font file {}. Error: {}", font_name, font_data.error.message);
            throw std::runtime_error("Failed to load font " + font_name);
        }

        // ... and create new font data for future use.
        m_font_manager->LoadFont(font_handle, font_data.value.data, font_data.value.size);
        return font_handle;
    }

    TextMesh TextController::BuildTextMesh(const FontHandle font_handle, const std::string &text,
                                           const TextAlignment &alignment) const {
        spdlog::info("Building text mesh {}", text);
        const Font &font = m_font_manager->GetFont(font_handle);

        const auto codepoints = m_utf8_decoder->GenerateCodepointsFromText(text);
        const auto layout = m_layout_engine->GenerateTextLayout(font, codepoints, alignment);
        return m_text_mesh_builder->GenerateTextMesh(layout);
    }

    FontTextureDescriptor TextController::GetTextureDescription(const FontHandle font_handle) const {
        const Font &font = m_font_manager->GetFont(font_handle);
        return font.GetTextureDescriptor();
    }
} // namespace
