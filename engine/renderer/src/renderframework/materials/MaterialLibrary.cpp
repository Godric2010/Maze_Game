#include "MaterialLibrary.hpp"

namespace Engine::Renderer::RenderFramework::Materials
{
    MaterialLibrary::MaterialLibrary(AssetHandling::AssetHandler* asset_handler)
    {
        if (asset_handler == nullptr)
        {
            throw std::invalid_argument("Cannot build Material Library. asset_handler is nullptr");
        }
        m_asset_handler = asset_handler;
    }

    MaterialLibrary::~MaterialLibrary()
    {
        Clear();
        m_asset_handler = nullptr;
    }

    void MaterialLibrary::Add(Assets::MaterialHandle material_handle)
    {
        const auto asset = m_asset_handler->GetAsset<AssetHandling::MaterialAsset>(material_handle);


        const MaterialTextureRef albedo_texture{
            .texture = asset->albedo_texture.texture,
            .uv_scale = asset->albedo_texture.uv_scale,
            .tiling = asset->albedo_texture.tiling,
        };

        Material material{
            .render_state = asset->render_state,
            .shader = asset->shader_handle,
            .albedo_texture = albedo_texture,
            .base_color = asset->base_color,
        };
        m_material_map.emplace(material_handle, material);
    }

    void MaterialLibrary::Remove(const Assets::MaterialHandle material)
    {
        m_material_map.erase(material);
    }

    void MaterialLibrary::Clear()
    {
        m_material_map.clear();
    }

    Material& MaterialLibrary::Get(const Assets::MaterialHandle material)
    {
        if (m_material_map.contains(material))
        {
            return m_material_map.at(material);
        }
        throw std::invalid_argument("Material handle not found");
    }
} // namespace
