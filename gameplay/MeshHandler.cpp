#include "MeshHandler.hpp"

namespace Gameplay {
    MeshHandler::MeshHandler(Engine::Core::IEngine &engine) {
        CreateWallMesh(engine);
        CreateFloorMesh(engine);
        CreateKeyMesh(engine);
    }

    MeshHandler::~MeshHandler() = default;

    Engine::Renderer::MeshHandle MeshHandler::GetFloorMesh() const {
        return m_floor_mesh_handle;
    }

    Engine::Renderer::MeshHandle MeshHandler::GetWallMesh() const {
        return m_wall_mesh_handle;
    }

    Engine::Renderer::MeshHandle MeshHandler::GetKeyMesh() const {
        return m_key_mesh_handle;
    }

    void MeshHandler::CreateFloorMesh(Engine::Core::IEngine &engine) {
        auto quad_mesh = Engine::Renderer::MeshAsset{};
        quad_mesh.vertices = std::vector<glm::vec3>();
        quad_mesh.vertices.emplace_back(-0.5, 0.0, -0.5);
        quad_mesh.vertices.emplace_back(0.5, 0.0, -0.5);
        quad_mesh.vertices.emplace_back(0.5, 0.0, 0.5);
        quad_mesh.vertices.emplace_back(-0.5, 0.0, 0.5);
        quad_mesh.indices = std::vector<unsigned int>();
        quad_mesh.indices.push_back(0);
        quad_mesh.indices.push_back(1);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(3);
        quad_mesh.indices.push_back(0);
        m_floor_mesh_handle = engine.RegisterMesh(quad_mesh);
    }

    void MeshHandler::CreateWallMesh(Engine::Core::IEngine &engine) {
        auto quad_mesh = Engine::Renderer::MeshAsset{};
        quad_mesh.vertices = std::vector<glm::vec3>();
        quad_mesh.vertices.emplace_back(-0.5, -0.5, 0.0);
        quad_mesh.vertices.emplace_back(0.5, -0.5, 0.0);
        quad_mesh.vertices.emplace_back(0.5, 0.5, 0.0);
        quad_mesh.vertices.emplace_back(-0.5, 0.5, 0.0);
        quad_mesh.indices = std::vector<unsigned int>();
        quad_mesh.indices.push_back(0);
        quad_mesh.indices.push_back(1);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(3);
        quad_mesh.indices.push_back(0);
        m_wall_mesh_handle = engine.RegisterMesh(quad_mesh);
    }

    void MeshHandler::CreateKeyMesh(Engine::Core::IEngine &engine) {
        auto cube_mesh = Engine::Renderer::MeshAsset{};
        cube_mesh.vertices = std::vector<glm::vec3>();
        cube_mesh.indices = std::vector<unsigned int>();

        // front face
        cube_mesh.vertices.emplace_back(-0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, -0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, -0.5);
        cube_mesh.indices.push_back(0);
        cube_mesh.indices.push_back(1);
        cube_mesh.indices.push_back(2);
        cube_mesh.indices.push_back(2);
        cube_mesh.indices.push_back(3);
        cube_mesh.indices.push_back(0);

        // back face
        cube_mesh.vertices.emplace_back(-0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, 0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, 0.5);
        cube_mesh.indices.push_back(4);
        cube_mesh.indices.push_back(5);
        cube_mesh.indices.push_back(6);
        cube_mesh.indices.push_back(6);
        cube_mesh.indices.push_back(7);
        cube_mesh.indices.push_back(4);

        // top face
        cube_mesh.vertices.emplace_back(-0.5, 0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, 0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, -0.5);
        cube_mesh.indices.push_back(8);
        cube_mesh.indices.push_back(9);
        cube_mesh.indices.push_back(10);
        cube_mesh.indices.push_back(10);
        cube_mesh.indices.push_back(11);
        cube_mesh.indices.push_back(8);

        // bottom face
        cube_mesh.vertices.emplace_back(-0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(-0.5, -0.5, 0.5);
        cube_mesh.indices.push_back(12);
        cube_mesh.indices.push_back(13);
        cube_mesh.indices.push_back(14);
        cube_mesh.indices.push_back(14);
        cube_mesh.indices.push_back(15);
        cube_mesh.indices.push_back(12);

        // left face
        cube_mesh.vertices.emplace_back(-0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(-0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, -0.5);
        cube_mesh.vertices.emplace_back(-0.5, 0.5, 0.5);
        cube_mesh.indices.push_back(16);
        cube_mesh.indices.push_back(17);
        cube_mesh.indices.push_back(18);
        cube_mesh.indices.push_back(18);
        cube_mesh.indices.push_back(19);
        cube_mesh.indices.push_back(16);

        // right face
        cube_mesh.vertices.emplace_back(0.5, -0.5, -0.5);
        cube_mesh.vertices.emplace_back(0.5, -0.5, 0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, 0.5);
        cube_mesh.vertices.emplace_back(0.5, 0.5, -0.5);
        cube_mesh.indices.push_back(20);
        cube_mesh.indices.push_back(21);
        cube_mesh.indices.push_back(22);
        cube_mesh.indices.push_back(22);
        cube_mesh.indices.push_back(23);
        cube_mesh.indices.push_back(20);

        m_key_mesh_handle = engine.RegisterMesh(cube_mesh);
    }
} // namespace
