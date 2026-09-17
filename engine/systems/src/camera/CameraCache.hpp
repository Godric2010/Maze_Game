#pragma once
#include <cstdint>
#include <unordered_map>

#include "Mat4.hpp"

namespace yarep::systems::camera {
    class CameraCache {
    public:
        struct Element {
            math::Mat4 projection;
            math::Mat4 view;
            uint64_t version;
        };

        CameraCache();

        ~CameraCache();

        void RegisterEntity(uint64_t entity);

        void DeregisterEntity(uint64_t entity);

        void SetCacheValue(uint64_t entity, const math::Mat4& view, const math::Mat4& projection, uint64_t version);

        const Element &GetCacheValue(uint64_t entity);

    private:
        std::unordered_map<uint64_t, Element> m_cache;
    };
} // namespace
