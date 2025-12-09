#pragma once
#include <unordered_map>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Transform.hpp"


namespace Engine::Systems::Transform {
    struct TransformCacheValue {
        glm::vec3 last_position;
        glm::vec3 last_rotation;
        glm::vec3 last_scale;
        uint64_t last_version;
        glm::mat4 transform_matrix;
    };

    class TransformCache {
    public:
        TransformCache();

        ~TransformCache();

        void RegisterEntity(uint64_t entity);

        void DeregisterEntity(uint64_t entity);

        bool IsDirty(uint64_t entity, const Components::Transform* transform);

        void SetValue(uint64_t entity, const Components::Transform* transform,
                      const glm::mat4& transform_mat);

        glm::mat4 GetValue(uint64_t entity);

    private:
        std::unordered_map<uint64_t, TransformCacheValue> m_cache;
        const float m_float_tolerance = 0.00001f;
    };
} // namespace
