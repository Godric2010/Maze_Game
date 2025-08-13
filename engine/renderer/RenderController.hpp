//
// Created by Godri on 8/10/2025.
//

#pragma once
#include <memory>

#include "Camera.hpp"
#include "Renderable.hpp"
#include "Window.hpp"
#include "meshmanagement/MeshManager.hpp"
#include "renderframework/Renderer.hpp"
#include "shadermanagement/ShaderManager.hpp"

namespace Engine::Renderer {
	class RenderController {
		public:
			explicit RenderController(const Window::WindowContext &windowContext);

			~RenderController();

			void Render() const;

		private:
			Window::WindowContext m_windowContext;
			std::unique_ptr<RenderFramework::IRenderer> m_renderer;
			std::unique_ptr<ShaderManagement::ShaderManager> m_shaderManager;
			std::unique_ptr<Meshmanagement::MeshManager> m_meshManager;
			std::unique_ptr<Camera> m_camera;
			std::vector<Renderable> m_renderables;
	};
}
