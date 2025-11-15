#include "LayoutEngine.hpp"

namespace Engine::Text {
    TextLayout LayoutEngine::GenerateTextLayout(const Font &font, const std::vector<uint32_t> &codepoints,
                                                const TextAlignment &alignment) {
        TextLayout text_layout{};

        if (codepoints.empty() || font.glyphs.empty()) {
            return text_layout;
        }
        float pen_x = 0.0f;
        float baseline_y = 0.0f;

        float min_x = std::numeric_limits<float>::infinity();
        float min_y = std::numeric_limits<float>::infinity();
        float max_x = -std::numeric_limits<float>::infinity();
        float max_y = -std::numeric_limits<float>::infinity();

        struct LineInfo {
            size_t start_index = 0;
            size_t end_index = 0;
            float width = 0.0f;
        };

        std::vector<LineInfo> line_infos;
        line_infos.reserve(8);

        LineInfo current_line{};
        current_line.start_index = 0;
        current_line.width = 0.0f;

        size_t glyph_count = 0;

        for (const uint32_t codepoint: codepoints) {
            if (codepoint == static_cast<uint32_t>('\n')) {
                current_line.end_index = glyph_count;
                line_infos.push_back(current_line);
                pen_x = 0.0f;
                baseline_y -= static_cast<float>(font.line_height);
                current_line.start_index = glyph_count;
                current_line.width = 0.0f;
            }

            const GlyphMetrics *metrics = font.GetGlyphMetrics(codepoint);
            if (metrics == nullptr) {
                continue;
            }

            const float x0 = pen_x + static_cast<float>(metrics->bearing_x);
            const float y0 = baseline_y - static_cast<float>(metrics->bearing_y);
            const float x1 = x0 + static_cast<float>(metrics->width);
            const float y1 = y0 + static_cast<float>(metrics->height);

            GlyphData glyph_data{};
            glyph_data.x0 = x0;
            glyph_data.y0 = y0;
            glyph_data.x1 = x1;
            glyph_data.y1 = y1;
            glyph_data.u0 = metrics->u0;
            glyph_data.v0 = metrics->v0;
            glyph_data.u1 = metrics->u1;
            glyph_data.v1 = metrics->v1;

            text_layout.glyph_data.push_back(glyph_data);
            ++glyph_count;

            current_line.width += static_cast<float>(metrics->advance_x);

            min_x = std::min(min_x, x0);
            max_x = std::max(max_x, x1);
            min_y = std::min(min_y, y0);
            max_y = std::max(max_y, y1);

            pen_x += static_cast<float>(metrics->advance_x);
        }
        if (glyph_count > 0) {
            current_line.end_index = glyph_count;
            line_infos.push_back(current_line);
        }

        if (glyph_count == 0) {
            return text_layout;
        }

        text_layout.min_x = min_x;
        text_layout.min_y = min_y;
        text_layout.max_x = max_x;
        text_layout.max_y = max_y;

        if (alignment != TextAlignment::Left) {
            for (const auto &[start_index, end_index, width]: line_infos) {
                const float line_width = width;
                float offset_x = 0.0f;

                switch (alignment) {
                    case TextAlignment::Center:
                        offset_x = -line_width * 0.5f;
                        break;
                    case TextAlignment::Right:
                        offset_x = -line_width;
                        break;
                    case TextAlignment::Left:
                    default:
                        break;
                }

                if (offset_x == 0.0f) {
                    continue;
                }

                for (size_t i = start_index; i < end_index; ++i) {
                    auto &glyph_data = text_layout.glyph_data[i];
                    glyph_data.x0 += offset_x;
                    glyph_data.x1 += offset_x;
                }
            }

            min_x = std::numeric_limits<float>::infinity();
            max_x = -std::numeric_limits<float>::infinity();

            for (const auto &glyph: text_layout.glyph_data) {
                min_x = std::min(min_x, glyph.x0);
                max_x = std::max(max_x, glyph.x1);
            }

            text_layout.min_x = min_x;
            text_layout.max_x = max_x;
        }

        return text_layout;
    }
} // namespace
