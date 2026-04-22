#include "OpenGlMeshLibrary.hpp"

#include <stdexcept>
#include <ranges>
#include <GL/glew.h>

namespace Engine::Renderer::RenderFramework::OpenGl {
    OpenGlMeshLibrary::OpenGlMeshLibrary() {
        m_meshes.clear();
    }

    OpenGlMeshLibrary::~OpenGlMeshLibrary() = default;

    void OpenGlMeshLibrary::AddMesh(const Assets::MeshHandle& handle, const AssetHandling::MeshAsset& mesh,
                                    const uint32_t revision) {
        OpenGLMesh m = {};
        m.numVertices = mesh.vertices.size();
        m.numIndices = mesh.indices.size();

        if (!mesh.IsValid()) {
            m_meshes[handle] = m;
            return;
        }

        glGenVertexArrays(1, &m.VAO);
        glGenBuffers(1, &m.VBO);
        glGenBuffers(1, &m.EBO);

        glBindVertexArray(m.VAO);

        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
        const auto size = static_cast<GLsizeiptr>(m.numVertices * sizeof(AssetHandling::MeshVertexAsset));
        glBufferData(GL_ARRAY_BUFFER, size, mesh.vertices.data(), GL_STATIC_DRAW);

        // Bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(sizeof(uint32_t) * m.numIndices),
                     mesh.indices.data(),
                     GL_STATIC_DRAW
                );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(AssetHandling::MeshVertexAsset),
                              reinterpret_cast<void*>(offsetof(AssetHandling::MeshVertexAsset, position))
                );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(AssetHandling::MeshVertexAsset),
                              reinterpret_cast<void*>(offsetof(AssetHandling::MeshVertexAsset, normal))
                );

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(AssetHandling::MeshVertexAsset),
                              reinterpret_cast<void*>(offsetof(AssetHandling::MeshVertexAsset, uv))
                );

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        m_meshes[handle] = m;
        m_mesh_revisions[handle] = revision;
    }

    OpenGLMesh &OpenGlMeshLibrary::GetMesh(const Assets::MeshHandle& handle) {
        return m_meshes[handle];
    }

    uint32_t OpenGlMeshLibrary::Size() const {
        return m_meshes.size();
    }


    void OpenGlMeshLibrary::RemoveMesh(const Assets::MeshHandle& handle) {
        m_meshes.erase(handle);
        m_mesh_revisions.erase(handle);
    }

    bool OpenGlMeshLibrary::HasMesh(const Assets::MeshHandle& handle) const {
        return m_meshes.contains(handle);
    }

    uint32_t OpenGlMeshLibrary::GetMeshRevision(const Assets::MeshHandle& handle) const {
        if (const auto it = m_mesh_revisions.find(handle); it != m_mesh_revisions.end()) {
            return it->second;
        }
        throw std::invalid_argument("No mesh handle found in mesh revisions map.");
    }

    void OpenGlMeshLibrary::ClearMeshes() {
        for (auto& val: m_meshes | std::views::values) {
            glDeleteBuffers(1, &val.VBO);
            glDeleteBuffers(1, &val.EBO);
            glDeleteVertexArrays(1, &val.VAO);
        }
        m_meshes.clear();
    }
} // namespace
