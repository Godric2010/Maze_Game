//
// Created by sebastian on 22.03.26.
//

#pragma once
#include "AssetTypes.hpp"
#include "Assets/AssetHandleTypes.hpp"

namespace yarep::renderer::resources
{
    class IGpuMeshLibrary
    {
    public:
        virtual ~IGpuMeshLibrary() = default;

        virtual void AddMesh(const assets::MeshHandle& mesh_handle, const asset_handling::MeshAsset& mesh_asset, uint32_t revision) = 0;

        virtual void RemoveMesh(const assets::MeshHandle& mesh_handle) = 0;
        
        [[nodiscard]] virtual bool HasMesh(const assets::MeshHandle& mesh_handle) const = 0;
        
        virtual uint32_t GetMeshRevision(const assets::MeshHandle& mesh_handle) const = 0;
        
        virtual void ClearMeshes() = 0;
    };
}
