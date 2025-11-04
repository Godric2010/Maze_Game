//
// Created by Sebastian Borsch on 30.08.25.
//

#pragma once
#include <functional>
#include <vector>
#include <any>
#include "GameWorld.hpp"
#include "../renderer/Datatypes.hpp"

namespace Engine::Core {
    struct Screen {
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

        virtual Renderer::MeshHandle RegisterMesh(const Renderer::MeshAsset &mesh_asset) = 0;

        [[nodiscard]] virtual GameWorld &GetWorld() const = 0;

        virtual void RegisterForSystemCommands(std::function<void(std::vector<std::any>)> command_callback) = 0;

        virtual void RegisterInputMap(InputMap map) = 0;

        virtual void EnableInputMap(std::string input_map) = 0;

        virtual void DisableInputMap(std::string input_map) = 0;

        virtual Screen GetScreen() = 0;
    };
}
