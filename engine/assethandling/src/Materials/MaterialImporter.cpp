#include "MaterialImporter.hpp"

#include <iostream>
#include <toml++/toml.hpp>

namespace Engine::AssetHandling::Materials
{
    const std::string MaterialCategory = "material";
    const std::string TextureCategory = "textures";
    const std::string ColorCategory = "colors";

    const std::string MaterialAssetName = "name";
    const std::string RenderType = "render_type";
    const std::string ShaderName = "shader";

    const std::string Albedo = "albedo";

    const std::string TextureFileName = "file_name";
    const std::string TextureTiling = "tiling";
    const std::string TextureUVScale = "uv_scale";

    const std::string BaseColor = "base";

    void MaterialImporter::BuildMaterialAssetFromFile(MaterialAsset& material_asset, const std::string& file_content)
    {
        toml::table toml_content;
        try
        {
            toml_content = toml::parse(file_content);
        }
        catch (const toml::parse_error& err)
        {
            std::cerr << "Parse error:\n" << err << std::endl;
        }
        ReadMaterialProperties(material_asset, toml_content);
        ReadTextureProperties(material_asset.albedo_texture, toml_content, Albedo);
        ReadColorProperties(material_asset, toml_content);
    }

    void MaterialImporter::ReadMaterialProperties(MaterialAsset& material_asset, const toml::table& content_table)
    {
        const auto category = content_table[MaterialCategory];
        material_asset.name = ReadFieldAsString(category, MaterialAssetName);
        material_asset.render_state = ReadFieldAsRenderState(category, RenderType);
        material_asset.shader_name = ReadFieldAsString(category, ShaderName);
    }

    void MaterialImporter::ReadTextureProperties(MaterialTexture& material_texture, const toml::table& content_table,
                                                 const std::string& texture_type)
    {
        if (const auto tex_table = content_table[TextureCategory][texture_type].as_table())
        {
            material_texture.name = ReadFieldAsString(toml::node_view(*tex_table), TextureFileName);
            material_texture.tiling = ReadFieldAsVec2(toml::node_view(*tex_table), TextureTiling);
            material_texture.uv_scale = ReadFieldAsVec2(toml::node_view(*tex_table), TextureUVScale);
        }
    }

    void MaterialImporter::ReadColorProperties(MaterialAsset& material_asset, const toml::table& content_table)
    {
        const auto category = content_table[ColorCategory];
        material_asset.base_color = ReadFieldAsVec4(category, BaseColor);
    }

    std::string MaterialImporter::ReadFieldAsString(toml::node_view<const toml::node> node,
                                                    const std::string& field_name)
    {
        if (const auto name = node[field_name].value<std::string>())
        {
            return *name;
        }
        throw std::runtime_error("Unknown field name for string conversion: " + field_name);
    }

    glm::vec2 MaterialImporter::ReadFieldAsVec2(toml::node_view<const toml::node> node, const std::string& field_name)
    {
        if (const auto vector2 = node[field_name].as_array())
        {
            glm::vec2 vec;
            vec.x = *vector2->get(0)->value<float>();
            vec.y = *vector2->get(1)->value<float>();
            return vec;
        }

        throw std::runtime_error("Unknown field name for vec2 conversion: " + field_name);
    }

    glm::vec4 MaterialImporter::ReadFieldAsVec4(toml::node_view<const toml::node> node, const std::string& field_name)
    {
        if (const auto vector4 = node[field_name].as_array())
        {
            glm::vec4 vec;
            vec.x = *vector4->get(0)->value<float>();
            vec.y = *vector4->get(1)->value<float>();
            vec.z = *vector4->get(2)->value<float>();
            vec.w = *vector4->get(3)->value<float>();
            return vec;
        }

        throw std::runtime_error("Unknown field name for vec4 conversion: " + field_name);
    }

    RenderState MaterialImporter::ReadFieldAsRenderState(toml::node_view<const toml::node> node,
                                                         const std::string& field_name)
    {
        if (const auto render_state_str = node[field_name].value<std::string>())
        {
            if (render_state_str == "opaque")
            {
                return RenderState::Opaque;
            }
            if (render_state_str == "ui")
            {
                return RenderState::UI;
            }

            throw std::runtime_error("Unknown material render type: " + *render_state_str);
        }
        throw std::runtime_error("Unknown field name for render type: " + field_name);
    }
} // namespace
