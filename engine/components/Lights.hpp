//
// Created by sebastian on 28.04.26.
//

#pragma once
#include <glm/vec3.hpp>

namespace Engine::Components {
    struct PointLight {
    private:
        glm::vec3 m_color{};
        float m_intensity;

        float m_constant;
        float m_linear;
        float m_quadratic;

        bool m_enabled;

    public:
        PointLight() {
            m_color = glm::vec3(0.0f, 0.0f, 0.0f);
            m_intensity = 0.0f;
            m_constant = 0.0f;
            m_linear = 0.0f;
            m_quadratic = 0.0f;
            m_enabled = true;
        }

        PointLight &SetColor(const float r, const float g, const float b) {
            m_color = glm::vec3(r, g, b);
            return *this;
        }

        PointLight &SetIntensity(float intensity) {
            if (intensity > 1.0) {
                intensity = 1.0;
            }
            if (intensity < 0.0) {
                intensity = 0.0;
            }
            m_intensity = intensity;
            return *this;
        }

        PointLight &SetConstant(const float constant) {
            m_constant = constant;
            return *this;
        }

        PointLight &SetLinear(const float linear) {
            m_linear = linear;
            return *this;
        }

        PointLight &SetQuadratic(const float quadratic) {
            m_quadratic = quadratic;
            return *this;
        }

        PointLight &SetEnabled(const bool enabled) {
            m_enabled = enabled;
            return *this;
        }

        [[nodiscard]] glm::vec3 GetColor() const { return m_color; }
        [[nodiscard]] float GetIntensity() const { return m_intensity; }
        [[nodiscard]] float GetConstant() const { return m_constant; }
        [[nodiscard]] float GetLinear() const { return m_linear; }
        [[nodiscard]] float GetQuadratic() const { return m_quadratic; }
        [[nodiscard]] bool GetEnabled() const { return m_enabled; }
    };
}
