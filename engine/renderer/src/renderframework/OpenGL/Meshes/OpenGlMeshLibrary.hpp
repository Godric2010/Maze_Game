#pragma once
#include <unordered_map>

#include "AssetTypes.hpp"
#include "Datatypes.hpp"
#include "OpenGLMesh.hpp"
#include "../../../resources/IGpuMeshLibrary.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    class OpenGlMeshLibrary : public Resources::IGpuMeshLibrary
    {
    public:
        OpenGlMeshLibrary();

        ~OpenGlMeshLibrary() override;

        void AddMesh(const Assets::MeshHandle& handle, const AssetHandling::MeshAsset& mesh, uint32_t revision) override;

        void RemoveMesh(const Assets::MeshHandle& handle) override;

        bool HasMesh(const Assets::MeshHandle& handle) const override;
        
        uint32_t GetMeshRevision(const Assets::MeshHandle& handle) const override;
        
        void ClearMeshes() override;
        
        OpenGLMesh& GetMesh(const Assets::MeshHandle& handle);

        [[nodiscard]] uint32_t Size() const;

    private:
        std::unordered_map<Assets::MeshHandle, OpenGLMesh> m_meshes{};
        std::unordered_map<Assets::MeshHandle, uint32_t> m_mesh_revisions;
    };
} // namespace
