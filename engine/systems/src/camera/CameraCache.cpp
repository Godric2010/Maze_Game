#include "CameraCache.hpp"
#include <stdexcept>

namespace Engine::Systems::Camera {
    CameraCache::CameraCache() = default;

    CameraCache::~CameraCache() = default;

    void CameraCache::RegisterEntity(const uint64_t entity) {
        if (m_cache.contains(entity)) {
            throw std::runtime_error("Camera Cache register: Entity already exists");
        }
        m_cache.emplace(entity, Element{});
    }

    void CameraCache::DeregisterEntity(const uint64_t entity) {
        if (m_cache.contains(entity)) {
            m_cache.erase(entity);
            return;
        }

        throw std::runtime_error("Camera Cache unregister: Entity does not exists");
    }

    void CameraCache::SetCacheValue(const uint64_t entity, const glm::mat4& view, const glm::mat4& projection,
                                    const uint64_t version) {
        if (!m_cache.contains(entity)) {
            throw std::runtime_error("Camera Cache set value: Entity does not exists");
        }

        m_cache[entity] = Element{projection, view, version};
    }

    const CameraCache::Element &CameraCache::GetCacheValue(const uint64_t entity) {
        if (m_cache.contains(entity)) {
            return m_cache[entity];
        }

        throw std::runtime_error("Camera Cache get value: Entity does not exists");
    }
} // namespace
