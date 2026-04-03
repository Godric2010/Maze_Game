//
// Created by sebastian on 24.02.26.
//

#pragma once
#include "AssetHandler.hpp"
#include "OpenGlMaterial.hpp"
#include "../../../resources/IGpuMaterialLibrary.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    class OpenGlMaterialLibrary : public Resources::IGpuMaterialLibrary
    {
    public:
        OpenGlMaterialLibrary();

        ~OpenGlMaterialLibrary() override;

        void AddMaterial(const Assets::MaterialHandle& material_handle,
                         const AssetHandling::MaterialAsset& material_asset,
                         uint32_t revision) override;

        void RemoveMaterial(const Assets::MaterialHandle& material) override;
        
        bool HasMaterial(const Assets::MaterialHandle& material) const override;
        
        uint32_t GetMaterialRevision(const Assets::MaterialHandle& material) const override;

        OpenGlMaterial& Get(Assets::MaterialHandle material);

        void ClearMaterials() override;

    private:
        std::unordered_map<Assets::MaterialHandle, OpenGlMaterial> m_material_map;
        std::unordered_map<Assets::MaterialHandle, uint32_t> m_revision_map;
    };
}
