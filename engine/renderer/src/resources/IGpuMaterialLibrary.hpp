//
// Created by sebastian on 22.03.26.
//

#pragma once
#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace Engine::Renderer::Resources
{
    class IGpuMaterialLibrary
    {
    public:
        virtual ~IGpuMaterialLibrary() = default;

        virtual void AddMaterial(const Assets::MaterialHandle& material_handle,
                                 const AssetHandling::MaterialAsset& material_asset) = 0;

        virtual void RemoveMaterial(const Assets::MaterialHandle& material_handle) = 0;

        virtual bool HasMaterial(const Assets::MaterialHandle& material_handle) const = 0;

        virtual void ClearMaterials() = 0;
    };
}
