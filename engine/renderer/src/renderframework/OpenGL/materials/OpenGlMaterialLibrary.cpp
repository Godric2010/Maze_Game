#include "OpenGlMaterial.hpp"
#include "OpenGlMaterialLibrary.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    OpenGlMaterialLibrary::OpenGlMaterialLibrary()
    {
    }

    OpenGlMaterialLibrary::~OpenGlMaterialLibrary()
    {
    }

    void OpenGlMaterialLibrary::AddMaterial(const Assets::MaterialHandle& material_handle,
                                            const AssetHandling::MaterialAsset& material_asset)
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
    }

    void OpenGlMaterialLibrary::RemoveMaterial(const Assets::MaterialHandle& material)
    {
        m_material_map.erase(material);
    }

    bool OpenGlMaterialLibrary::HasMaterial(const Assets::MaterialHandle& material) const
    {
        return m_material_map.contains(material);
    }

    void OpenGlMaterialLibrary::ClearMaterials()
    {
        m_material_map.clear();
    }

    OpenGlMaterial& OpenGlMaterialLibrary::Get(const Assets::MaterialHandle material)
    {
        if (m_material_map.contains(material))
        {
            return m_material_map.at(material);
        }
        throw std::invalid_argument("Material handle not found");
    }
} // namespace
