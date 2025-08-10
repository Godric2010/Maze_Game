//
// Created by Godri on 8/10/2025.
//

#pragma once
#include <memory>

#include "Window.hpp"
#include "renderframework/Renderer.hpp"
#include "shadermanagement/ShaderManager.hpp"

namespace Engine::Renderer {
	class RenderController {
		public:
			explicit RenderController(const Window::WindowContext &windowContext);

			~RenderController();

			void Render();

		private:
			Window::WindowContext m_windowContext;
			std::unique_ptr<RenderFramework::IRenderer> m_renderer;
			std::unique_ptr<ShaderManagement::ShaderManager> m_shaderManager;
	};
}
