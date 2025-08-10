//
// Created by Godri on 8/10/2025.
//

#pragma once
#include <memory>

#include "Renderer.hpp"
#include "Window.hpp"
#include "shadermanagement/ShaderManager.hpp"

namespace Engine::Renderer {
	class RenderController {
		public:
			RenderController();

			~RenderController();

			void Initialize();

			void Render();

			void Shutdown();

		private:
			Engine::Window::WindowContext m_windowContext;
			std::unique_ptr<IRenderer> m_renderer;
			std::unique_ptr<ShaderManagement::ShaderManager> m_shaderManager;
	};
}
