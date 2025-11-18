#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include <memory>
#include <string>
#include "../src/Utf8Decoder.hpp"

static uint32_t invalid_character = 0xFFFD;

static bool HasInvalidCharacters(const std::vector<uint32_t> &codepoints) {
    for (const uint32_t codepoint: codepoints) {
        if (codepoint == invalid_character) {
            return true;
        }
    }
    return false;
}

TEST_CASE("Utf8DecoderTests - Set valid string, expect only valid codepoints", "[text][fast]") {
    const auto decoder = std::make_unique<Engine::Text::Utf8Decoder>();

    SECTION("Use simple text, no spaces, no special characters") {
        const std::string text = "MyTestText";

        const auto codepoints = decoder->GenerateCodepointsFromText(text);
        REQUIRE(codepoints.size() == text.size());
        REQUIRE_FALSE(HasInvalidCharacters(codepoints));
    }

    SECTION("Text with spaces") {
        const std::string text = "My Test Text";
        const auto codepoints = decoder->GenerateCodepointsFromText(text);
        REQUIRE(codepoints.size() == text.size());
        REQUIRE_FALSE(HasInvalidCharacters(codepoints));
    }

    SECTION("Text with punktuation") {
        const std::string text = "My,Test-Text!";
        const auto codepoints = decoder->GenerateCodepointsFromText(text);
        REQUIRE(codepoints.size() == text.size());
        REQUIRE_FALSE(HasInvalidCharacters(codepoints));
    }

    SECTION("Text with numbers") {
        const std::string text = "MyTestText2";
        const auto codepoints = decoder->GenerateCodepointsFromText(text);
        REQUIRE(codepoints.size() == text.size());
        REQUIRE_FALSE(HasInvalidCharacters(codepoints));
    }
}

TEST_CASE("Utf8DecoderTests - Set non-ascii string, expect invalid codepoints", "[text][fast]") {
    const auto decoder = std::make_unique<Engine::Text::Utf8Decoder>();

    SECTION("Invalid UTF-8 char") {
        const std::string text = "❤️";
        const auto codepoints = decoder->GenerateCodepointsFromText(text);
        REQUIRE(codepoints.size() == text.size());
        REQUIRE(HasInvalidCharacters(codepoints));
    }

    SECTION("Invalid UTF-8 string") {
        const std::string text = "MyText❤️";
        const auto codepoints = decoder->GenerateCodepointsFromText(text);
        REQUIRE(codepoints.size() == text.size());
        REQUIRE(HasInvalidCharacters(codepoints));
    }
}
