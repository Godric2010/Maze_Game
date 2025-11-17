#include "OpenGLMeshManager.hpp"

#include <ranges>

namespace Engine::Renderer::RenderFramework::OpenGl {
    OpenGlMeshManager::OpenGlMeshManager() {
        m_mesh_handle = 0;
        m_meshes.clear();
    }

    OpenGlMeshManager::~OpenGlMeshManager() = default;

    MeshHandle OpenGlMeshManager::AddMesh(const MeshAsset &mesh) {
        const MeshHandle handle = m_mesh_handle;
        m_mesh_handle++;

        OpenGLMesh m = {};
        m.numVertices = mesh.vertices.size();
        m.numIndices = mesh.indices.size();

        glGenVertexArrays(1, &m.VAO);
        glGenBuffers(1, &m.VBO);
        glGenBuffers(1, &m.EBO);

        glBindVertexArray(m.VAO);

        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
        const auto size = static_cast<GLsizeiptr>(m.numVertices * sizeof(MeshVertex));
        glBufferData(GL_ARRAY_BUFFER, size, mesh.vertices.data(), GL_STATIC_DRAW);

        // Bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(uint32_t) * m.numIndices),
                     mesh.indices.data(),GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, position)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                              reinterpret_cast<void *>(offsetof(MeshVertex, uv)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        m_meshes[handle] = m;
        return handle;
    }

    OpenGLMesh &OpenGlMeshManager::GetMesh(const MeshHandle &handle) {
        return m_meshes[handle];
    }

    uint32_t OpenGlMeshManager::Size() const {
        return m_meshes.size();
    }


    void OpenGlMeshManager::RemoveMesh(const MeshHandle &handle) {
        m_meshes.erase(handle);
    }

    void OpenGlMeshManager::Clear() {
        for (auto &val: m_meshes | std::views::values) {
            glDeleteBuffers(1, &val.VBO);
            glDeleteBuffers(1, &val.EBO);
            glDeleteVertexArrays(1, &val.VAO);
        }
        m_meshes.clear();
    }
} // namespace
