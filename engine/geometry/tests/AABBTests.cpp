#if __APPLE__
    #include <catch2/catch_test_macros.hpp>
    #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/AABB.hpp"

using namespace yarep::geometry;
using namespace yarep::math;


// ============================================================================
// construction
// ============================================================================

TEST_CASE("AABB default constructor creates zero sized bounds") {
    constexpr AABB bounds{};

    REQUIRE(bounds.min == Vec3{0, 0, 0});
    REQUIRE(bounds.max == Vec3{0, 0, 0});
}


TEST_CASE("AABB constructor stores minimum and maximum") {
    constexpr AABB bounds{
        Vec3{-1, -2, -3},
        Vec3{4, 5, 6}
    };

    REQUIRE(bounds.min == Vec3{-1, -2, -3});
    REQUIRE(bounds.max == Vec3{4, 5, 6});
}


TEST_CASE("AABB constructor supports negative coordinates") {
    constexpr AABB bounds{
        Vec3{-10, -20, -30},
        Vec3{-1, -2, -3}
    };

    REQUIRE(bounds.min == Vec3{-10, -20, -30});
    REQUIRE(bounds.max == Vec3{-1, -2, -3});
}


TEST_CASE("AABB supports zero sized bounds") {
    constexpr Vec3 point{
        2,
        -4,
        7
    };

    constexpr AABB bounds{
        point,
        point
    };

    REQUIRE(bounds.min == point);
    REQUIRE(bounds.max == point);
}


// ============================================================================
// equality
// ============================================================================

TEST_CASE("AABB equality recognizes identical bounds") {
    constexpr AABB lhs{
        Vec3{-1, -2, -3},
        Vec3{4, 5, 6}
    };

    constexpr AABB rhs{
        Vec3{-1, -2, -3},
        Vec3{4, 5, 6}
    };

    REQUIRE(lhs == rhs);
}


TEST_CASE("AABB equality rejects different minimum") {
    constexpr AABB lhs{
        Vec3{-1, -2, -3},
        Vec3{4, 5, 6}
    };

    constexpr AABB rhs{
        Vec3{-2, -2, -3},
        Vec3{4, 5, 6}
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("AABB equality rejects different maximum") {
    constexpr AABB lhs{
        Vec3{-1, -2, -3},
        Vec3{4, 5, 6}
    };

    constexpr AABB rhs{
        Vec3{-1, -2, -3},
        Vec3{4, 5, 7}
    };

    REQUIRE_FALSE(lhs == rhs);
}


TEST_CASE("AABB inequality recognizes different bounds") {
    constexpr AABB lhs{
        Vec3{-1, -2, -3},
        Vec3{4, 5, 6}
    };

    constexpr AABB rhs{
        Vec3{-1, -2, -3},
        Vec3{4, 5, 7}
    };

    REQUIRE(lhs != rhs);
}


// ============================================================================
// value semantics
// ============================================================================

TEST_CASE("AABB copy preserves values") {
    constexpr AABB original{
        Vec3{-5, -4, -3},
        Vec3{1, 2, 3}
    };

    constexpr AABB copy = original;

    REQUIRE(copy == original);
}


// ============================================================================
// constexpr contract
// ============================================================================

static_assert(
        AABB{}.min
        == Vec3{0, 0, 0}
        );

static_assert(
        AABB{}.max
        == Vec3{0, 0, 0}
        );

static_assert(
        AABB{
            Vec3{-1, -2, -3},
            Vec3{4, 5, 6}
        }.min
        == Vec3{-1, -2, -3}
        );

static_assert(
        AABB{
            Vec3{-1, -2, -3},
            Vec3{4, 5, 6}
        }.max
        == Vec3{4, 5, 6}
        );

static_assert(
        AABB{
            Vec3{-1, -2, -3},
            Vec3{4, 5, 6}
        }
        ==
        AABB{
            Vec3{-1, -2, -3},
            Vec3{4, 5, 6}
        }
        );
