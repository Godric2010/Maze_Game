#pragma once
#include <memory>

#include "ICacheManager.hpp"
#include "camera/CameraCache.hpp"
#include "transform/TransformCache.hpp"
#include "ui/UiCache.hpp"

namespace yarep::systems {
    class CacheManager : public ICacheManager {
    public:
        CacheManager() {
            m_transform_cache = std::make_unique<transform::TransformCache>();
            m_camera_cache = std::make_unique<camera::CameraCache>();
            m_ui_cache = std::make_unique<ui::UiCache>();
        }

        ~CacheManager() override = default;

        [[nodiscard]] transform::TransformCache* GetTransformCache() const {
            return m_transform_cache.get();
        }

        [[nodiscard]] camera::CameraCache* GetCameraCache() const {
            return m_camera_cache.get();
        }

        [[nodiscard]] ui::UiCache* GetUiCache() const {
            return m_ui_cache.get();
        }

    private:
        std::unique_ptr<transform::TransformCache> m_transform_cache;
        std::unique_ptr<camera::CameraCache> m_camera_cache;
        std::unique_ptr<ui::UiCache> m_ui_cache;
    };
} // namespace
