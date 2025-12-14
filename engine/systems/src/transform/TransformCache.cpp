#include "TransformCache.hpp"

#include <string>
#include <Transform.hpp>

namespace Engine::Systems::Transform {
    TransformCache::TransformCache() = default;

    TransformCache::~TransformCache() = default;

    void TransformCache::RegisterTransformEntity(const uint64_t entity) {
        if (m_transform_cache.contains(entity)) {
            throw std::runtime_error("Entity already exists in Transform cache.");
        }
        m_transform_cache[entity] = TransformCacheValue();
    }

    void TransformCache::RegisterRectTransformEntity(const uint64_t entity) {
        if (m_rect_transform_cache.contains(entity)) {
            throw std::runtime_error("Entity already exists in Rect Transform cache");
        }
        m_rect_transform_cache[entity] = RectTransformCacheValue();
    }

    void TransformCache::DeregisterTransformEntity(const uint64_t entity) {
        if (!m_transform_cache.contains(entity)) {
            throw std::runtime_error("Entity does not exist in Transform cache.");
        }
        m_transform_cache.erase(entity);
    }

    void TransformCache::DeregisterRectTransformEntity(const uint64_t entity) {
        if (!m_rect_transform_cache.contains(entity)) {
            throw std::runtime_error("Entity does not exist in Rect Transform cache.");
        }
        m_rect_transform_cache.erase(entity);
    }

    bool TransformCache::IsDirty(const uint64_t entity, const Components::Transform* transform) {
        if (!m_transform_cache.contains(entity)) {
            throw std::runtime_error("Transform cache does not exist for entity " + std::to_string(entity));
        }

        const auto cache_val = m_transform_cache[entity];
        return cache_val.last_version != transform->GetVersion();
    }

    void TransformCache::SetValue(const uint64_t entity, const Components::Transform* transform,
                                  const glm::mat4& transform_mat) {
        if (!m_transform_cache.contains(entity)) {
            throw std::runtime_error("Entity does not exist in Transform cache.");
        }

        m_transform_cache[entity] = TransformCacheValue{
            .last_position = transform->GetPosition(),
            .last_rotation = transform->GetRotation(),
            .last_scale = transform->GetScale(),
            .last_version = transform->GetVersion(),
            .transform_matrix = transform_mat,
        };
    }

    void TransformCache::SetValue(const uint64_t entity, const RectTransformCacheValue& rect_transform_cache_value) {
        if (!m_rect_transform_cache.contains(entity)) {
            throw std::runtime_error("Entity does not exist in Rect Transform cache.");
        }
        m_rect_transform_cache[entity] = rect_transform_cache_value;
    }

    const TransformCacheValue &TransformCache::GetTransformValue(const uint64_t entity) {
        if (m_transform_cache.contains(entity)) {
            return m_transform_cache[entity];
        }
        throw std::runtime_error("Entity does not exist in Transform cache.");
    }

    const RectTransformCacheValue &TransformCache::GetRectTransformValue(const uint64_t entity) {
        if (m_rect_transform_cache.contains(entity)) {
            return m_rect_transform_cache[entity];
        }
        throw std::runtime_error("Entity does not exist in Rect Transform cache.");
    }
} // namespace
