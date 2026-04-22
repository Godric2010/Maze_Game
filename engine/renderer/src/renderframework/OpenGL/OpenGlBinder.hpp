//
// Created by sebastian on 15.03.26.
//

#pragma once
#include <tuple>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Meshes/OpenGLMesh.hpp"


namespace Engine::Renderer::RenderFramework::OpenGl {
    struct ShaderBindings {
        // Vertex bindings
        GLint model_bind;
        GLint normal_mat_bind;

        // Fragment bindings
        GLint color_bind;
        GLint albedo_tex_bind;
        GLint use_texture_bind;
        GLint specular_strength_bind;
        GLint shininess_bind;

        GLint light_position_bind;
        GLint light_color_bind;
        GLint light_ambient_bind;
    };

    class OpenGlBinder {
    public:
        OpenGlBinder();

        ~OpenGlBinder();

        static void BindOpaquePassParameters();

        static void BindUiPassParameters();

        ShaderBindings BindShaderFields(GLuint shader);

        void BindLight(const ShaderBindings& shader_bindings, glm::vec3 light_pos, glm::vec3 light_color,
                       float ambient_strength);

        void BindColor(const ShaderBindings& shader_bindings, glm::vec4 color);

        void BindSpecularStrength(const ShaderBindings& shader_bindings, float specular_strength);

        void BindShininess(const ShaderBindings& shader_bindings, float shininess);

        void BindAlbedoTexture(const ShaderBindings& shader_bindings, GLuint texture);

        void BindShader() const;

        GLsizei BindMesh(OpenGLMesh mesh);

        void BindModelMatrix(const ShaderBindings& shader_bindings, glm::mat4 proj_matrix);

        void BindNormalMatrix(const ShaderBindings& shader_bindings, glm::mat3 normal_matrix);

    private:
        std::tuple<GLuint, ShaderBindings> m_bound_shader{};
        GLuint m_bound_albedo_texture{};
        std::tuple<unsigned int, GLsizei> m_bound_mesh{};
    };
}
