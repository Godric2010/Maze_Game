#pragma once
#include <unordered_map>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Engine::Systems::Camera {
    class CameraCache {
    public:
        struct Element {
            glm::mat4 projection;
            glm::mat4 view;
            uint64_t version;
        };

        CameraCache();

        ~CameraCache();

        void RegisterEntity(uint64_t entity);

        void DeregisterEntity(uint64_t entity);

        void SetCacheValue(uint64_t entity, const glm::mat4& view, const glm::mat4& projection, uint64_t version);

        const Element &GetCacheValue(uint64_t entity);

    private:
        std::unordered_map<uint64_t, Element> m_cache;
    };
} // namespace
