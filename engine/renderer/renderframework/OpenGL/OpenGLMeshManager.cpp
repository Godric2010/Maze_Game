#include "OpenGLMeshManager.hpp"

#include <ranges>

namespace Engine::Renderer::RenderFramework::OpenGL {

    OpenGLMeshManager::OpenGLMeshManager() {
        m_meshHandle = 0;
        m_meshes.clear();
    }

    OpenGLMeshManager::~OpenGLMeshManager() = default;

    MeshHandle OpenGLMeshManager::AddMesh(const MeshAsset &mesh) {
        m_meshHandle++;
        OpenGLMesh m = {};
        m.numVertices = mesh.vertices.size();
        m.numIndices = mesh.indices.size();

        glGenVertexArrays(1, &m.VAO);
        glGenBuffers(1, &m.VBO);
        glGenBuffers(1, &m.EBO);

        glBindVertexArray(m.VAO);

        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
        const auto size = static_cast<GLsizeiptr>(m.numVertices * sizeof(glm::vec3));
        glBufferData(GL_ARRAY_BUFFER, size, mesh.vertices.data(), GL_STATIC_DRAW);

        // Bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(uint32_t) * m.numIndices),
                     mesh.indices.data(),GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void *>(0));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        m_meshes[m_meshHandle] = m;
        return m_meshHandle;
    }

    OpenGLMesh &OpenGLMeshManager::GetMesh(const MeshHandle &handle) {
        return m_meshes[handle];
    }

    void OpenGLMeshManager::RemoveMesh(const MeshHandle &handle) {
        m_meshes.erase(handle);
    }

    void OpenGLMeshManager::Clear() {
        for (auto &val: m_meshes | std::views::values) {
            glDeleteBuffers(1, &val.VBO);
            glDeleteBuffers(1, &val.EBO);
            glDeleteVertexArrays(1, &val.VAO);
        }
        m_meshes.clear();
    }






} // namespace