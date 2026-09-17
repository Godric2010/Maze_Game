//
// Created by sebastian on 07.02.26.
//

#pragma once

namespace yarep::components {
    struct Rigidbody {
    private:
        math::Vec3 m_velocity{};
        bool m_velocity_fixed;
        uint64_t m_version;

    public:
        Rigidbody() {
            m_version = 0;
            m_velocity = math::Vec3(0.0f, 0.0f, 0.0f);
            m_velocity_fixed = false;
        }

        Rigidbody &SetVelocity(const math::Vec3& velocity) {
            m_velocity = velocity;
            m_version++;
            return *this;
        }

        Rigidbody &SetVelocityFixed(const bool velocity_fixed) {
            m_velocity_fixed = velocity_fixed;
            return *this;
        }

        [[nodiscard]] math::Vec3 GetVelocity() const { return this->m_velocity; }
        [[nodiscard]] bool IsVelocityFixed() const { return m_velocity_fixed; }
        [[nodiscard]] uint64_t GetVersion() const { return m_version; }
    };
}
