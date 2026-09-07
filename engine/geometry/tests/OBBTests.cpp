#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
    #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/OBB.hpp"

using namespace yarep::geometry;
using namespace yarep::math;


// ============================================================================
// construction
// ============================================================================

TEST_CASE("OBB default constructor creates zero sized identity bounds") {
    constexpr OBB bounds{};

    REQUIRE(bounds.center == Vec3{0, 0, 0});
    REQUIRE(bounds.half_extents == Vec3{0, 0, 0});
    REQUIRE(bounds.rotation == Quaternion::identity());
}


TEST_CASE("OBB constructor stores center half extents and rotation") {
    constexpr Quaternion rotation{
        0,
        0.70710678f,
        0,
        0.70710678f
    };

    constexpr OBB bounds{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        rotation
    };

    REQUIRE(bounds.center == Vec3{1, 2, 3});
    REQUIRE(bounds.half_extents == Vec3{4, 5, 6});
    REQUIRE(bounds.rotation == rotation);
}


TEST_CASE("OBB constructor supports negative center coordinates") {
    constexpr OBB bounds{
        Vec3{-10, -20, -30},
        Vec3{1, 2, 3},
        Quaternion::identity()
    };

    REQUIRE(bounds.center == Vec3{-10, -20, -30});
    REQUIRE(bounds.half_extents == Vec3{1, 2, 3});
    REQUIRE(bounds.rotation == Quaternion::identity());
}


TEST_CASE("OBB supports zero half extents") {
    constexpr OBB bounds{
        Vec3{2, -4, 7},
        Vec3{0, 0, 0},
        Quaternion::identity()
    };

    REQUIRE(bounds.center == Vec3{2, -4, 7});
    REQUIRE(bounds.half_extents == Vec3{0, 0, 0});
}


TEST_CASE("OBB supports zero extent on individual axes") {
    constexpr OBB bounds{
        Vec3{1, 2, 3},
        Vec3{0, 4, 0},
        Quaternion::identity()
    };

    REQUIRE(bounds.half_extents == Vec3{0, 4, 0});
}


TEST_CASE("OBB constructor preserves rotation unchanged") {
    constexpr Quaternion rotation{
        1,
        2,
        3,
        4
    };

    constexpr OBB bounds{
        Vec3{0, 0, 0},
        Vec3{1, 1, 1},
        rotation
    };

    REQUIRE(bounds.rotation == rotation);
}


// ============================================================================
// equality
// ============================================================================

TEST_CASE("OBB equality recognizes identical bounds") {
    constexpr OBB lhs{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion{0, 0, 0, 1}
    };

    constexpr OBB rhs{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion{0, 0, 0, 1}
    };

    REQUIRE(lhs == rhs);
}


TEST_CASE("OBB equality rejects different center") {
    constexpr OBB lhs{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion::identity()
    };

    constexpr OBB rhs{
        Vec3{2, 2, 3},
        Vec3{4, 5, 6},
        Quaternion::identity()
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("OBB equality rejects different half extents") {
    constexpr OBB lhs{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion::identity()
    };

    constexpr OBB rhs{
        Vec3{1, 2, 3},
        Vec3{4, 5, 7},
        Quaternion::identity()
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("OBB equality rejects different rotation") {
    constexpr OBB lhs{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion{0, 0, 0, 1}
    };

    constexpr OBB rhs{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion{0, 1, 0, 0}
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("OBB equality compares quaternion components exactly") {
    constexpr Quaternion rotation{
        0,
        0.70710678f,
        0,
        0.70710678f
    };

    constexpr Quaternion negated_rotation{
        -rotation.x,
        -rotation.y,
        -rotation.z,
        -rotation.w
    };

    constexpr OBB lhs{
        Vec3{0, 0, 0},
        Vec3{1, 1, 1},
        rotation
    };

    constexpr OBB rhs{
        Vec3{0, 0, 0},
        Vec3{1, 1, 1},
        negated_rotation
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("OBB inequality recognizes different bounds") {
    constexpr OBB lhs{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion::identity()
    };

    constexpr OBB rhs{
        Vec3{1, 2, 3},
        Vec3{4, 5, 7},
        Quaternion::identity()
    };

    REQUIRE(lhs != rhs);
}


// ============================================================================
// value semantics
// ============================================================================

TEST_CASE("OBB copy preserves values") {
    constexpr OBB original{
        Vec3{-5, 4, 3},
        Vec3{2, 3, 4},
        Quaternion{0, 0.5f, 0, 0.5f}
    };

    constexpr OBB copy = original;

    REQUIRE(copy == original);
}


// ============================================================================
// constexpr contract
// ============================================================================

static_assert(
    OBB{}.center
    == Vec3{0, 0, 0});

static_assert(
    OBB{}.half_extents
    == Vec3{0, 0, 0});

static_assert(
    OBB{}.rotation
    == Quaternion::identity());

static_assert(
    OBB{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion{0, 0, 0, 1}
    }.center
    == Vec3{1, 2, 3});

static_assert(
    OBB{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion{0, 0, 0, 1}
    }.half_extents
    == Vec3{4, 5, 6});

static_assert(
    OBB{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion{0, 0, 0, 1}
    }.rotation
    == Quaternion{0, 0, 0, 1});

static_assert(
    OBB{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion::identity()
    }
    ==
    OBB{
        Vec3{1, 2, 3},
        Vec3{4, 5, 6},
        Quaternion::identity()
    });