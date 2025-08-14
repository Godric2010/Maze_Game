#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Core {
    class Transform {
    public:
        Transform();

        Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

        ~Transform();

        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::vec3& rotation);
        void SetScale(const glm::vec3& scale);

        const glm::vec3& GetPosition() const;
        const glm::vec3& GetRotation() const;
        const glm::vec3& GetScale() const;

        const glm::mat4& GetTransform() const;

    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        glm::mat4 m_transform;

        void RecalculateTransform();
    };
} // namespace