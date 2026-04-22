#include "OpenGlBinder.hpp"


namespace Engine::Renderer::RenderFramework::OpenGl {
    OpenGlBinder::OpenGlBinder() = default;

    OpenGlBinder::~OpenGlBinder() = default;

    void OpenGlBinder::BindOpaquePassParameters() {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    void OpenGlBinder::BindUiPassParameters() {
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_NEVER);
        glDisable(GL_CULL_FACE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    ShaderBindings OpenGlBinder::BindShaderFields(GLuint shader) {
        if (std::get<0>(m_bound_shader) != shader) {
            ShaderBindings shader_bindings{};
            shader_bindings.model_bind = glGetUniformLocation(shader, "u_Model");
            shader_bindings.normal_mat_bind = glGetUniformLocation(shader, "u_NormalMatrix");

            shader_bindings.color_bind = glGetUniformLocation(shader, "u_Color");
            shader_bindings.albedo_tex_bind = glGetUniformLocation(shader, "u_Texture");
            shader_bindings.use_texture_bind = glGetUniformLocation(shader, "u_UseTexture");
            shader_bindings.specular_strength_bind = glGetUniformLocation(shader, "u_SpecularStrength");
            shader_bindings.shininess_bind = glGetUniformLocation(shader, "u_Shininess");

            shader_bindings.light_position_bind = glGetUniformLocation(shader, "u_LightPosition");
            shader_bindings.light_color_bind = glGetUniformLocation(shader, "u_LightColor");
            shader_bindings.light_ambient_bind = glGetUniformLocation(shader, "u_AmbientStrength");
            m_bound_shader = std::make_tuple(shader, shader_bindings);
        }
        return std::get<1>(m_bound_shader);
    }

    void OpenGlBinder::BindLight(const ShaderBindings& shader_bindings, glm::vec3 light_pos, glm::vec3 light_color,
                                 const float ambient_strength) {
        if (shader_bindings.light_position_bind == -1 ||
            shader_bindings.light_color_bind == -1 ||
            shader_bindings.light_ambient_bind == -1) {
            return;
        }
        glUniform3fv(shader_bindings.light_position_bind, 1, glm::value_ptr(light_pos));
        glUniform3fv(shader_bindings.light_color_bind, 1, glm::value_ptr(light_color));
        glUniform1f(shader_bindings.light_ambient_bind, ambient_strength);
    }

    void OpenGlBinder::BindColor(const ShaderBindings& shader_bindings, glm::vec4 color) {
        if (shader_bindings.color_bind == -1) {
            return;
        }
        glUniform4fv(shader_bindings.color_bind, 1, glm::value_ptr(color));
    }

    void OpenGlBinder::BindSpecularStrength(const ShaderBindings& shader_bindings, const float specular_strength) {
        if (shader_bindings.specular_strength_bind == -1) {
            return;
        }
        glUniform1f(shader_bindings.specular_strength_bind, specular_strength);
    }

    void OpenGlBinder::BindShininess(const ShaderBindings& shader_bindings, const float shininess) {
        if (shader_bindings.shininess_bind == -1) {
            return;
        }
        glUniform1f(shader_bindings.shininess_bind, shininess);
    }

    void OpenGlBinder::BindAlbedoTexture(const ShaderBindings& shader_bindings, const GLuint texture) {
        if (shader_bindings.use_texture_bind == -1 || shader_bindings.albedo_tex_bind == -1) {
            return;
        }
        if (m_bound_albedo_texture == texture) {
            return;
        }
        if (texture == 0) {
            glUniform1i(shader_bindings.use_texture_bind, GL_FALSE);
        } else {
            glUniform1i(shader_bindings.use_texture_bind, GL_TRUE);
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        m_bound_albedo_texture = texture;
    }

    void OpenGlBinder::BindShader() const {
        glUseProgram(std::get<0>(m_bound_shader));
    }

    GLsizei OpenGlBinder::BindMesh(OpenGLMesh mesh) {
        if (std::get<0>(m_bound_mesh) != mesh.VAO) {
            glBindVertexArray(mesh.VAO);
            auto index_size = static_cast<GLsizei>(mesh.numIndices);
            m_bound_mesh = std::make_tuple(mesh.VAO, index_size);
        }
        return std::get<1>(m_bound_mesh);
    }

    void OpenGlBinder::BindModelMatrix(const ShaderBindings& shader_bindings, glm::mat4 proj_matrix) {
        if (shader_bindings.model_bind == -1) {
            return;
        }
        glUniformMatrix4fv(shader_bindings.model_bind, 1, GL_FALSE, glm::value_ptr(proj_matrix));
    }

    void OpenGlBinder::BindNormalMatrix(const ShaderBindings& shader_bindings, glm::mat3 normal_matrix) {
        if (shader_bindings.normal_mat_bind == -1) {
            return;
        }
        glUniformMatrix3fv(shader_bindings.normal_mat_bind, 1, GL_FALSE, glm::value_ptr(normal_matrix));
    }
} // namespace
