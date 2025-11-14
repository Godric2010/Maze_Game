#include "Utf8Decoder.hpp"

#include <spdlog/spdlog.h>

namespace Engine::Text {
    std::vector<uint32_t> Utf8Decoder::GenerateCodepointsFromText(const std::string &text) const {
        std::vector<uint32_t> result;
        result.reserve(text.size());

        const auto *text_data = reinterpret_cast<const unsigned char *>(text.data());
        const std::size_t text_length = text.size();

        std::size_t index = 0;
        while (index < text_length) {
            unsigned char c = text_data[index];

            // is ASCII-Character?
            if (c <= 0x7f) {
                result.push_back(c);
                ++index;
                continue;
            }

            spdlog::warn("{} is no ASCII character... This is currently not supported. Replacing with default char.",
                         c);
            result.push_back(m_replacement_character);
            ++index;
        }
        return result;
    }
} // namespace
