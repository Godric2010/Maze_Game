//
// Created by Sebastian Borsch on 30.08.25.
//

#pragma once
#include "World.hpp"
#include "../renderer/Datatypes.hpp"

namespace Engine::Core {
    class IEngine {
    public:
        virtual ~IEngine() = default;

        virtual Renderer::MeshHandle RegisterMesh(const Renderer::MeshAsset &meshAsset) = 0;

        [[nodiscard]] virtual Ecs::World &GetWorld() const = 0;
    };
}
