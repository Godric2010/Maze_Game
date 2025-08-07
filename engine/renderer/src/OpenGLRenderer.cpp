#include "OpenGLRenderer.hpp"

#include <iostream>
#include <ostream>

namespace Engine::Renderer {
    OpenGLRenderer::OpenGLRenderer(Window::WindowContext windowContext) {
    }

    OpenGLRenderer::~OpenGLRenderer() = default;

    void OpenGLRenderer::Initialize() {

        std::cout << "Initializing OpenGLRenderer" << std::endl;
    }

    void OpenGLRenderer::Render() {
        std::cout << "Rendering OpenGLRenderer" << std::endl;
    }

    void OpenGLRenderer::Shutdown() {
        std::cout << "Shutting down OpenGLRenderer" << std::endl;
    }


} // namespace
