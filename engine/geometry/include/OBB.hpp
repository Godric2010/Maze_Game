#pragma once
#include <Quaternion.hpp>
#include <Vec3.hpp>

namespace yarep::geometry {
    // ReSharper disable once CppInconsistentNaming
    /**
  * @struct OBB
  * @brief Represents an oriented bounding box in 3D space.
  *
  * The box is defined by a center position, half-extents along its local axes,
  * and a quaternion describing its orientation.
  *
  * Half-extents are expected to be non-negative.
  * The rotation is stored unchanged and is not guaranteed to be normalized.
  */
    struct OBB {
        math::Vec3 center{};
        math::Vec3 half_extents{};
        math::Quaternion rotation{};

        constexpr OBB() = default;

        constexpr OBB(math::Vec3 center, math::Vec3 half_extents, math::Quaternion rotation) : center(center),
            half_extents(half_extents),
            rotation(rotation) {
        }

        constexpr bool operator==(const OBB& other) const {
            return center == other.center && half_extents == other.half_extents && rotation == other.rotation;
        }
    };
}
