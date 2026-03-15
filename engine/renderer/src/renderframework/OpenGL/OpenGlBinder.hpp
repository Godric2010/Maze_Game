//
// Created by sebastian on 15.03.26.
//

#pragma once
#include <tuple>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLMesh.hpp"


namespace Engine::Renderer::RenderFramework::OpenGl
{
    struct ShaderBindings
    {
        GLint model_bind;
        GLint color_bind;
        GLint albedo_tex_bind;
        GLint use_texture_bind;
    };

    class OpenGlBinder
    {
    public:
        OpenGlBinder();
        ~OpenGlBinder();

        static void BindOpaquePassParameters();
        static void BindUiPassParameters();
        ShaderBindings BindShaderFields(GLuint shader);
        void BindColor(ShaderBindings shader_bindings, glm::vec4 color);
        void BindAlbedoTexture(ShaderBindings shader_bindings, GLuint texture);
        void BindShader() const;
        GLsizei BindMesh(OpenGLMesh mesh);
        void BindMatrix(ShaderBindings shader_bindings, glm::mat4 proj_matrix);

    private:
        std::tuple<GLuint, ShaderBindings> m_bound_shader{};
        GLuint m_bound_albedo_texture{};
        std::tuple<unsigned int, GLsizei> m_bound_mesh{};
    };
}
