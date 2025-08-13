#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Renderer {
    class Renderable {
    public:
        Renderable(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec4 color);

        ~Renderable();

        [[nodiscard]] glm::mat4 GetModelMatrix() const;
        [[nodiscard]] glm::vec4 GetColor() const;

    private:
        glm::vec3 m_position{};
        glm::vec3 m_rotation{};
        glm::vec3 m_scale{};
        glm::mat4 m_modelMatrix{1.0f};
        glm::vec4 m_color{};

        static glm::mat4 CreateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    };
} // namespace