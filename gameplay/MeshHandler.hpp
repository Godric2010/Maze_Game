#pragma once
#include "IApplication.hpp"
#include "Renderer/Types.hpp"

namespace Gameplay {
    class MeshHandler {
    public:
        explicit MeshHandler(Engine::IApplication& engine);

        ~MeshHandler();

        [[nodiscard]] Engine::Renderer::MeshHandle GetWallMesh() const;

        [[nodiscard]] Engine::Renderer::MeshHandle GetFloorMesh() const;

        [[nodiscard]] Engine::Renderer::MeshHandle GetKeyMesh() const;

    private:
        Engine::Renderer::MeshHandle m_wall_mesh_handle{};

        Engine::Renderer::MeshHandle m_floor_mesh_handle{};

        Engine::Renderer::MeshHandle m_key_mesh_handle{};

        void CreateWallMesh(Engine::IApplication& engine);

        void CreateFloorMesh(Engine::IApplication& engine);

        void CreateKeyMesh(Engine::IApplication& engine);
    };
} // namespace
