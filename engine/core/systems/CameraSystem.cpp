#include "CameraSystem.hpp"

namespace Engine::Core::Systems {
	CameraSystem::CameraSystem() = default;

	CameraSystem::~CameraSystem() = default;

	void CameraSystem::SetServices(Ecs::IServiceToEcsProvider* serviceLocator) {
	}

	void CameraSystem::Run(Ecs::World&world, float deltaTime) {
		for (const auto cameras = world.GetComponentsOfType<Components::Camera>(); auto camera: cameras) {
			const auto entity = camera.second;
			const auto cameraComponent = camera.first;
			const auto cameraTransform = world.GetComponent<Components::Transform>(entity);
			CalculateOrientation(cameraComponent, cameraTransform);
		}
	}

	void CameraSystem::CalculateOrientation(Components::Camera* cameraComponent,
	                                        const Components::Transform* transform) {
		cameraComponent->projection = glm::perspective(glm::radians(cameraComponent->FieldOfView),
		                                               cameraComponent->AspectRatio,
		                                               cameraComponent->NearClip,
		                                               cameraComponent->FarClip);

		const auto cam_rotation = transform->getRotation();
		const float pitchRad = glm::radians(cam_rotation->x);
		const float yawRad = glm::radians(cam_rotation->y);
		const float rollRad = glm::radians(cam_rotation->z);

		const glm::mat4 R = glm::yawPitchRoll(yawRad, pitchRad, rollRad);

		constexpr auto localForward = glm::vec3(0, 0, -1);
		constexpr auto localUp = glm::vec3(0, 1, 0);
		const glm::vec3 forward = glm::normalize(R * glm::vec4(localForward, 0.0f));
		const glm::vec3 up = glm::normalize(R * glm::vec4(localUp, 0.0f));

		const glm::vec3 eye = *transform->getPosition();
		const glm::vec3 target = eye + forward;

		cameraComponent->view = glm::lookAt(eye, target, up);
		cameraComponent->model = glm::mat4(1.0f);
	}
} // namespace
