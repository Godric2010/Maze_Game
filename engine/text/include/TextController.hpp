#pragma once
#include <AssetHandler.hpp>
#include <memory>
#include <string>
#include "Types.hpp"

#include "../src/FontManager.hpp"
#include "../src/LayoutEngine.hpp"
#include "../src/TextMeshBuilder.hpp"
#include "../src/Utf8Decoder.hpp"

namespace Engine::Text {
    /**
     * @class TextController
     * This class is responsible for orchestrating
     * the usage of fonts and building renderable texts.
     */
    class TextController {
    public:
        explicit TextController(AssetHandling::AssetHandler* asset_handler);

        ~TextController() = default;

        [[nodiscard]] FontHandleResult LoadFont(const std::string& font_name, int pixel_size) const;

        [[nodiscard]] TextMesh BuildTextMesh(FontHandle font_handle, const std::string &text,
                                             const TextAlignment &alignment) const;

        [[nodiscard]] FontTextureDescriptor GetTextureDescription(FontHandle font_handle) const;

    private:
        std::unique_ptr<FontManager> m_font_manager;
        std::unique_ptr<Utf8Decoder> m_utf8_decoder;
        std::unique_ptr<LayoutEngine> m_layout_engine;
        std::unique_ptr<TextMeshBuilder> m_text_mesh_builder;
    };
} // namespace
