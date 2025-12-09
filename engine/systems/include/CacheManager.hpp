#pragma once
#include <memory>
#include "../src/transform/TransformCache.hpp"

namespace Engine::Systems {
    class CacheManager {
    public:
        CacheManager() {
            m_transform_cache = std::make_unique<Transform::TransformCache>();
        }

        ~CacheManager() = default;

        Transform::TransformCache* GetTransformCache() const {
            return m_transform_cache.get();
        }

    private:
        std::unique_ptr<Transform::TransformCache> m_transform_cache;
    };
} // namespace
