#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../src/LayoutEngine.hpp"

static Engine::Text::Font BuildFakeFont() {
    Engine::Text::Font fake_font{};
    fake_font.line_height = 25;
    fake_font.glyphs = {
        {
            static_cast<uint32_t>('A'), Engine::Text::GlyphMetrics{
                .width = 10,
                .height = 20,
                .bearing_x = 0,
                .bearing_y = 0,
                .advance_x = 12,
            }
        },
        {
            static_cast<uint32_t>('B'), Engine::Text::GlyphMetrics{
                .width = 10,
                .height = 20,
                .bearing_x = 0,
                .bearing_y = 0,
                .advance_x = 12,
            }
        },
        {
            static_cast<uint32_t>('C'), Engine::Text::GlyphMetrics{
                .width = 8,
                .height = 20,
                .bearing_x = 0,
                .bearing_y = 0,
                .advance_x = 10,
            }
        },
    };

    return fake_font;
}

TEST_CASE("LayoutEngineTests - Build one line text string") {
    auto layout_engine = std::make_unique<Engine::Text::LayoutEngine>();
    auto fake_font = BuildFakeFont();
    auto codepoints = std::vector<uint32_t>{
        static_cast<uint32_t>('A'), static_cast<uint32_t>('B'), static_cast<uint32_t>('C')
    };

    auto result = layout_engine->GenerateTextLayout(fake_font, codepoints, Engine::Text::TextAlignment::Left);

    auto glyph_metric_a = fake_font.glyphs[static_cast<uint32_t>('A')];
    auto glyph_metric_b = fake_font.glyphs[static_cast<uint32_t>('B')];

    REQUIRE(result.glyph_data.size() == 3);
    REQUIRE(result.glyph_data[0].x0 == 0 + glyph_metric_a.bearing_x);
    REQUIRE(result.glyph_data[1].x0 == glyph_metric_a.advance_x + glyph_metric_b.bearing_x);
}

TEST_CASE("LayoutEngineTests - Build two line text string") {
    auto layout_engine = std::make_unique<Engine::Text::LayoutEngine>();
    auto fake_font = BuildFakeFont();
    auto codepoints = std::vector<uint32_t>{
        static_cast<uint32_t>('A'), static_cast<uint32_t>('B'), static_cast<uint32_t>('\n'), static_cast<uint32_t>('C')
    };

    auto result = layout_engine->GenerateTextLayout(fake_font, codepoints, Engine::Text::TextAlignment::Left);

    REQUIRE(result.Height() == 45);
    REQUIRE(result.glyph_data[0].y0 == 0);
    REQUIRE(result.glyph_data[1].y0 == 0);
    REQUIRE(result.glyph_data[2].y0 == -fake_font.line_height);
}

TEST_CASE("LayoutEngineTests - Build two lines, center alignment") {
    auto layout_engine = std::make_unique<Engine::Text::LayoutEngine>();
    auto fake_font = BuildFakeFont();
    auto codepoints = std::vector<uint32_t>{
        static_cast<uint32_t>('A'), static_cast<uint32_t>('B'), static_cast<uint32_t>('\n'), static_cast<uint32_t>('C')
    };

    auto result = layout_engine->GenerateTextLayout(fake_font, codepoints, Engine::Text::TextAlignment::Center);

    float expected_x0 = result.min_x + result.Width() * 0.5f - 4;
    REQUIRE(result.glyph_data[2].x0 == expected_x0);
}
