#include "TransformSystem.hpp"
#include <Transform.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Engine::Systems {
    TransformSystem::TransformSystem() = default;

    TransformSystem::~TransformSystem() = default;

    void TransformSystem::Initialize() {
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::Transform>(
                [this](const Ecs::EntityId entity, const Components::Transform& _) {
                    if (this->Cache()->GetTransformCache() == nullptr) {
                        throw std::runtime_error("TransformSystem: Transform cache is null");
                    }
                    this->Cache()->GetTransformCache()->RegisterTransformEntity(entity);
                }
                );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::Transform>(
                [this](const Ecs::EntityId entity) {
                    this->Cache()->GetTransformCache()->DeregisterTransformEntity(entity);
                }
                );
    }

    void TransformSystem::Run(float delta_time) {
        const auto transform_components = EcsWorld()->GetComponentsOfType<Components::Transform>();
        for (const auto [transform, entity]: transform_components) {
            if (!Cache()->GetTransformCache()->IsDirty(entity, transform)) {
                continue;
            }
            auto matrix = CalculateModelMatrix(transform->GetPosition(),
                                               transform->GetRotation(),
                                               transform->GetScale()
                    );
            Cache()->GetTransformCache()->SetValue(entity, transform, matrix);
        }
    }

    glm::mat4 TransformSystem::CalculateModelMatrix(const glm::vec3 position, const glm::vec3 rotation,
                                                    const glm::vec3 scale) {
        const auto matrix = translate(glm::mat4(1.0), position) *
                            rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0, 0, 1)) *
                            rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
                            rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
                            glm::scale(glm::mat4(1.0), scale);
        return matrix;
    }
} // namespace
