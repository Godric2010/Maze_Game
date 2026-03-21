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
    const std::string RenderQueue = "render_queue";
    const std::string ShaderName = "shader";

    const std::string Albedo = "albedo";

    const std::string TextureFileName = "file_name";
    const std::string TextureTiling = "tiling";
    const std::string TextureUVScale = "uv_scale";

    const std::string BaseColor = "base";

    void MaterialImporter::ExtractMaterialFileData(MaterialFileData& material_asset, const std::string& file_content)
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

    void MaterialImporter::ReadMaterialProperties(MaterialFileData& material_asset, const toml::table& content_table)
    {
        const auto category = content_table[MaterialCategory];
        material_asset.name = ReadFieldAsString(category, MaterialAssetName);
        material_asset.render_state = ReadFieldAsRenderState(category, RenderType);
        material_asset.render_queue_index = ReadFieldAsInt(category, RenderQueue);
        material_asset.shader_name = ReadFieldAsString(category, ShaderName);
    }

    void MaterialImporter::ReadTextureProperties(MaterialTextureFileData& material_texture,
                                                 const toml::table& content_table,
                                                 const std::string& texture_type)
    {
        if (const auto tex_table = content_table[TextureCategory][texture_type].as_table())
        {
            material_texture.name = ReadFieldAsString(toml::node_view(*tex_table), TextureFileName);
            material_texture.tiling = ReadFieldAsVec2(toml::node_view(*tex_table), TextureTiling);
            material_texture.uv_scale = ReadFieldAsVec2(toml::node_view(*tex_table), TextureUVScale);
        }
    }

    void MaterialImporter::ReadColorProperties(MaterialFileData& material_asset, const toml::table& content_table)
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

    int MaterialImporter::ReadFieldAsInt(toml::node_view<const toml::node> node, const std::string& field_name)
    {
        if (const auto value = node[field_name].value<int>())
        {
            return *value;
        }
        throw std::runtime_error("Unknown field name for int conversion: " + field_name);
    }


    glm::vec2 MaterialImporter::ReadFieldAsVec2(toml::node_view<const toml::node> node, const std::string& field_name)
    {
        const auto array = node[field_name].as_array();
        if (!array)
        {
            throw std::runtime_error("Field is not a vec2 array: " + field_name);
        }

        if (array->size() != 2)
        {
            throw std::runtime_error("Field does not contain two elements as required for a vec2: " + field_name);
        }

        const auto* x_node = array->get(0);
        const auto* y_node = array->get(1);

        if (!x_node || !y_node)
        {
            throw std::runtime_error("Field contains null elements for vec2 conversion: " + field_name);
        }

        const auto x = x_node->value<float>();
        const auto y = y_node->value<float>();

        if (!x || !y)
        {
            throw std::runtime_error("Field contains non-float element for vec2: " + field_name);
        }

        return glm::vec2(x.value(), y.value());
    }

    glm::vec4 MaterialImporter::ReadFieldAsVec4(toml::node_view<const toml::node> node, const std::string& field_name)
    {
        const auto array = node[field_name].as_array();
        if (!array)
        {
            throw std::runtime_error("Field is not a vec4 array: " + field_name);
        }

        if (array->size() != 4)
        {
            throw std::runtime_error("Field does not contain four elements as required for a vec4: " + field_name);
        }

        const auto* x_node = array->get(0);
        const auto* y_node = array->get(1);
        const auto* z_node = array->get(2);
        const auto* w_node = array->get(3);

        if (!x_node || !y_node || !z_node || !w_node)
        {
            throw std::runtime_error("Field contains null elements for vec4 conversion: " + field_name);
        }

        const auto x = x_node->value<float>();
        const auto y = y_node->value<float>();
        const auto z = z_node->value<float>();
        const auto w = w_node->value<float>();

        if (!x || !y || !z || !w)
        {
            throw std::runtime_error("Field contains non-float element for vec2: " + field_name);
        }

        return glm::vec4(x.value(), y.value(), z.value(), w.value());
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
