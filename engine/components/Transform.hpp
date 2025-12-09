//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once

#include <glm/glm.hpp>


namespace Engine::Components {
    struct Transform {
    private:
        glm::vec3 m_position{};
        glm::vec3 m_rotation{};
        glm::vec3 m_scale{};
        uint64_t m_version = 0;

    public:
        Transform() {
            m_position = glm::vec3(0.0f);
            m_rotation = glm::vec3(0.0f);
            m_scale = glm::vec3(1.0f);
            m_version = 0;
        }

        Transform &SetPosition(const glm::vec3& position) {
            m_position = position;
            m_version++;
            return *this;
        }

        Transform &SetRotation(const glm::vec3& rotation) {
            m_rotation = rotation;
            m_version++;
            return *this;
        }

        Transform &SetScale(const glm::vec3& scale) {
            m_scale = scale;
            m_version++;
            return *this;
        }

        [[nodiscard]] glm::vec3 GetPosition() const { return this->m_position; }
        [[nodiscard]] glm::vec3 GetRotation() const { return this->m_rotation; }
        [[nodiscard]] glm::vec3 GetScale() const { return this->m_scale; }
        uint64_t GetVersion() const { return this->m_version; }
    };
}
