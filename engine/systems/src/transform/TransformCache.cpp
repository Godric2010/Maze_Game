#include "TransformCache.hpp"

#include <string>
#include <Transform.hpp>

namespace Engine::Systems::Transform {
    TransformCache::TransformCache() = default;

    TransformCache::~TransformCache() = default;

    void TransformCache::RegisterEntity(const uint64_t entity) {
        m_cache[entity] = TransformCacheValue();
    }

    void TransformCache::DeregisterEntity(const uint64_t entity) {
        m_cache.erase(entity);
    }

    bool TransformCache::IsDirty(const uint64_t entity, const Components::Transform* transform) {
        if (!m_cache.contains(entity)) {
            throw std::runtime_error("Transform cache does not exist for entity " + std::to_string(entity));
        }

        const auto cache_val = m_cache[entity];
        return cache_val.last_version != transform->GetVersion();
    }

    void TransformCache::SetValue(const uint64_t entity, const Components::Transform* transform,
                                  const glm::mat4& transform_mat) {
        m_cache[entity] = TransformCacheValue{
            .last_position = transform->GetPosition(),
            .last_rotation = transform->GetRotation(),
            .last_scale = transform->GetScale(),
            .last_version = transform->GetVersion(),
            .transform_matrix = transform_mat,
        };
    }

    glm::mat4 TransformCache::GetValue(const uint64_t entity) {
        if (m_cache.contains(entity)) {
            return m_cache[entity].transform_matrix;
        }
        throw std::runtime_error("Transform cache does not exist");
    }
} // namespace
