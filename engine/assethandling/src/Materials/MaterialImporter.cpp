#include "MaterialImporter.hpp"

namespace Engine::AssetHandling::Materials
{
    const std::string MaterialCategory = "material";
    const std::string TextureCategory = "textures";
    const std::string ColorCategory = "colors";

    const std::string MaterialAssetName = "name";
    const std::string RenderType = "render_type";
    const std::string RenderQueue = "render_queue";
    const std::string ShaderName = "shader";

    const std::string Albedo = "albedo";

    const std::string TextureFileName = "file_name";
    const std::string TextureTiling = "tiling";
    const std::string TextureUVScale = "uv_scale";

    const std::string BaseColor = "base";

    void MaterialImporter::ExtractMaterialFileData(MaterialFileData& material_asset, const std::string& file_content)
    {
        const auto toml_doc = Utilities::Toml::TomlDocument(file_content);
     
        ReadMaterialProperties(material_asset, toml_doc.GetRequiredTable(MaterialCategory));
        ReadTextureProperties(material_asset.albedo_texture, toml_doc.GetOptionalTable(TextureCategory), Albedo);
        ReadColorProperties(material_asset, toml_doc.GetRequiredTable(ColorCategory));
    }

    void MaterialImporter::ReadMaterialProperties(MaterialFileData& material_asset,
                                                  const Utilities::Toml::TomlTable& table)
    {
        material_asset.name = table.GetRequiredString(MaterialAssetName);
        material_asset.render_state = table.GetRequiredEnum<RenderState>(RenderType, RenderStateMap);
        material_asset.render_queue_index = table.GetRequiredInt(RenderQueue);
        material_asset.shader_name = table.GetRequiredString(ShaderName);
    }

    void MaterialImporter::ReadTextureProperties(MaterialTextureFileData& material_texture,
                                                 const std::optional<Utilities::Toml::TomlTable>& table,
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
        
        material_texture.name = tex_table.value().GetRequiredString(TextureFileName);
        material_texture.tiling = tex_table.value().GetRequiredVec2(TextureTiling);
        material_texture.uv_scale = tex_table.value().GetRequiredVec2(TextureUVScale);
    }

    void MaterialImporter::ReadColorProperties(MaterialFileData& material_asset,
                                               const Utilities::Toml::TomlTable& table)
    {
        material_asset.base_color = table.GetRequiredVec4(BaseColor);
    }
} // namespace
