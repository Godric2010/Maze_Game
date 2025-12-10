//
// Created by Sebastian Borsch on 25.08.25.
//

#pragma once
#include <glm/glm.hpp>

namespace Engine::Components {
    struct Camera {
        Camera() {
            m_width = 0;
            m_height = 0;
            m_field_of_view = 0;
            m_aspect_ratio = 0;
            m_near_clip = 0;
            m_far_clip = 0;
            m_version = 0;
        }

        Camera &SetWidth(const float width) {
            m_width = width;
            m_version++;
            return *this;
        }

        Camera &SetHeight(const float height) {
            m_height = height;
            m_version++;
            return *this;
        }

        Camera &SetFieldOfView(const float field_of_view) {
            m_field_of_view = field_of_view;
            m_version++;
            return *this;
        }

        Camera &SetAspectRatio(const float aspect_ratio) {
            m_aspect_ratio = aspect_ratio;
            m_version++;
            return *this;
        }

        Camera &SetNearClip(const float near_clip) {
            m_near_clip = near_clip;
            m_version++;
            return *this;
        }

        Camera &SetFarClip(const float far_clip) {
            m_far_clip = far_clip;
            m_version++;
            return *this;
        }

        [[nodiscard]] float GetWidth() const {
            return m_width;
        }

        [[nodiscard]] float GetHeight() const {
            return m_height;
        }

        [[nodiscard]] float GetFieldOfView() const {
            return m_field_of_view;
        }

        [[nodiscard]] float GetAspectRatio() const {
            return m_aspect_ratio;
        }

        [[nodiscard]] float GetNearClip() const {
            return m_near_clip;
        }

        [[nodiscard]] float GetFarClip() const {
            return m_far_clip;
        }

        [[nodiscard]] uint64_t GetVersion() const {
            return m_version;
        }

    private:
        float m_width;
        float m_height;
        float m_field_of_view;
        float m_aspect_ratio;
        float m_near_clip;
        float m_far_clip;
        uint64_t m_version;
    };
}
