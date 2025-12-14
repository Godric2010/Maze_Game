#pragma once
#include "IEngine.hpp"

namespace Gameplay {
    class MeshHandler {
    public:
        explicit MeshHandler(Engine::Core::IEngine& engine);

        ~MeshHandler();

        [[nodiscard]] Engine::Renderer::MeshHandle GetWallMesh() const;

        [[nodiscard]] Engine::Renderer::MeshHandle GetFloorMesh() const;

        [[nodiscard]] Engine::Renderer::MeshHandle GetKeyMesh() const;

    private:
        Engine::Renderer::MeshHandle m_wall_mesh_handle{};

        Engine::Renderer::MeshHandle m_floor_mesh_handle{};

        Engine::Renderer::MeshHandle m_key_mesh_handle{};

        void CreateWallMesh(Engine::Core::IEngine& engine);

        void CreateFloorMesh(Engine::Core::IEngine& engine);

        void CreateKeyMesh(Engine::Core::IEngine& engine);
    };
} // namespace
