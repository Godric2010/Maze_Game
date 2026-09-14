#pragma once
#include <limits>

#include "Vec3.hpp"

namespace yarep::geometry {
    struct RaySlabTestInterval {
        float entry = -std::numeric_limits<float>::infinity();
        float exit = std::numeric_limits<float>::infinity();

        math::Vec3 entry_normal{};
        math::Vec3 exit_normal{};
    };


    /**
     * Perform a slab test for a ray and where the ray enters or exists the slab.
     * @param origin The origin of the ray for this slab (axis)
     * @param direction The direction of the ray for this slab (axis)
     * @param slab_min The minimum value of this slab
     * @param slab_max The maximum value of this slab
     * @param min_normal The normal the slab has to at min
     * @param max_normal The normal the slab has at max
     * @param interval The interval that stores the result of this slab test
     * @return Returns true if the slab test found a valid interval, returns false if the ray direction is parallel to and outside of this slab or no common interval remains.
     */
    bool slab_test_interval(const float origin, const float direction, const float slab_min, const float slab_max,
                            const math::Vec3& min_normal, const math::Vec3& max_normal, RaySlabTestInterval& interval);
}
