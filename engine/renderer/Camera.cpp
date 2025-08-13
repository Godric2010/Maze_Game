#include "Camera.hpp"

namespace Engine::Renderer {
    Camera::Camera(const glm::vec3 position, const float width, const float height, const float fov, const float near,
                   const float far) {
        m_position = position;
        m_width = width;
        m_height = height;
        m_fovY = fov;
        m_zNear = near;
        m_zFar = far;
        m_aspect = m_width / m_height;

        m_projectionMatrix = glm::perspective(glm::radians(m_fovY), m_aspect, m_zNear, m_zFar);
        m_viewMatrix = glm::lookAt(m_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        m_modelMatrix = glm::mat4(1.0f);
    }

    Camera::~Camera() = default;

    void Camera::SetPosition(const glm::vec3 &position) {
        m_position = position;
        m_viewMatrix = glm::lookAt(m_position, m_position + glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    }

    glm::vec3 Camera::GetPosition() const {
        return m_position;
    }

    glm::mat4 Camera::GetProjectionMatrix() const {
        return m_projectionMatrix;
    }

    glm::mat4 Camera::GetViewMatrix() const {
        return m_viewMatrix;
    }


} // namespace
