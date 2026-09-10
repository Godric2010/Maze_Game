#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
    #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include <cmath>

#include <QuaternionMath.hpp>
#include "../include/Bounds.hpp"

using namespace yarep::geometry;
using namespace yarep::math;

namespace {
    constexpr float TEST_EPSILON = 0.0001f;

    bool test_near(
            const float lhs,
            const float rhs,
            const float epsilon = TEST_EPSILON) {
        return std::abs(lhs - rhs) <= epsilon;
    }

    void require_vec3_near(
            const Vec3& actual,
            const Vec3& expected,
            const float epsilon = TEST_EPSILON) {
        REQUIRE(test_near(actual.x, expected.x, epsilon));
        REQUIRE(test_near(actual.y, expected.y, epsilon));
        REQUIRE(test_near(actual.z, expected.z, epsilon));
    }

    void require_aabb_near(
            const AABB& actual,
            const AABB& expected,
            const float epsilon = TEST_EPSILON) {
        require_vec3_near(
                actual.min,
                expected.min,
                epsilon
                );

        require_vec3_near(
                actual.max,
                expected.max,
                epsilon
                );
    }

    void require_obb_near(
            const OBB& actual,
            const OBB& expected,
            const float epsilon = TEST_EPSILON) {
        require_vec3_near(
                actual.center,
                expected.center,
                epsilon
                );

        require_vec3_near(
                actual.half_extents,
                expected.half_extents,
                epsilon
                );

        REQUIRE(
                nearly_equal(
                    actual.rotation,
                    expected.rotation,
                    epsilon)
                );
    }
}


// ============================================================================
// Sphere -> AABB
// ============================================================================

TEST_CASE("Sphere -> AABB - Sphere converts to centered AABB") {
    constexpr Sphere sphere{
        Vec3{0, 0, 0},
        5.0f
    };

    const AABB result =
            to_aabb(sphere);

    require_aabb_near(
            result,
            AABB{
                Vec3{-5, -5, -5},
                Vec3{5, 5, 5}
            }
            );
}


TEST_CASE("Sphere -> AABB - Offset sphere converts to AABB") {
    constexpr Sphere sphere{
        Vec3{10, -4, 7},
        3.0f
    };

    const AABB result =
            to_aabb(sphere);

    require_aabb_near(
            result,
            AABB{
                Vec3{7, -7, 4},
                Vec3{13, -1, 10}
            }
            );
}


TEST_CASE("Sphere -> AABB - Zero radius sphere converts to point AABB") {
    constexpr Sphere sphere{
        Vec3{2, -4, 7},
        0.0f
    };

    const AABB result =
            to_aabb(sphere);

    require_aabb_near(
            result,
            AABB{
                Vec3{2, -4, 7},
                Vec3{2, -4, 7}
            }
            );
}


// ============================================================================
// OBB -> AABB
// ============================================================================

TEST_CASE("OBB -> AABB - Identity OBB converts to matching AABB") {
    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    const AABB result =
            to_aabb(bounds);

    require_aabb_near(
            result,
            AABB{
                Vec3{-2, -3, -4},
                Vec3{2, 3, 4}
            }
            );
}


TEST_CASE("OBB -> AABB - Offset identity OBB converts to AABB") {
    constexpr OBB bounds{
        Vec3{10, -5, 3},
        Vec3{2, 3, 4},
        Quaternion::identity()
    };

    const AABB result =
            to_aabb(bounds);

    require_aabb_near(
            result,
            AABB{
                Vec3{8, -8, -1},
                Vec3{12, -2, 7}
            }
            );
}


TEST_CASE("OBB -> AABB - OBB rotated ninety degrees around Y converts to tight AABB") {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 3, 4},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                )
    };

    const AABB result =
            to_aabb(bounds);

    // X and Z extents swap after a 90 degree Y rotation.

    require_aabb_near(
            result,
            AABB{
                Vec3{-4, -3, -2},
                Vec3{4, 3, 2}
            }
            );
}


TEST_CASE("OBB -> AABB - OBB rotated forty five degrees around Z converts to tight AABB") {
    const OBB bounds{
        Vec3{0, 0, 0},
        Vec3{2, 1, 3},

        from_axis_angle(
                Vec3{0, 0, 1},
                Angle::from_degrees(45)
                )
    };

    const AABB result =
            to_aabb(bounds);

    const float xy_extent =
            3.0f / std::sqrt(2.0f);

    require_aabb_near(
            result,
            AABB{
                Vec3{-xy_extent, -xy_extent, -3},
                Vec3{xy_extent, xy_extent, 3}
            }
            );
}


TEST_CASE("OBB -> AABB - Zero sized OBB converts to point AABB") {
    const OBB bounds{
        Vec3{4, -2, 7},
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(73)
                )
    };

    const AABB result =
            to_aabb(bounds);

    require_aabb_near(
            result,
            AABB{
                Vec3{4, -2, 7},
                Vec3{4, -2, 7}
            }
            );
}


// ============================================================================
// AABB + Transform -> OBB
// ============================================================================

