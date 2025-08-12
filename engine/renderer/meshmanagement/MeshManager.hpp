#pragma once
#include "Mesh.hpp"

namespace Engine::Renderer::Meshmanagement {
    class MeshManager {
    public:
        MeshManager();

        ~MeshManager();

        void AddMesh(const Mesh& mesh);

    private:
        std::vector<Mesh> m_meshes;
    };
} // namespace