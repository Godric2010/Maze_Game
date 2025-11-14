#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Engine::Text {
    /**
     * @class Utf8Decoder
     * Decodes a standard utf-8 string into codepoints, that are used to identify the matching
     * glyph in the font for each character in the string.
     */
    class Utf8Decoder {
    public:
        Utf8Decoder() = default;

        ~Utf8Decoder() = default;

        [[nodiscard]] std::vector<uint32_t> GenerateCodepointsFromText(const std::string &text) const;

    private:
        const uint32_t m_replacement_character = 0xFFFD;
    };
} // namespace
