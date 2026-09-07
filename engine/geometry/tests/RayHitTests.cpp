#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
    #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/RayHit.hpp"

using namespace yarep::geometry;
using namespace yarep::math;


// ============================================================================
// construction
// ============================================================================

TEST_CASE("RayHit default constructor creates zero hit") {
    constexpr RayHit hit{};

    REQUIRE(hit.distance == 0.0f);
    REQUIRE(hit.point == Vec3{0, 0, 0});
    REQUIRE(hit.normal == Vec3{0, 0, 0});
}


TEST_CASE("RayHit constructor stores distance point and normal") {
    constexpr RayHit hit{
        5.0f,
        Vec3{1, 2, 3},
        Vec3{0, 1, 0}
    };

    REQUIRE(hit.distance == 5.0f);
    REQUIRE(hit.point == Vec3{1, 2, 3});
    REQUIRE(hit.normal == Vec3{0, 1, 0});
}


TEST_CASE("RayHit constructor supports zero distance") {
    constexpr RayHit hit{
        0.0f,
        Vec3{1, 2, 3},
        Vec3{-1, 0, 0}
    };

    REQUIRE(hit.distance == 0.0f);
    REQUIRE(hit.point == Vec3{1, 2, 3});
    REQUIRE(hit.normal == Vec3{-1, 0, 0});
}


TEST_CASE("RayHit constructor supports negative coordinates") {
    constexpr RayHit hit{
        10.0f,
        Vec3{-5, -4, -3},
        Vec3{0, 0, -1}
    };

    REQUIRE(hit.distance == 10.0f);
    REQUIRE(hit.point == Vec3{-5, -4, -3});
    REQUIRE(hit.normal == Vec3{0, 0, -1});
}


TEST_CASE("RayHit constructor preserves non normalized normal") {
    constexpr RayHit hit{
        3.0f,
        Vec3{1, 2, 3},
        Vec3{0, 5, 0}
    };

    REQUIRE(hit.normal == Vec3{0, 5, 0});
}


// ============================================================================
// equality
// ============================================================================

TEST_CASE("RayHit equality recognizes identical hits") {
    constexpr RayHit lhs{
        5.0f,
        Vec3{1, 2, 3},
        Vec3{0, 1, 0}
    };

    constexpr RayHit rhs{
        5.0f,
        Vec3{1, 2, 3},
        Vec3{0, 1, 0}
    };

    REQUIRE(lhs == rhs);
}


TEST_CASE("RayHit equality rejects different distance") {
    constexpr RayHit lhs{
        5.0f,
        Vec3{1, 2, 3},
        Vec3{0, 1, 0}
    };

    constexpr RayHit rhs{
        6.0f,
        Vec3{1, 2, 3},
        Vec3{0, 1, 0}
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("RayHit equality rejects different point") {
    constexpr RayHit lhs{
        5.0f,
        Vec3{1, 2, 3},
        Vec3{0, 1, 0}
    };

    constexpr RayHit rhs{
        5.0f,
        Vec3{1, 2, 4},
        Vec3{0, 1, 0}
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("RayHit equality rejects different normal") {
    constexpr RayHit lhs{
        5.0f,
        Vec3{1, 2, 3},
        Vec3{0, 1, 0}
    };

    constexpr RayHit rhs{
        5.0f,
        Vec3{1, 2, 3},
        Vec3{1, 0, 0}
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("RayHit inequality recognizes different hits") {
    constexpr RayHit lhs{
        5.0f,
        Vec3{1, 2, 3},
        Vec3{0, 1, 0}
    };

    constexpr RayHit rhs{
        6.0f,
        Vec3{1, 2, 3},
        Vec3{0, 1, 0}
    };

    REQUIRE(lhs != rhs);
}


// ============================================================================
// value semantics
// ============================================================================

TEST_CASE("RayHit copy preserves values") {
    constexpr RayHit original{
        7.0f,
        Vec3{-5, 4, 3},
        Vec3{0, -1, 0}
    };

    constexpr RayHit copy = original;

    REQUIRE(copy == original);
}


// ============================================================================
// constexpr contract
// ============================================================================

static_assert(
        RayHit{}.distance
        == 0.0f
        );

static_assert(
        RayHit{}.point
        == Vec3{0, 0, 0}
        );

static_assert(
        RayHit{}.normal
        == Vec3{0, 0, 0}
        );

static_assert(
        RayHit{
            5.0f,
            Vec3{1, 2, 3},
            Vec3{0, 1, 0}
        }.distance
        == 5.0f
        );

static_assert(
        RayHit{
            5.0f,
            Vec3{1, 2, 3},
            Vec3{0, 1, 0}
        }.point
        == Vec3{1, 2, 3}
        );

static_assert(
        RayHit{
            5.0f,
            Vec3{1, 2, 3},
            Vec3{0, 1, 0}
        }.normal
        == Vec3{0, 1, 0}
        );

static_assert(
        RayHit{
            5.0f,
            Vec3{1, 2, 3},
            Vec3{0, 1, 0}
        }
        ==
        RayHit{
            5.0f,
            Vec3{1, 2, 3},
            Vec3{0, 1, 0}
        }
        );
