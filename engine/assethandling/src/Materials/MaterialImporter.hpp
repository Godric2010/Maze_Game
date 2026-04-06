//
// Created by sebastian on 19.02.26.
//

#pragma once
#include <string>
#include <glm/vec2.hpp>
#include "AssetTypes.hpp"
#include "toml/TomlDocument.hpp"


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
        static void ReadMaterialProperties(MaterialFileData& material_asset, const Utilities::Toml::TomlTable& table);
        static void ReadTextureProperties(MaterialTextureFileData& material_texture, const std::optional<Utilities::Toml::TomlTable>& table,
                                          const std::string& texture_type);
        static void ReadColorProperties(MaterialFileData& material_asset, const Utilities::Toml::TomlTable& table);
    };
}
