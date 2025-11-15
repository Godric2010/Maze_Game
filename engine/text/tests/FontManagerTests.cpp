#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../src/FontManager.hpp"

static std::string valid_font_name = "SpaceFont.ttf";
/* TODO: Fix filepath when loading fonts from the tests - Correct ressource folder not found...
TEST_CASE("FontManagerTests - Load font") {
    auto font_manager = std::make_unique<Engine::Text::FontManager>();

    SECTION("Load font - expect valid font handle") {
        const auto font_handle = font_manager->LoadFont(valid_font_name, 32);
        REQUIRE(font_handle > 0);
    }

    SECTION("Load font - same font name but different size creates different font handle") {
        const auto font_handle_a = font_manager->LoadFont(valid_font_name, 32);
        const auto font_handle_b = font_manager->LoadFont(valid_font_name, 55);
        REQUIRE(font_handle_a > 0);
        REQUIRE(font_handle_b > 0);
        REQUIRE(font_handle_a != font_handle_b);
    }

    SECTION("Load font - Invalid font name throws exception") {
        REQUIRE_THROWS(font_manager->LoadFont("InvalidFontName.ttf", 32));
    }
}

TEST_CASE("FontManagerTests - Get Font") {
    const auto font_manager = std::make_unique<Engine::Text::FontManager>();
    auto font_handle = font_manager->LoadFont(valid_font_name, 32);

    auto font = font_manager->GetFont(font_handle);
    REQUIRE(font.atlas_width > 0);
    REQUIRE(font.atlas_height > 0);

    REQUIRE(font.glyphs.contains(65)); // 65 is ASCII for 'A'

    auto glyph = font.glyphs.find(65)->second;
    REQUIRE(glyph.width > 0);
    REQUIRE(glyph.height > 0);
    REQUIRE(glyph.advance_x > 0);
}

TEST_CASE("FontManagerTests - Get Texture Descriptor") {
    const auto font_manager = std::make_unique<Engine::Text::FontManager>();
    auto font_handle = font_manager->LoadFont(valid_font_name, 32);
    auto font = font_manager->GetFont(font_handle);
    REQUIRE(font.atlas_width > 0);
    REQUIRE(font.atlas_height > 0);

    auto [width, height, pixels] = font.GetTextureDescriptor();
    REQUIRE(width > font.atlas_width);
    REQUIRE(height > font.atlas_height);
    REQUIRE(pixels.size() > font.atlas_width * font.atlas_height);
}
*/
