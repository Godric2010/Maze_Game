//
// Created by sebastian on 22.03.26.
//

#pragma once
#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace yarep::Renderer::Resources
{
    class IGpuMaterialLibrary
    {
    public:
        virtual ~IGpuMaterialLibrary() = default;

        virtual void AddMaterial(const assets::MaterialHandle& material_handle,
                                 const AssetHandling::MaterialAsset& material_asset,
                                 uint32_t revision) = 0;

        virtual void RemoveMaterial(const assets::MaterialHandle& material_handle) = 0;

        [[nodiscard]] virtual bool HasMaterial(const assets::MaterialHandle& material_handle) const = 0;

        virtual uint32_t GetMaterialRevision(const assets::MaterialHandle& material_handle) const = 0;

        virtual void ClearMaterials() = 0;
    };
}
