//
// Created by sebastian on 19.02.26.
//

#pragma once
#include <toml++/toml.hpp>
#include <string>
#include "AssetTypes.hpp"


namespace Engine::AssetHandling::Materials
{
    struct MaterialTextureFileData
    {
        std::string name;
        glm::vec2 uv_scale;
        glm::vec2 tiling;
    };

    struct MaterialFileData
    {
        std::string name;
        RenderState render_state;
        size_t render_queue_index;
        std::string shader_name;
        MaterialTextureFileData albedo_texture;
        glm::vec4 base_color;
    };

    class MaterialImporter
    {
    public:
        static void ExtractMaterialFileData(MaterialFileData& material_asset, const std::string& file_content);

    private:
        static void ReadMaterialProperties(MaterialFileData& material_asset, const toml::table& content_table);
        static void ReadTextureProperties(MaterialTextureFileData& material_texture, const toml::table& content_table,
                                          const std::string& texture_type);
        static void ReadColorProperties(MaterialFileData& material_asset, const toml::table& content_table);

        static std::string ReadFieldAsString(toml::node_view<const toml::node> node, const std::string& field_name);
        static int ReadFieldAsInt(toml::node_view<const toml::node> node, const std::string& field_name);
        static glm::vec2 ReadFieldAsVec2(toml::node_view<const toml::node> node, const std::string& field_name);
        static glm::vec4 ReadFieldAsVec4(toml::node_view<const toml::node> node, const std::string& field_name);
        static RenderState ReadFieldAsRenderState(toml::node_view<const toml::node> node,
                                                  const std::string& field_name);
    };
}
