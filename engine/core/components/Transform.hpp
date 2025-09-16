//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once

#include <glm/glm.hpp>

namespace Engine::Core::Components {
    struct Transform {
    private:
        glm::vec3 m_position{};
        glm::vec3 m_rotation{};
        glm::vec3 m_scale{};
        glm::mat4 m_matrix{};

    public:
        Transform() {
            m_position = glm::vec3(0.0f);
            m_rotation = glm::vec3(0.0f);
            m_scale = glm::vec3(1.0f);
            m_matrix = glm::mat4(1.0f);
        }

        Transform(const glm::vec3 position, const glm::vec3 rotation) {
            this->m_position = position;
            this->m_rotation = rotation;
            this->m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
            m_matrix = glm::mat4(1.0f);
        }

        Transform(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale) {
            this->m_position = position;
            this->m_rotation = rotation;
            this->m_scale = scale;
            m_matrix = glm::mat4(1.0f);
        }

        [[nodiscard]] glm::vec3 GetPosition() const { return this->m_position; }
        [[nodiscard]] glm::vec3 GetRotation() const { return this->m_rotation; }
        [[nodiscard]] glm::vec3 GetScale() const { return this->m_scale; }
        [[nodiscard]] glm::mat4 GetMatrix() const { return this->m_matrix; }
        void SetMatrix(const glm::mat4 &matrix) { this->m_matrix = matrix; }
    };
}
