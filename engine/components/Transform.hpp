//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../systems/include/PhysicsSystem.hpp"

namespace Engine::Components {
    struct Transform {
        friend Systems::Physics::PhysicsSystem;

    private:
        glm::vec3 m_position{};
        glm::vec3 m_rotation{};
        glm::vec3 m_scale{};
        glm::mat4 m_matrix{};
        bool m_dirty{};

        void Set(const glm::vec3 pos, const glm::vec3 rot, const glm::vec3 scale) {
            m_position = pos;
            m_rotation = rot;
            m_scale = scale;
            m_matrix = glm::mat4(1.0f);
            m_dirty = true;
        }

    public:
        Transform() {
            m_position = glm::vec3(0.0f);
            m_rotation = glm::vec3(0.0f);
            m_scale = glm::vec3(1.0f);
            m_matrix = glm::mat4(1.0f);
            m_dirty = false;
        }

        Transform(const glm::vec3 position, const glm::vec3 rotation) {
            Set(position, rotation, glm::vec3(1.0f));
        }

        Transform(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale) {
            Set(position, rotation, scale);
        }

        [[nodiscard]] glm::vec3 GetPosition() const { return this->m_position; }
        [[nodiscard]] glm::vec3 GetRotation() const { return this->m_rotation; }
        [[nodiscard]] glm::vec3 GetScale() const { return this->m_scale; }
        [[nodiscard]] glm::mat4 GetMatrix() const { return this->m_matrix; }
        [[nodiscard]] bool IsDirty() const { return this->m_dirty; }
        void SetMatrix(const glm::mat4 &matrix) { this->m_matrix = matrix; }
    };
}
