#include "TransformSystem.hpp"
#include <TransformComponent.hpp>

namespace yarep::systems {
    TransformSystem::TransformSystem() = default;

    TransformSystem::~TransformSystem() = default;

    void TransformSystem::Initialize() {
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::TransformComponent>(
                [this](const ecs::EntityId entity, const components::TransformComponent& _) {
                    if (this->Cache()->GetTransformCache() == nullptr) {
                        throw std::runtime_error("TransformSystem: Transform cache is null");
                    }
                    this->Cache()->GetTransformCache()->RegisterTransformEntity(entity);
                }
                );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<components::TransformComponent>(
                [this](const ecs::EntityId entity) {
                    this->Cache()->GetTransformCache()->DeregisterTransformEntity(entity);
                }
                );
    }

    void TransformSystem::Run(float delta_time) {
        const auto transform_components = EcsWorld()->GetComponentsOfType<components::TransformComponent>();
        for (const auto [transform, entity]: transform_components) {
            if (!Cache()->GetTransformCache()->IsDirty(entity, transform)) {
                continue;
            }
            auto matrix = math::to_matrix(transform->GetTransform());
            Cache()->GetTransformCache()->SetValue(entity, transform, matrix);
        }
    }
} // namespace
