#include "OpenGlMeshLibrary.hpp"

#include <stdexcept>
#include <ranges>
#include <GL/glew.h>

namespace yarep::renderer::render_framework::open_gl {
    OpenGlMeshLibrary::OpenGlMeshLibrary() {
        m_meshes.clear();
    }

    OpenGlMeshLibrary::~OpenGlMeshLibrary() = default;

    void OpenGlMeshLibrary::AddMesh(const assets::MeshHandle& handle, const asset_handling::MeshAsset& mesh,
                                    const uint32_t revision) {
        OpenGlMesh m = {};
        m.num_vertices = mesh.vertices.size();
        m.num_indices = mesh.indices.size();

        if (!mesh.IsValid()) {
            m_meshes[handle] = m;
            return;
        }

        glGenVertexArrays(1, &m.vao);
        glGenBuffers(1, &m.vbo);
        glGenBuffers(1, &m.ebo);

        glBindVertexArray(m.vao);

        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
        const auto size = static_cast<GLsizeiptr>(m.num_vertices * sizeof(asset_handling::MeshVertexAsset));
        glBufferData(GL_ARRAY_BUFFER, size, mesh.vertices.data(), GL_STATIC_DRAW);

        // Bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(sizeof(uint32_t) * m.num_indices),
                     mesh.indices.data(),
                     GL_STATIC_DRAW
                );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(asset_handling::MeshVertexAsset),
                              reinterpret_cast<void*>(offsetof(asset_handling::MeshVertexAsset, position))
                );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(asset_handling::MeshVertexAsset),
                              reinterpret_cast<void*>(offsetof(asset_handling::MeshVertexAsset, normal))
                );

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(asset_handling::MeshVertexAsset),
                              reinterpret_cast<void*>(offsetof(asset_handling::MeshVertexAsset, uv))
                );

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        m_meshes[handle] = m;
        m_mesh_revisions[handle] = revision;
    }

    OpenGlMesh &OpenGlMeshLibrary::GetMesh(const assets::MeshHandle& handle) {
        return m_meshes[handle];
    }

    uint32_t OpenGlMeshLibrary::Size() const {
        return m_meshes.size();
    }


    void OpenGlMeshLibrary::RemoveMesh(const assets::MeshHandle& handle) {
        m_meshes.erase(handle);
        m_mesh_revisions.erase(handle);
    }

    bool OpenGlMeshLibrary::HasMesh(const assets::MeshHandle& handle) const {
        return m_meshes.contains(handle);
    }

    uint32_t OpenGlMeshLibrary::GetMeshRevision(const assets::MeshHandle& handle) const {
        if (const auto it = m_mesh_revisions.find(handle); it != m_mesh_revisions.end()) {
            return it->second;
        }
        throw std::invalid_argument("No mesh handle found in mesh revisions map.");
    }

    void OpenGlMeshLibrary::ClearMeshes() {
        for (auto& val: m_meshes | std::views::values) {
            glDeleteBuffers(1, &val.vbo);
            glDeleteBuffers(1, &val.ebo);
            glDeleteVertexArrays(1, &val.vao);
        }
        m_meshes.clear();
    }
} // namespace
