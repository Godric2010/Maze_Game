#include "MaterialImporter.hpp"

namespace yarep::asset_handling::materials
{
    const std::string material_category = "material";
    const std::string texture_category = "textures";
    const std::string color_category = "colors";

    const std::string material_asset_name = "name";
    const std::string render_type = "render_type";
    const std::string render_queue = "render_queue";
    const std::string shader_name = "shader";

    const std::string albedo = "albedo";

    const std::string texture_file_name = "file_name";
    const std::string texture_tiling = "tiling";
    const std::string texture_uv_scale = "uv_scale";

    const std::string base_color = "base";

    void MaterialImporter::ExtractMaterialFileData(MaterialFileData& material_asset, const std::string& file_content)
    {
        const auto toml_doc = utilities::toml_utils::TomlDocument(file_content);
     
        ReadMaterialProperties(material_asset, toml_doc.GetRequiredTable(material_category));
        ReadTextureProperties(material_asset.albedo_texture, toml_doc.GetOptionalTable(texture_category), albedo);
        ReadColorProperties(material_asset, toml_doc.GetRequiredTable(color_category));
    }

    void MaterialImporter::ReadMaterialProperties(MaterialFileData& material_asset,
                                                  const utilities::toml_utils::TomlTable& table)
    {
        material_asset.name = table.GetRequiredString(material_asset_name);
        material_asset.render_state = table.GetRequiredEnum<RenderState>(render_type, render_state_map);
        material_asset.render_queue_index = table.GetRequiredInt(render_queue);
        material_asset.shader_name = table.GetRequiredString(shader_name);
    }

    void MaterialImporter::ReadTextureProperties(MaterialTextureFileData& material_texture,
                                                 const std::optional<utilities::toml_utils::TomlTable>& table,
                                                 const std::string& texture_type)
    {
        if (!table.has_value())
        {
            return;
        }
        const auto tex_table = table->GetOptionalTable(texture_type);
        if (!tex_table)
        {
            return;
        }
        
        material_texture.name = tex_table.value().GetRequiredString(texture_file_name);
        material_texture.tiling = tex_table.value().GetRequiredVec2(texture_tiling);
        material_texture.uv_scale = tex_table.value().GetRequiredVec2(texture_uv_scale);
    }

    void MaterialImporter::ReadColorProperties(MaterialFileData& material_asset,
                                               const utilities::toml_utils::TomlTable& table)
    {
        material_asset.base_color = table.GetRequiredVec4(base_color);
    }
} // namespace
