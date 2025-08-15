#include "Camera.hpp"

namespace Engine::Core {
    Camera::Camera(const glm::vec3 position, const float width, const float height, const float fov, const float near,
                   const float far) {
        m_position = position;
        m_width = width;
        m_height = height;
        m_fovY = fov;
        m_zNear = near;
        m_zFar = far;
        m_aspect = m_width / m_height;

        m_front = glm::vec3(0, 0, -1);
        m_right = glm::vec3(1, 0, 0);
        m_up = glm::vec3(0, 1, 0);

        m_yawDeg = -90.0f;
        m_pitchDeg = 0.0f;


        m_projectionMatrix = glm::perspective(glm::radians(m_fovY), m_aspect, m_zNear, m_zFar);
        RecalculateOrientation();
        m_modelMatrix = glm::mat4(1.0f);
    }

    Camera::~Camera() = default;

    void Camera::SetPosition(const glm::vec3 &position) {
        m_position = position;
        RecalculateOrientation();
    }

    void Camera::AddYawPitch(const float yawDeg, const float pitchDeg) {
        m_yawDeg += yawDeg;
        m_pitchDeg += pitchDeg;

        if (m_pitchDeg > 89.0f) m_pitchDeg = 89.0f;
        if (m_pitchDeg < -89.0f) m_pitchDeg = -89.0f;

        RecalculateOrientation();
    }

    void Camera::SetYawPitch(const float yawDeg, const float pitchDeg) {
        m_yawDeg = yawDeg;
        m_pitchDeg = glm::clamp(pitchDeg, -89.0f, 89.0f);
        RecalculateOrientation();
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

    void Camera::RecalculateOrientation() {
        const float yawRad = glm::radians(m_yawDeg);
        const float pitchRad = glm::radians(m_pitchDeg);

        glm::vec3 front;
        front.x = cosf(yawRad) * cosf(pitchRad);
        front.y = sinf(pitchRad);
        front.z = sinf(yawRad) * cosf(pitchRad);
        m_front = glm::normalize(front);

        m_right = glm::normalize(glm::cross(m_front, WORLD_UP));
        m_up = glm::normalize(glm::cross(m_right, m_front));

        m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
    }

} // namespace
