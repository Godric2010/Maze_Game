#pragma once
#include <unordered_map>

#include "AssetTypes.hpp"
#include "Datatypes.hpp"
#include "OpenGlMesh.hpp"
#include "../../../resources/IGpuMeshLibrary.hpp"

namespace yarep::renderer::render_framework::open_gl
{
    class OpenGlMeshLibrary : public resources::IGpuMeshLibrary
    {
    public:
        OpenGlMeshLibrary();

        ~OpenGlMeshLibrary() override;

        void AddMesh(const assets::MeshHandle& handle, const asset_handling::MeshAsset& mesh, uint32_t revision) override;

        void RemoveMesh(const assets::MeshHandle& handle) override;

        bool HasMesh(const assets::MeshHandle& handle) const override;
        
        uint32_t GetMeshRevision(const assets::MeshHandle& handle) const override;
        
        void ClearMeshes() override;
        
        OpenGlMesh& GetMesh(const assets::MeshHandle& handle);

        [[nodiscard]] uint32_t Size() const;

    private:
        std::unordered_map<assets::MeshHandle, OpenGlMesh> m_meshes{};
        std::unordered_map<assets::MeshHandle, uint32_t> m_mesh_revisions;
    };
} // namespace
