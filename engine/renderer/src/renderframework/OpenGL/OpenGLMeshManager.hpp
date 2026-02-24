#pragma once
#include <unordered_map>

#include "AssetTypes.hpp"
#include "Datatypes.hpp"
#include "OpenGLMesh.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl {
    class OpenGlMeshManager {
    public:
        OpenGlMeshManager();

        ~OpenGlMeshManager();

        void AddMesh(const AssetHandling::MeshAsset &mesh, Assets::MeshHandle handle);

        OpenGLMesh &GetMesh(const Assets::MeshHandle &handle);

        [[nodiscard]] uint32_t Size() const;

        void RemoveMesh(const Assets::MeshHandle &handle);

        void Clear();

    private:
        std::unordered_map<Assets::MeshHandle, OpenGLMesh> m_meshes{};
    };
} // namespace