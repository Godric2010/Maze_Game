//
// Created by Sebastian Borsch on 30.08.25.
//

#pragma once
#include <functional>
#include <vector>
#include <any>
#include "GameWorld.hpp"
#include "IScene.hpp"
#include "../renderer/Datatypes.hpp"

namespace Engine::Core {
    struct Screen_Old {
        float width;
        float height;
        float scale_x;
        float scale_y;
        float aspect;
    };

    class IEngine {
    public:
        virtual ~IEngine() = default;

        virtual void StopExecution() = 0;

        virtual void LoadScene(std::unique_ptr<IScene> scene) = 0;

        virtual Renderer::MeshHandle RegisterMesh(const Renderer::MeshAsset &mesh_asset) = 0;

        [[nodiscard]] virtual GameWorld &GetWorld() const = 0;

        virtual void RegisterForSystemCommands(std::function<void(std::vector<std::any>)> command_callback) = 0;

        virtual void RegisterInputMap(InputMap map) = 0;
    };
}
