#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "OpenGLMesh.hpp"
#include "../../Datatypes.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl {
    class OpenGlMeshManager {
    public:
        OpenGlMeshManager();

        ~OpenGlMeshManager();

        MeshHandle AddMesh(const MeshAsset &mesh);

        OpenGLMesh &GetMesh(const MeshHandle &handle);

        [[nodiscard]] uint32_t Size() const;

        void RemoveMesh(const MeshHandle &handle);

        void Clear();

    private:
        MeshHandle m_mesh_handle;
        std::unordered_map<MeshHandle, OpenGLMesh> m_meshes;
    };
} // namespace