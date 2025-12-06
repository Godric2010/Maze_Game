#include "../../include/TransformSystem.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <Transform.hpp>

namespace Engine::Systems {
    TransformSystem::TransformSystem() = default;

    TransformSystem::~TransformSystem() = default;

    void TransformSystem::Initialize() {
    }

    void TransformSystem::Run(float delta_time) {
        auto transform_components = EcsWorld()->GetComponentsOfType<Components::Transform>();
        for (const auto transform: transform_components | std::views::keys) {
            if (!transform->IsDirty()) {
                continue;
            }
            auto matrix = CalculateModelMatrix(transform->GetPosition(), transform->GetRotation(),
                                               transform->GetScale());
            transform->SetMatrix(matrix);
        }

    }

    glm::mat4 TransformSystem::CalculateModelMatrix(const glm::vec3 position, const glm::vec3 rotation,
                                                    const glm::vec3 scale) {
        const auto matrix = glm::translate(glm::mat4(1.0), position) *
                            rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0, 0, 1)) *
                            rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
                            rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
                            glm::scale(glm::mat4(1.0), scale);
        return matrix;
    }
} // namespace
