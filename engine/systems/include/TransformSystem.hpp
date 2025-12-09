#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include "ISystem.hpp"
#include "../src/transform/TransformCache.hpp"

ECS_SYSTEM(TransformSystem, LateUpdate, [ENGINE])

namespace Engine::Systems {
    class TransformSystem : public Ecs::IEngineSystem {
    public:
        TransformSystem();

        ~TransformSystem() override;

        void Initialize() override;

        void Run(float delta_time) override;

    private:
        static glm::mat4 CalculateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        Transform::TransformCache* m_transform_cache = nullptr;
    };
} // namespace