TEST_CASE("AABB + Transform -> OBB - Identity transform converts AABB to matching OBB") {
    constexpr AABB local_bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    const OBB result =
            to_obb(
                    local_bounds,
                    Transform::identity()
                    );

    require_obb_near(
            result,
            OBB{
                Vec3{0, 0, 0},
                Vec3{2, 3, 4},
                Quaternion::identity()
            }
            );
}


TEST_CASE("AABB + Transform -> OBB - AABB converts to OBB using local center and half extents") {
    constexpr AABB local_bounds{
        Vec3{2, 4, 6},
        Vec3{6, 10, 14}
    };

    const OBB result =
            to_obb(
                    local_bounds,
                    Transform::identity()
                    );

    require_obb_near(
            result,
            OBB{
                Vec3{4, 7, 10},
                Vec3{2, 3, 4},
                Quaternion::identity()
            }
            );
}


TEST_CASE("AABB + Transform -> OBB - Translation moves OBB center") {
    constexpr AABB local_bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Transform transform{
        Vec3{10, -5, 7},
        Quaternion::identity(),
        Vec3{1, 1, 1}
    };

    const OBB result =
            to_obb(
                    local_bounds,
                    transform
                    );

    require_obb_near(
            result,
            OBB{
                Vec3{10, -5, 7},
                Vec3{2, 3, 4},
                Quaternion::identity()
            }
            );
}


TEST_CASE("AABB + Transform -> OBB - Rotation rotates OBB center and orientation") {
    constexpr AABB local_bounds{
        Vec3{0, -1, -1},
        Vec3{4, 1, 1}
    };

    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Transform transform{
        Vec3{0, 0, 0},
        rotation,
        Vec3{1, 1, 1}
    };

    const OBB result =
            to_obb(
                    local_bounds,
                    transform
                    );

    // Local center is (2,0,0).
    // +90 degrees around Y maps +X -> -Z.

    require_obb_near(
            result,
            OBB{
                Vec3{0, 0, -2},
                Vec3{2, 1, 1},
                rotation
            }
            );
}


TEST_CASE("AABB + Transform -> OBB - Non uniform scale changes OBB half extents") {
    constexpr AABB local_bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{2, 3, 4}
    };

    const OBB result =
            to_obb(
                    local_bounds,
                    transform
                    );

    require_obb_near(
            result,
            OBB{
                Vec3{0, 0, 0},
                Vec3{4, 9, 16},
                Quaternion::identity()
            }
            );
}


TEST_CASE("AABB + Transform -> OBB - Negative scale produces positive OBB half extents") {
    constexpr AABB local_bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{-2, -3, -4}
    };

    const OBB result =
            to_obb(
                    local_bounds,
                    transform
                    );

    require_obb_near(
            result,
            OBB{
                Vec3{0, 0, 0},
                Vec3{4, 9, 16},
                Quaternion::identity()
            }
            );
}


TEST_CASE("AABB + Transform -> OBB - Negative scale mirrors local OBB center") {
    constexpr AABB local_bounds{
        Vec3{2, 0, 0},
        Vec3{6, 2, 2}
    };

    constexpr Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{-2, 1, 1}
    };

    const OBB result =
            to_obb(
                    local_bounds,
                    transform
                    );

    // Local center is (4,1,1).
    // Negative X scale mirrors it to (-8,1,1).

    require_obb_near(
            result,
            OBB{
                Vec3{-8, 1, 1},
                Vec3{4, 1, 1},
                Quaternion::identity()
            }
            );
}


TEST_CASE("AABB + Transform -> OBB - Zero scale collapses OBB extents") {
    constexpr AABB local_bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Transform transform{
        Vec3{10, 20, 30},
        Quaternion::identity(),
        Vec3{0, 0, 0}
    };

    const OBB result =
            to_obb(
                    local_bounds,
                    transform
                    );

    require_obb_near(
            result,
            OBB{
                Vec3{10, 20, 30},
                Vec3{0, 0, 0},
                Quaternion::identity()
            }
            );
}


TEST_CASE("AABB + Transform -> OBB - AABB to OBB applies scale rotation translation in TRS order") {
    constexpr AABB local_bounds{
        Vec3{0, -1, -1},
        Vec3{2, 1, 1}
    };

    const Quaternion rotation =
            from_axis_angle(
                    Vec3{0, 1, 0},
                    Angle::from_degrees(90)
                    );

    const Transform transform{
        Vec3{10, 20, 30},
        rotation,
        Vec3{2, 3, 4}
    };

    const OBB result =
            to_obb(
                    local_bounds,
                    transform
                    );

    // Local center:
    // (1,0,0)
    //
    // Scale:
    // (2,0,0)
    //
    // Rotate +90 Y:
    // (0,0,-2)
    //
    // Translate:
    // (10,20,28)

    require_obb_near(
            result,
            OBB{
                Vec3{10, 20, 28},
                Vec3{2, 3, 4},
                rotation
            }
            );
}


// ============================================================================
// transform_bounds
// ============================================================================

TEST_CASE("transform_bounds - Identity transform leaves AABB unchanged") {
    constexpr AABB local_bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    Transform::identity()
                    );

    require_aabb_near(
            result,
            local_bounds
            );
}


