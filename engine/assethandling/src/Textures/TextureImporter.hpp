//
// Created by sebastian on 07.02.26.
//

#pragma once
#include "AssetTypes.hpp"


namespace yarep::asset_handling::textures
{
    class TextureImporter
    {
    public:
        static void BuildTextureFromFile(TextureAsset& texture_asset, const std::vector<uint8_t>& bytes, const std::string& file_name);
    };
}
