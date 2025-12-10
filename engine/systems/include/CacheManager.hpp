#pragma once
#include <memory>

#include "../src/camera/CameraCache.hpp"
#include "../src/transform/TransformCache.hpp"
#include "../src/ui/UiCache.hpp"

namespace Engine::Systems {
    class CacheManager {
    public:
        CacheManager() {
            m_transform_cache = std::make_unique<Transform::TransformCache>();
            m_camera_cache = std::make_unique<Camera::CameraCache>();
            m_ui_cache = std::make_unique<UI::UiCache>();
        }

        ~CacheManager() = default;

        [[nodiscard]] Transform::TransformCache* GetTransformCache() const {
            return m_transform_cache.get();
        }

        [[nodiscard]] Camera::CameraCache* GetCameraCache() const {
            return m_camera_cache.get();
        }

        [[nodiscard]] UI::UiCache* GetUiCache() const {
            return m_ui_cache.get();
        }

    private:
        std::unique_ptr<Transform::TransformCache> m_transform_cache;
        std::unique_ptr<Camera::CameraCache> m_camera_cache;
        std::unique_ptr<UI::UiCache> m_ui_cache;
    };
} // namespace
