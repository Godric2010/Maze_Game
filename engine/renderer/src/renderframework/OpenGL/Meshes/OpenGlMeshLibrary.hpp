#pragma once
#include <unordered_map>

#include "AssetTypes.hpp"
#include "Datatypes.hpp"
#include "OpenGLMesh.hpp"
#include "../../IGpuMeshLibrary.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    class OpenGlMeshLibrary : public IGpuMeshLibrary
    {
    public:
        OpenGlMeshLibrary();

        ~OpenGlMeshLibrary() override;

        void AddMesh(const Assets::MeshHandle& handle, const AssetHandling::MeshAsset& mesh) override;

        void RemoveMesh(const Assets::MeshHandle& handle) override;

        bool HasMesh(const Assets::MeshHandle& handle) const override;
        
        void ClearMeshes() override;
        
        OpenGLMesh& GetMesh(const Assets::MeshHandle& handle);

        [[nodiscard]] uint32_t Size() const;

    private:
        std::unordered_map<Assets::MeshHandle, OpenGLMesh> m_meshes{};
    };
} // namespace
