#include "RayIntersectionHelper.hpp"
#include <algorithm>

namespace yarep::geometry {
    bool slab_test_interval(const float origin, const float direction, const float slab_min,
                            const float slab_max, const math::Vec3& min_normal,
                            const math::Vec3& max_normal,
                            RaySlabTestInterval& interval) {
        // Check if direction is parallel to this slab
        if (direction == 0.0f) {
            // Outside -> can never enter this slab
            if (origin < slab_min || origin > slab_max) {
                return false;
            }

            // Inside -> This axis does not restrict the ray interval but also has infinite solutions. -> No further action required.
            return true;
        }

        auto near_t = (slab_min - origin) / direction;
        auto far_t = (slab_max - origin) / direction;

        auto near_normal = min_normal;
        auto far_normal = max_normal;

        // Ray might travel from max -> min
        if (near_t > far_t) {
            std::swap(near_t, far_t);
            std::swap(near_normal, far_normal);
        }

        // This slab is entered later than the provided interval
        if (near_t > interval.entry) {
            interval.entry = near_t;
            interval.entry_normal = near_normal;
        }

        // This slab is exited earlier than the provided interval
        if (far_t < interval.exit) {
            interval.exit = far_t;
            interval.exit_normal = far_normal;
        }

        return interval.entry <= interval.exit;
    }
}
