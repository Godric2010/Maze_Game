#include "TextureImporter.hpp"

#include <filesystem>
#include <lodepng.h>
#include <stdexcept>
#include <fmt/format.h>

namespace Engine::AssetHandling::Textures
{
    void TextureImporter::BuildTextureFromFile(TextureAsset& texture_asset, const std::vector<uint8_t>& bytes,
                                               const std::string& file_name)
    {
        unsigned texture_width;
        unsigned texture_height;
        std::vector<uint8_t> pixels;

        unsigned error_code = lodepng::decode(pixels, texture_width, texture_height, bytes.data(), bytes.size());
        if (error_code != 0)
        {
            throw std::runtime_error(fmt::format("Texture import failed for '{}'. Error message: {}", file_name, error_code));
        }

        texture_asset.width = static_cast<uint32_t>(texture_width);
        texture_asset.height = static_cast<uint32_t>(texture_height);
        texture_asset.format = PixelFormat::RGBA8;
        texture_asset.bytes = std::move(pixels);
    }
} // namespace
