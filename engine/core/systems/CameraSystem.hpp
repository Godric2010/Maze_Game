#pragma once
#include <glm/vec3.hpp>

#include "ISystem.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"

ECS_SYSTEM(CameraSystem, Render, [])
namespace Engine::Core::Systems {
    class CameraSystem : public Ecs::ISystem{
    public:
        CameraSystem();

        ~CameraSystem() override;

        void Run(Ecs::World &world, float deltaTime) override;

    private:
        glm::vec3 m_front{};
        glm::vec3 m_up{};
        glm::vec3 m_right{};
        static constexpr auto WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

        void CalculateOrientation(Components::Camera* cameraComponent, const Components::Transform* transform);
    };
} // namespace