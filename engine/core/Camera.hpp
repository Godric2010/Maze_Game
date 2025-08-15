#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Core {
    class Camera {
    public:
        Camera(glm::vec3 position, float width, float height, float fov, float near, float far);

        ~Camera();

        void SetPosition(const glm::vec3& position);
        void AddYawPitch(float yawDeg, float pitchDeg);
        void SetYawPitch(float yawDeg, float pitchDeg);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        glm::vec3 GetPosition() const;

    private:

        void RecalculateOrientation();

        float m_width;
        float m_height;
        float m_fovY;
        float m_aspect;
        float m_zNear;
        float m_zFar;

        // Orientation and position
        glm::vec3 m_position{};
        float m_yawDeg;
        float m_pitchDeg;

        // Basis vectors
        glm::vec3 m_front;
        glm::vec3 m_right;
        glm::vec3 m_up;
        static constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

        // Matrices
        glm::mat4 m_projectionMatrix{};
        glm::mat4 m_viewMatrix{};
        glm::mat4 m_modelMatrix{};
    };
} // namespace