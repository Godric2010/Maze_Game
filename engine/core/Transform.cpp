#include "Transform.hpp"

namespace Engine::Core {
    Transform::Transform() {
        m_position = glm::vec3(0.0f);
        m_rotation = glm::vec3(0.0f);
        m_scale = glm::vec3(1.0f);
        m_transform = glm::mat4(1.0f);
    }

    Transform::Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) {
        m_position = position;
        m_rotation = rotation;
        m_scale = scale;
        RecalculateTransform();
    }

    Transform::~Transform() = default;

    void Transform::SetPosition(const glm::vec3 &position) {
        m_position = position;
        RecalculateTransform();
    }

    void Transform::SetRotation(const glm::vec3 &rotation) {
        m_rotation = rotation;
        RecalculateTransform();
    }

    void Transform::SetScale(const glm::vec3 &scale) {
        m_scale = scale;
        RecalculateTransform();
    }

    const glm::vec3 &Transform::GetPosition() const {
        return m_position;
    }

    const glm::vec3 &Transform::GetRotation() const {
        return m_rotation;
    }

    const glm::vec3 &Transform::GetScale() const {
        return m_scale;
    }

    const glm::mat4 &Transform::GetTransform() const {
        return m_transform;
    }

    void Transform::RecalculateTransform() {
        m_transform = glm::translate(glm::mat4(1.0), m_position) *
                      glm::rotate(glm::mat4(1.0), glm::radians(m_rotation.z), glm::vec3(0, 0, 1)) *
                      glm::rotate(glm::mat4(1.0), glm::radians(m_rotation.y), glm::vec3(0, 1, 0)) *
                      glm::rotate(glm::mat4(1.0), glm::radians(m_rotation.x), glm::vec3(1, 0, 0)) *
                      glm::scale(glm::mat4(1.0), m_scale);
    }
} // namespace
