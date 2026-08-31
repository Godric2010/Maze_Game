#include "OpenGlMaterial.hpp"
#include "OpenGlMaterialLibrary.hpp"

namespace yarep::renderer::render_framework::open_gl
{
    OpenGlMaterialLibrary::OpenGlMaterialLibrary()
    {
    }

    OpenGlMaterialLibrary::~OpenGlMaterialLibrary()
    {
    }

    void OpenGlMaterialLibrary::AddMaterial(const assets::MaterialHandle& material_handle,
                                            const asset_handling::MaterialAsset& material_asset, 
                                            const uint32_t revision)
    {
        const MaterialTextureRef albedo_texture{
            .texture = material_asset.albedo_texture.texture,
            .uv_scale = material_asset.albedo_texture.uv_scale,
            .tiling = material_asset.albedo_texture.tiling,
        };

        OpenGlMaterial material{
            .render_state = material_asset.render_state,
            .shader = material_asset.shader_handle,
            .albedo_texture = albedo_texture,
            .base_color = material_asset.base_color,
        };
        m_material_map.emplace(material_handle, material);
        m_revision_map[material_handle] = revision;
    }

    void OpenGlMaterialLibrary::RemoveMaterial(const assets::MaterialHandle& material)
    {
        m_material_map.erase(material);
        m_revision_map.erase(material);
    }

    bool OpenGlMaterialLibrary::HasMaterial(const assets::MaterialHandle& material) const
    {
        return m_material_map.contains(material);
    }

    uint32_t OpenGlMaterialLibrary::GetMaterialRevision(const assets::MaterialHandle& material) const
    {
        if (m_revision_map.contains(material))
        {
            return m_revision_map.at(material);
        }

        throw std::invalid_argument("Material handle not found");
    }

    void OpenGlMaterialLibrary::ClearMaterials()
    {
        m_material_map.clear();
    }

    OpenGlMaterial& OpenGlMaterialLibrary::Get(const assets::MaterialHandle material)
    {
        if (m_material_map.contains(material))
        {
            return m_material_map.at(material);
        }
        throw std::invalid_argument("Material handle not found");
    }
} // namespace
