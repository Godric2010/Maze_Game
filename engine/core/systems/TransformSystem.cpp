#include "TransformSystem.hpp"

#include <iostream>
#include <ostream>
#include <glm/ext/matrix_transform.hpp>

#include "components/Transform.hpp"

namespace Engine::Core::Systems {
    TransformSystem::TransformSystem() = default;

    TransformSystem::~TransformSystem() = default;

    void TransformSystem::Run(Ecs::World &world, float deltaTime) {
        std::cout<< "Run transform system" << std::endl;
        auto transformComponents = world.GetComponentsOfType<Components::Transform>();
        for (const auto transform: transformComponents | std::views::keys) {
            transform->Matrix = CalculateMatrix(transform->Position, transform->Rotation, transform->Scale);
        }
    }

    glm::mat4 TransformSystem::CalculateMatrix(const glm::vec3 position, const glm::vec3 rotation,
                                               const glm::vec3 scale) {
        const auto matrix = glm::translate(glm::mat4(1.0), position) *
                            glm::rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0, 0, 1)) *
                            glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
                            glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
                            glm::scale(glm::mat4(1.0), scale);
        return matrix;
    }
} // namespace
