#pragma once
#include <unordered_map>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include "Transform.hpp"


namespace Engine::Systems::Transform {
    struct TransformCacheValue {
        glm::vec3 last_position;
        glm::vec3 last_rotation;
        glm::vec3 last_scale;
        uint64_t last_version;
        glm::mat4 transform_matrix;
    };

    struct RectTransformCacheValue {
        uint64_t last_version;
        glm::vec2 global_position;
        glm::vec2 global_size;
        glm::mat4 global_matrix;
        uint8_t layer;
    };

    class TransformCache {
    public:
        TransformCache();

        ~TransformCache();

        void RegisterTransformEntity(uint64_t entity);

        void RegisterRectTransformEntity(uint64_t entity);

        void DeregisterTransformEntity(uint64_t entity);

        void DeregisterRectTransformEntity(uint64_t entity);

        bool IsDirty(uint64_t entity, const Components::Transform* transform);

        void SetValue(uint64_t entity, const Components::Transform* transform,
                      const glm::mat4& transform_mat);

        void SetValue(uint64_t entity, const RectTransformCacheValue& rect_transform_cache_value);

        const TransformCacheValue &GetTransformValue(uint64_t entity);

        const RectTransformCacheValue &GetRectTransformValue(uint64_t entity);

    private:
        std::unordered_map<uint64_t, TransformCacheValue> m_transform_cache;
        std::unordered_map<uint64_t, RectTransformCacheValue> m_rect_transform_cache;
        const float m_float_tolerance = 0.00001f;
    };
} // namespace