TEST_CASE("transform_bounds - Translation moves AABB") {
    constexpr AABB local_bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Transform transform{
        Vec3{10, -5, 7},
        Quaternion::identity(),
        Vec3{1, 1, 1}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    require_aabb_near(
            result,
            AABB{
                Vec3{8, -8, 3},
                Vec3{12, -2, 11}
            }
            );
}


TEST_CASE("transform_bounds - Uniform scale transforms AABB") {
    constexpr AABB local_bounds{
        Vec3{-1, -2, -3},
        Vec3{1, 2, 3}
    };

    constexpr Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{2, 2, 2}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    require_aabb_near(
            result,
            AABB{
                Vec3{-2, -4, -6},
                Vec3{2, 4, 6}
            }
            );
}


TEST_CASE("transform_bounds - Non uniform scale transforms AABB") {
    constexpr AABB local_bounds{
        Vec3{-1, -2, -3},
        Vec3{1, 2, 3}
    };

    constexpr Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{2, 3, 4}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    require_aabb_near(
            result,
            AABB{
                Vec3{-2, -6, -12},
                Vec3{2, 6, 12}
            }
            );
}


TEST_CASE("transform_bounds - Negative scale transforms AABB correctly") {
    constexpr AABB local_bounds{
        Vec3{-1, -2, -3},
        Vec3{1, 2, 3}
    };

    constexpr Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{-2, -3, -4}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    require_aabb_near(
            result,
            AABB{
                Vec3{-2, -6, -12},
                Vec3{2, 6, 12}
            }
            );
}


TEST_CASE("transform_bounds - Negative scale mirrors asymmetric AABB") {
    constexpr AABB local_bounds{
        Vec3{1, 2, 3},
        Vec3{4, 6, 8}
    };

    constexpr Transform transform{
        Vec3{0, 0, 0},
        Quaternion::identity(),
        Vec3{-2, 1, 1}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    require_aabb_near(
            result,
            AABB{
                Vec3{-8, 2, 3},
                Vec3{-2, 6, 8}
            }
            );
}


TEST_CASE("transform_bounds - Zero scale collapses AABB to transform position") {
    constexpr AABB local_bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    constexpr Transform transform{
        Vec3{10, 20, 30},
        Quaternion::identity(),
        Vec3{0, 0, 0}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    require_aabb_near(
            result,
            AABB{
                Vec3{10, 20, 30},
                Vec3{10, 20, 30}
            }
            );
}


TEST_CASE("transform_bounds - Ninety degree Y rotation swaps AABB X and Z extents") {
    constexpr AABB local_bounds{
        Vec3{-2, -3, -4},
        Vec3{2, 3, 4}
    };

    const Transform transform{
        Vec3{0, 0, 0},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{1, 1, 1}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    require_aabb_near(
            result,
            AABB{
                Vec3{-4, -3, -2},
                Vec3{4, 3, 2}
            }
            );
}


TEST_CASE("transform_bounds - Transform bounds applies scale rotation translation") {
    constexpr AABB local_bounds{
        Vec3{-1, -2, -3},
        Vec3{1, 2, 3}
    };

    const Transform transform{
        Vec3{10, 20, 30},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{2, 3, 4}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    // After scale:
    //
    // X extent = 2
    // Y extent = 6
    // Z extent = 12
    //
    // +90 Y swaps X/Z extents.
    //
    // World extents:
    // X = 12
    // Y = 6
    // Z = 2

    require_aabb_near(
            result,
            AABB{
                Vec3{-2, 14, 28},
                Vec3{22, 26, 32}
            }
            );
}


TEST_CASE("transform_bounds - Transform bounds preserves asymmetric local center") {
    constexpr AABB local_bounds{
        Vec3{0, 0, 0},
        Vec3{4, 2, 6}
    };

    const Transform transform{
        Vec3{10, 20, 30},

        from_axis_angle(
                Vec3{0, 1, 0},
                Angle::from_degrees(90)
                ),

        Vec3{1, 1, 1}
    };

    const AABB result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    // Local center = (2,1,3)
    // Half extents  = (2,1,3)
    //
    // Rotated center:
    // (3,1,-2)
    //
    // World center:
    // (13,21,28)
    //
    // Rotated half extents:
    // (3,1,2)

    require_aabb_near(
            result,
            AABB{
                Vec3{10, 20, 26},
                Vec3{16, 22, 30}
            }
            );
}


TEST_CASE("transform_bounds - Transform bounds result matches AABB of transformed OBB") {
    constexpr AABB local_bounds{
        Vec3{-2, -1, -3},
        Vec3{4, 5, 7}
    };

    const Transform transform{
        Vec3{7, -3, 11},

        from_axis_angle(
                Vec3{1, 2, 3},
                Angle::from_degrees(57)
                ),

        Vec3{2, 3, 4}
    };

    const AABB direct_result =
            transform_bounds(
                    local_bounds,
                    transform
                    );

    const OBB world_obb =
            to_obb(
                    local_bounds,
                    transform
                    );

    const AABB via_obb_result =
            to_aabb(world_obb);

    require_aabb_near(
            direct_result,
            via_obb_result
            );
}
