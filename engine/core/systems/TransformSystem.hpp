#pragma once
#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include "ISystem.hpp"
#include "ui/RectTransform.hpp"

ECS_SYSTEM(TransformSystem, LateUpdate, [ENGINE])

namespace Engine::Core::Systems {
    class TransformSystem : public Ecs::IEngineSystem {
    public:
        TransformSystem();

        ~TransformSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static glm::mat4 CalculateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    };
} // namespace
