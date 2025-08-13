#include "Renderable.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace Engine::Renderer {
    Renderable::Renderable(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec4 color) {
        m_position = position;
        m_rotation = rotation;
        m_scale = scale;
        m_color = color;

        m_modelMatrix = CreateModelMatrix(m_position, m_rotation, m_scale);
    }

    Renderable::~Renderable() = default;

    glm::mat4 Renderable::GetModelMatrix() const {
        return m_modelMatrix;
    }

    glm::vec4 Renderable::GetColor() const {
        return m_color;
    }

    glm::mat4 Renderable::CreateModelMatrix(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale) {
        return glm::translate(glm::mat4(1.0), position) *
               glm::rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0, 0, 1)) *
               glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
               glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
               glm::scale(glm::mat4(1.0), scale);
    }
} // namespace
