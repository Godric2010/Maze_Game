//
// Created by Sebastian Borsch on 28.10.25.
//

#pragma once
#include <glm/vec2.hpp>

namespace Engine::Core::Systems {
    class TransformSystem;
}

namespace Engine::Core::Components::UI {
    struct RectTransform {
    private:
        friend Systems::TransformSystem;

    public:
        RectTransform() {
            m_position = glm::vec2(0.0f);
            m_size = glm::vec2(0.0f);
            m_pivot = glm::vec2(0.0f);
            m_rotation = 0.0f;
            m_layer_z = 0.0f;
        }

        RectTransform(const glm::vec2 position, const glm::vec2 size) {
            m_position = position;
            m_size = size;
            m_pivot = glm::vec2(0.0f);
            m_rotation = 0.0f;
            m_layer_z = 0.0f;
        }

        RectTransform(const glm::vec2 position, const glm::vec2 size, const float layer) {
            m_position = position;
            m_size = size;
            m_pivot = glm::vec2(0.0f);
            m_rotation = 0.0f;
            m_layer_z = layer;
        }

        RectTransform(const glm::vec2 position, const glm::vec2 size, const glm::vec2 pivot, const float rotation,
                      const float layer) {
            m_position = position;
            m_size = size;
            m_pivot = pivot;
            m_rotation = rotation;
            m_layer_z = layer;
        }

        [[nodiscard]] glm::vec2 Position() const { return m_position; }
        [[nodiscard]] glm::vec2 Size() const { return m_size; }
        [[nodiscard]] glm::vec2 Pivot() const { return m_pivot; }
        [[nodiscard]] float Rotation() const { return m_rotation; }
        [[nodiscard]] float LayerZ() const { return m_layer_z; }
        [[nodiscard]] glm::mat4 GetMatrix() const { return m_matrix; }

    private:
        glm::mat4 m_matrix{};
        glm::vec2 m_position{};
        glm::vec2 m_size{};
        glm::vec2 m_pivot{};
        float m_rotation;
        float m_layer_z;
    };
}
