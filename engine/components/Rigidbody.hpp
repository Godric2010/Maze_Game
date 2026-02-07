//
// Created by sebastian on 07.02.26.
//

#pragma once
#include <glm/vec3.hpp>

namespace Engine::Components
{
    struct Rigidbody
    {
    private:
        glm::vec3 m_velocity{};
        bool m_velocity_fixed;
        uint64_t m_version;

    public:
        Rigidbody()
        {
            m_version = 0;
            m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            m_velocity_fixed = false;
        }

        Rigidbody& SetVelocity(const glm::vec3& velocity)
        {
            m_velocity = velocity;
            m_version++;
            return *this;
        }
        
        Rigidbody& SetVelocityFixed(const bool velocity_fixed)
        {
            m_velocity_fixed = velocity_fixed;
            return *this;
        }

        [[nodiscard]] glm::vec3 GetVelocity() const { return this->m_velocity; }
        [[nodiscard]] bool IsVelocityFixed() const { return m_velocity_fixed; }
        [[nodiscard]] uint64_t GetVersion() const { return m_version; }
    };
}
