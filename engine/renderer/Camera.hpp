#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Renderer {
    class Camera {
    public:
        Camera(glm::vec3 position, float width, float height, float fov, float near, float far);

        ~Camera();

        void SetPosition(const glm::vec3& position);
        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        glm::vec3 GetPosition() const;

    private:
        glm::vec3 m_position{};
        float m_width;
        float m_height;
        float m_fovY;
        float m_aspect;
        float m_zNear;
        float m_zFar;

        glm::mat4 m_projectionMatrix{};
        glm::mat4 m_viewMatrix{};
        glm::mat4 m_modelMatrix{};
    };
} // namespace