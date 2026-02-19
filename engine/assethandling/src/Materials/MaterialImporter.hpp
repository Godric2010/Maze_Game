//
// Created by sebastian on 19.02.26.
//

#pragma once
#include <toml++/toml.hpp>

#include "AssetTypes.hpp"


namespace Engine::AssetHandling::Materials
{
    class MaterialImporter
    {
    public:
        static void BuildMaterialAssetFromFile(MaterialAsset& material_asset, const std::string& file_content);

    private:
        static void ReadMaterialProperties(MaterialAsset& material_asset, const toml::table& content_table);
        static void ReadTextureProperties(MaterialTexture& material_texture, const toml::table& content_table, const std::string& texture_type);
        static void ReadColorProperties(MaterialAsset& material_asset, const toml::table& content_table);

        static std::string ReadFieldAsString(toml::node_view<const toml::node> node, const std::string& field_name);
        static glm::vec2 ReadFieldAsVec2(toml::node_view<const toml::node> node, const std::string& field_name);
        static glm::vec4 ReadFieldAsVec4(toml::node_view<const toml::node> node, const std::string& field_name);
        static RenderState ReadFieldAsRenderState(toml::node_view<const toml::node> node,
                                                  const std::string& field_name);
    };
}
