//
// Created by Godri on 8/10/2025.
//

#pragma once
#include <memory>

#include "Window.hpp"
#include "renderframework/Renderer.hpp"
#include "shadermanagement/ShaderManager.hpp"
#include "Datatypes.hpp"

namespace Engine::Renderer {
    class RenderController {
    public:
        explicit RenderController(const Environment::WindowContext &window_context);

        ~RenderController();

        MeshHandle RegisterMesh(const MeshAsset &mesh) const;

        void UnregisterMesh(const MeshHandle &handle) const;

        void BeginFrame(const CameraAsset &camera_asset) const;

        void SubmitFrame(DrawAssets &draw_assets) const;

    private:
        Environment::WindowContext m_windowContext;
        std::unique_ptr<RenderFramework::IRenderer> m_renderer;
        std::unique_ptr<ShaderManagement::ShaderManager> m_shaderManager;
    };
}
