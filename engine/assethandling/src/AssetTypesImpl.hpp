//
// Created by Sebastian Borsch on 18.01.26.
//

#pragma once
#include "IFileReader.hpp"
#include "../include/AssetTypes.hpp"

namespace Engine::AssetHandling {
    template<typename T>
    struct AssetTraits;

    template<>
    struct AssetTraits<ShaderAsset> {
        static constexpr std::string dir_name = "shaders";

        static std::shared_ptr<ShaderAsset> Load(Environment::Files::IFileReader* file_reader,
                                                 const std::string& asset_name) {
            const auto vertex_shader_content = file_reader->ReadTextFromFile(dir_name + "/" + asset_name + ".vert");
            if (!vertex_shader_content.Ok()) {
                throw std::runtime_error("Failed to load vertex shader" + asset_name);
            }
            const auto fragment_shader_content = file_reader->ReadTextFromFile(dir_name + "/" + asset_name + ".frag");
            if (!fragment_shader_content.Ok()) {
                throw std::runtime_error("Failed to load fragment shader" + asset_name);
            }
            auto shader_asset = std::make_shared<ShaderAsset>();
            shader_asset->name = asset_name;
            shader_asset->vertex_content = vertex_shader_content.value;
            shader_asset->fragment_content = fragment_shader_content.value;
            return shader_asset;
        }
    };

    template<>
    struct AssetTraits<FontAsset> {
        static constexpr std::string dir_name = "fonts";

        static std::shared_ptr<FontAsset> Load(Environment::Files::IFileReader* file_reader,
                                               const std::string& asset_name) {
            const auto font_content = file_reader->ReadBinaryFromFile(dir_name + "/" + asset_name);
            if (!font_content.Ok()) {
                throw std::runtime_error("Failed to load font asset" + asset_name);
            }
            const auto font_asset = std::make_shared<FontAsset>();
            font_asset->name = asset_name;
            font_asset->bytes = font_content.value.data;
            return font_asset;
        }
    };
}
