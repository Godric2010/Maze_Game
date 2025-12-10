#pragma once
#include <memory>

#include "../src/camera/CameraCache.hpp"
#include "../src/transform/TransformCache.hpp"

namespace Engine::Systems {
    class CacheManager {
    public:
        CacheManager() {
            m_transform_cache = std::make_unique<Transform::TransformCache>();
            m_camera_cache = std::make_unique<Camera::CameraCache>();
        }

        ~CacheManager() = default;

        [[nodiscard]] Transform::TransformCache* GetTransformCache() const {
            return m_transform_cache.get();
        }

        [[nodiscard]] Camera::CameraCache* GetCameraCache() const {
            return m_camera_cache.get();
        }

    private:
        std::unique_ptr<Transform::TransformCache> m_transform_cache;
        std::unique_ptr<Camera::CameraCache> m_camera_cache;
    };
} // namespace
