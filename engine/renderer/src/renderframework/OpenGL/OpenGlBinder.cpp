#include "OpenGlBinder.hpp"

namespace Engine::Renderer::RenderFramework::OpenGl
{
    OpenGlBinder::OpenGlBinder() = default;

    OpenGlBinder::~OpenGlBinder() = default;

    void OpenGlBinder::BindOpaquePassParameters()
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }

    void OpenGlBinder::BindUiPassParameters()
    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    ShaderBindings OpenGlBinder::BindShader(GLuint shader)
    {
        if (std::get<0>(m_bound_shader) != shader)
        {
            ShaderBindings shader_bindings{};
            shader_bindings.model_bind = glGetUniformLocation(shader, "u_Model");
            shader_bindings.color_bind = glGetUniformLocation(shader, "u_Color");
            shader_bindings.albedo_tex_bind = glGetUniformLocation(shader, "u_Texture");
            shader_bindings.use_texture_bind = glGetUniformLocation(shader, "u_UseTexture");
            m_bound_shader = std::make_tuple(shader, shader_bindings);
        }

        glUseProgram(shader);
        return std::get<1>(m_bound_shader);
    }

    void OpenGlBinder::BindColor(ShaderBindings shader_bindings, glm::vec4 color)
    {
        glUniform4fv(shader_bindings.color_bind, 1, glm::value_ptr(color));
    }

    void OpenGlBinder::BindAlbedoTexture(const ShaderBindings shader_bindings, const GLuint texture)
    {
        if (m_bound_albedo_texture == texture)
        {
            return;
        }

        if (texture == 0)
        {
            glUniform1i(shader_bindings.use_texture_bind, GL_FALSE);
        }
        else
        {
            glUniform1i(shader_bindings.use_texture_bind, GL_TRUE);
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        m_bound_albedo_texture = texture;
    }

    GLsizei OpenGlBinder::BindMesh(OpenGLMesh mesh)
    {
        if (std::get<0>(m_bound_mesh) != mesh.VAO)
        {
            glBindVertexArray(mesh.VAO);
            auto index_size = static_cast<GLsizei>(mesh.numIndices);
            m_bound_mesh = std::make_tuple(mesh.VAO, index_size);
        }
        return std::get<1>(m_bound_mesh);
    }

    void OpenGlBinder::BindMatrix(ShaderBindings shader_bindings, glm::mat4 proj_matrix)
    {
        glUniformMatrix4fv(shader_bindings.model_bind, 1, GL_FALSE, glm::value_ptr(proj_matrix));
    }
} // namespace
