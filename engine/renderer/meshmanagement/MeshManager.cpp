#include "MeshManager.hpp"

namespace Engine::Renderer::Meshmanagement {
    MeshManager::MeshManager() {
        m_meshes = std::vector<Mesh>();
    }

    MeshManager::~MeshManager() {
        m_meshes.clear();
    }

    void MeshManager::AddMesh(const Mesh &mesh) {
        m_meshes.push_back(mesh);
    }



} // namespace