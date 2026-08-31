//
// Created by sebastian on 24.02.26.
//

#pragma once
#include "AssetHandler.hpp"
#include "OpenGlMaterial.hpp"
#include "../../../resources/IGpuMaterialLibrary.hpp"

namespace yarep::renderer::render_framework::open_gl
{
    class OpenGlMaterialLibrary : public resources::IGpuMaterialLibrary
    {
    public:
        OpenGlMaterialLibrary();

        ~OpenGlMaterialLibrary() override;

        void AddMaterial(const assets::MaterialHandle& material_handle,
                         const asset_handling::MaterialAsset& material_asset,
                         uint32_t revision) override;

        void RemoveMaterial(const assets::MaterialHandle& material) override;
        
        bool HasMaterial(const assets::MaterialHandle& material) const override;
        
        uint32_t GetMaterialRevision(const assets::MaterialHandle& material) const override;

        OpenGlMaterial& Get(assets::MaterialHandle material);

        void ClearMaterials() override;

    private:
        std::unordered_map<assets::MaterialHandle, OpenGlMaterial> m_material_map;
        std::unordered_map<assets::MaterialHandle, uint32_t> m_revision_map;
    };
}
