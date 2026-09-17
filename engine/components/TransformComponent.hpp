//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once

#include <Math.hpp>
#include <spdlog/spdlog.h>

namespace yarep::components {
    struct TransformComponent {
    private:
        math::Transform m_transform;
        uint64_t m_version = 0;

    public:
        TransformComponent() {
            m_transform = math::Transform();
            m_version = 0;
        }

        TransformComponent &SetPosition(const math::Vec3& position) {
            m_transform.position = position;
            m_version++;
            return *this;
        }

        TransformComponent &SetRotation(const math::Quaternion& rotation) {
            m_transform.rotation = rotation;
            m_version++;
            return *this;
        }

        TransformComponent &SetScale(const math::Vec3& scale) {
            m_transform.scale = scale;
            m_version++;
            return *this;
        }

        [[nodiscard]] math::Vec3 GetPosition() const { return this->m_transform.position; }
        [[nodiscard]] math::Quaternion GetRotation() const { return this->m_transform.rotation; }
        [[nodiscard]] math::Vec3 GetScale() const { return this->m_transform.scale; }
        [[nodiscard]] math::Transform GetTransform() const { return m_transform; }
        [[nodiscard]] uint64_t GetVersion() const { return this->m_version; }
    };
}
