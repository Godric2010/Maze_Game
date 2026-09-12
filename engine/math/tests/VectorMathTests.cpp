#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include <cmath>

#include "../include/VectorMath.hpp"

using namespace yarep::math;

namespace
{
    constexpr float TEST_EPSILON = 0.00001f;

    bool test_near(
        const float lhs,
        const float rhs,
        const float epsilon = TEST_EPSILON)
    {
        return std::abs(lhs - rhs) <= epsilon;
    }
}


// ============================================================================
// Vec2
// ============================================================================

TEST_CASE("Vec2 length squared")
{
    constexpr Vec2 positive{3, 4};
    constexpr Vec2 negative{-3, -4};
    constexpr Vec2 mixed{-3, 4};
    constexpr Vec2 zero{0, 0};

    REQUIRE(length_squared(positive) == 25);
    REQUIRE(length_squared(negative) == 25);
    REQUIRE(length_squared(mixed) == 25);
    REQUIRE(length_squared(zero) == 0);
}

TEST_CASE("Vec2 length")
{
    constexpr Vec2 positive{3, 4};
    constexpr Vec2 negative{-3, -4};
    constexpr Vec2 irrational{1, 1};
    constexpr Vec2 zero{0, 0};

    REQUIRE(test_near(length(positive), 5.0f));
    REQUIRE(test_near(length(negative), 5.0f));
    REQUIRE(test_near(length(irrational), std::sqrt(2.0f)));
    REQUIRE(test_near(length(zero), 0.0f));

    REQUIRE(test_near(length(positive), length(-positive)));
}

TEST_CASE("Vec2 normalize")
{
    constexpr Vec2 positive{3, 4};
    constexpr Vec2 mixed{-3, 4};
    constexpr Vec2 unit{1, 0};
    constexpr Vec2 zero{0, 0};

    const Vec2 normalized_positive = normalize(positive);
    REQUIRE(test_near(normalized_positive.x, 0.6f));
    REQUIRE(test_near(normalized_positive.y, 0.8f));
    REQUIRE(test_near(length(normalized_positive), 1.0f));

    const Vec2 normalized_mixed = normalize(mixed);
    REQUIRE(test_near(normalized_mixed.x, -0.6f));
    REQUIRE(test_near(normalized_mixed.y, 0.8f));
    REQUIRE(test_near(length(normalized_mixed), 1.0f));

    const Vec2 normalized_unit = normalize(unit);
    REQUIRE(test_near(normalized_unit.x, 1.0f));
    REQUIRE(test_near(normalized_unit.y, 0.0f));

    const Vec2 normalized_zero = normalize(zero);
    REQUIRE(normalized_zero == Vec2{0, 0});
}

TEST_CASE("Vec2 dot")
{
    constexpr Vec2 a{1, 2};
    constexpr Vec2 b{3, 4};

    REQUIRE(dot(a, b) == 11);
    REQUIRE(dot(b, a) == 11);

    constexpr Vec2 horizontal{1, 0};
    constexpr Vec2 vertical{0, 1};

    REQUIRE(dot(horizontal, vertical) == 0);

    constexpr Vec2 c{2, -3};
    constexpr Vec2 opposite{-2, 3};

    REQUIRE(dot(c, opposite) == -13);
    REQUIRE(dot(c, c) == length_squared(c));

    constexpr Vec2 zero{0, 0};
    REQUIRE(dot(c, zero) == 0);
}

TEST_CASE("Vec2 distance squared")
{
    constexpr Vec2 a{1, 2};
    constexpr Vec2 b{4, 6};

    REQUIRE(distance_squared(a, b) == 25);
    REQUIRE(distance_squared(b, a) == 25);
    REQUIRE(distance_squared(a, a) == 0);

    constexpr Vec2 negative{-1, -2};
    constexpr Vec2 positive{2, 2};

    REQUIRE(distance_squared(negative, positive) == 25);
}

TEST_CASE("Vec2 distance")
{
    constexpr Vec2 a{1, 2};
    constexpr Vec2 b{4, 6};

    REQUIRE(test_near(distance(a, b), 5.0f));
    REQUIRE(test_near(distance(b, a), 5.0f));
    REQUIRE(test_near(distance(a, a), 0.0f));

    constexpr Vec2 c{0, 0};
    constexpr Vec2 d{1, 1};

    REQUIRE(test_near(distance(c, d), std::sqrt(2.0f)));
}

TEST_CASE("Vec2 lerp")
{
    constexpr Vec2 from{-2, 4};
    constexpr Vec2 to{6, -4};

    REQUIRE(lerp(from, to, 0.0f) == from);
    REQUIRE(lerp(from, to, 1.0f) == to);

    REQUIRE(lerp(from, to, 0.5f) == Vec2{2, 0});
    REQUIRE(lerp(from, to, 0.25f) == Vec2{0, 2});

    // Lerp extrapolates. It does not clamp t.
    REQUIRE(lerp(from, to, -0.5f) == Vec2{-6, 8});
    REQUIRE(lerp(from, to, 1.5f) == Vec2{10, -8});

    REQUIRE(lerp(from, from, 0.5f) == from);
}

TEST_CASE("Vec2 min")
{
    constexpr Vec2 a{1, -4};
    constexpr Vec2 b{-2, 3};

    REQUIRE(yarep::math::min(a, b) == Vec2{-2, -4});
    REQUIRE(yarep::math::min(b, a) == Vec2{-2, -4});

    constexpr Vec2 equal{2, 2};
    REQUIRE(yarep::math::min(equal, equal) == equal);
}

TEST_CASE("Vec2 max")
{
    constexpr Vec2 a{1, -4};
    constexpr Vec2 b{-2, 3};

    REQUIRE(yarep::math::max(a, b) == Vec2{1, 3});
    REQUIRE(yarep::math::max(b, a) == Vec2{1, 3});

    constexpr Vec2 equal{2, 2};
    REQUIRE(yarep::math::max(equal, equal) == equal);
}

TEST_CASE("Vec2 clamp")
{
    constexpr Vec2 minimum{0, -2};
    constexpr Vec2 maximum{4, 2};

    REQUIRE(
        yarep::math::clamp(Vec2{2, 0}, minimum, maximum)
        == Vec2{2,
        0}
    );

    REQUIRE(
        yarep::math::clamp(Vec2{5, -3}, minimum, maximum)
        == Vec2{4,
        -2}
    );

    REQUIRE(
        yarep::math::clamp(Vec2{-5, 8}, minimum, maximum)
        == Vec2{0,
        2}
    );

    REQUIRE(
        yarep::math::clamp(minimum, minimum, maximum)
        == minimum
    );

    REQUIRE(
        yarep::math::clamp(maximum, minimum, maximum)
        == maximum
    );
}

TEST_CASE("Vec2 nearly equal")
{
    constexpr float epsilon = 0.0001f;

    constexpr Vec2 a{1.0f, -2.0f};
    constexpr Vec2 equal{1.0f, -2.0f};
    constexpr Vec2 close{1.00005f, -1.99995f};
    constexpr Vec2 far_x{1.0002f, -2.0f};
    constexpr Vec2 far_y{1.0f, -1.9998f};

    REQUIRE(nearly_equal(a, equal, epsilon));
    REQUIRE(nearly_equal(a, close, epsilon));

    REQUIRE_FALSE(nearly_equal(a, far_x, epsilon));
    REQUIRE_FALSE(nearly_equal(a, far_y, epsilon));

    REQUIRE(nearly_equal(Vec2{0, 0}, Vec2{0, 0}, epsilon));
}


// ============================================================================
// Vec3
// ============================================================================

TEST_CASE("Vec3 length squared")
{
    constexpr Vec3 positive{2, 3, 6};
    constexpr Vec3 negative{-2, -3, -6};
    constexpr Vec3 mixed{-2, 3, -6};
    constexpr Vec3 zero{0, 0, 0};

    REQUIRE(length_squared(positive) == 49);
    REQUIRE(length_squared(negative) == 49);
    REQUIRE(length_squared(mixed) == 49);
    REQUIRE(length_squared(zero) == 0);
}

TEST_CASE("Vec3 length")
{
    constexpr Vec3 positive{2, 3, 6};
    constexpr Vec3 negative{-2, -3, -6};
    constexpr Vec3 irrational{1, 1, 1};
    constexpr Vec3 zero{0, 0, 0};

    REQUIRE(test_near(length(positive), 7.0f));
    REQUIRE(test_near(length(negative), 7.0f));
    REQUIRE(test_near(length(irrational), std::sqrt(3.0f)));
    REQUIRE(test_near(length(zero), 0.0f));

    REQUIRE(test_near(length(positive), length(-positive)));
}

TEST_CASE("Vec3 normalize")
{
    constexpr Vec3 positive{2, 3, 6};
    constexpr Vec3 mixed{-2, 3, -6};
    constexpr Vec3 unit{0, 1, 0};
    constexpr Vec3 zero{0, 0, 0};

    const Vec3 normalized_positive = normalize(positive);
    REQUIRE(test_near(normalized_positive.x, 2.0f / 7.0f));
    REQUIRE(test_near(normalized_positive.y, 3.0f / 7.0f));
    REQUIRE(test_near(normalized_positive.z, 6.0f / 7.0f));
    REQUIRE(test_near(length(normalized_positive), 1.0f));

    const Vec3 normalized_mixed = normalize(mixed);
    REQUIRE(test_near(normalized_mixed.x, -2.0f / 7.0f));
    REQUIRE(test_near(normalized_mixed.y, 3.0f / 7.0f));
    REQUIRE(test_near(normalized_mixed.z, -6.0f / 7.0f));
    REQUIRE(test_near(length(normalized_mixed), 1.0f));

    const Vec3 normalized_unit = normalize(unit);
    REQUIRE(test_near(normalized_unit.x, 0.0f));
    REQUIRE(test_near(normalized_unit.y, 1.0f));
    REQUIRE(test_near(normalized_unit.z, 0.0f));

    REQUIRE(normalize(zero) == Vec3{0, 0, 0});
}

TEST_CASE("Vec3 dot")
{
    constexpr Vec3 a{1, 2, 3};
    constexpr Vec3 b{4, -5, 6};

    REQUIRE(dot(a, b) == 12);
    REQUIRE(dot(b, a) == 12);

    constexpr Vec3 x_axis{1, 0, 0};
    constexpr Vec3 y_axis{0, 1, 0};

    REQUIRE(dot(x_axis, y_axis) == 0);

    constexpr Vec3 c{2, -3, 4};
    constexpr Vec3 opposite{-2, 3, -4};

    REQUIRE(dot(c, opposite) == -29);
    REQUIRE(dot(c, c) == length_squared(c));

    REQUIRE(dot(c, Vec3{0, 0, 0}) == 0);
}

TEST_CASE("Vec3 cross")
{
    constexpr Vec3 x_axis{1, 0, 0};
    constexpr Vec3 y_axis{0, 1, 0};
    constexpr Vec3 z_axis{0, 0, 1};

    REQUIRE(cross(x_axis, y_axis) == z_axis);
    REQUIRE(cross(y_axis, x_axis) == -z_axis);

    REQUIRE(cross(x_axis, x_axis) == Vec3{0, 0, 0});

    constexpr Vec3 a{1, 2, 3};
    constexpr Vec3 b{4, 5, 6};

    Vec3 result = cross(a, b);

    REQUIRE(result == Vec3{-3, 6, -3});

    // Cross product must be orthogonal to both input vectors.
    REQUIRE(dot(result, a) == 0);
    REQUIRE(dot(result, b) == 0);

    // Anti-commutative property.
    REQUIRE(cross(a, b) == -cross(b, a));

    REQUIRE(cross(a, Vec3{0, 0, 0}) == Vec3{0, 0, 0});
}

TEST_CASE("Vec3 distance squared")
{
    constexpr Vec3 a{1, 2, 3};
    constexpr Vec3 b{3, 5, 9};

    REQUIRE(distance_squared(a, b) == 49);
    REQUIRE(distance_squared(b, a) == 49);
    REQUIRE(distance_squared(a, a) == 0);

    constexpr Vec3 negative{-1, -2, -3};
    constexpr Vec3 positive{1, 1, 3};

    REQUIRE(distance_squared(negative, positive) == 49);
}

TEST_CASE("Vec3 distance")
{
    constexpr Vec3 a{1, 2, 3};
    constexpr Vec3 b{3, 5, 9};

    REQUIRE(test_near(distance(a, b), 7.0f));
    REQUIRE(test_near(distance(b, a), 7.0f));
    REQUIRE(test_near(distance(a, a), 0.0f));

    constexpr Vec3 c{0, 0, 0};
    constexpr Vec3 d{1, 1, 1};

    REQUIRE(test_near(distance(c, d), std::sqrt(3.0f)));
}

TEST_CASE("Vec3 lerp")
{
    constexpr Vec3 from{-2, 4, 6};
    constexpr Vec3 to{6, -4, -2};

    REQUIRE(lerp(from, to, 0.0f) == from);
    REQUIRE(lerp(from, to, 1.0f) == to);

    REQUIRE(lerp(from, to, 0.5f) == Vec3{2, 0, 2});
    REQUIRE(lerp(from, to, 0.25f) == Vec3{0, 2, 4});

    REQUIRE(lerp(from, to, -0.5f) == Vec3{-6, 8, 10});
    REQUIRE(lerp(from, to, 1.5f) == Vec3{10, -8, -6});

    REQUIRE(lerp(from, from, 0.5f) == from);
}

TEST_CASE("Vec3 min")
{
    constexpr Vec3 a{1, -4, 8};
    constexpr Vec3 b{-2, 3, 5};

    REQUIRE(yarep::math::min(a, b) == Vec3{-2, -4, 5});
    REQUIRE(yarep::math::min(b, a) == Vec3{-2, -4, 5});

    constexpr Vec3 equal{2, 2, 2};
    REQUIRE(yarep::math::min(equal, equal) == equal);
}

TEST_CASE("Vec3 max")
{
    constexpr Vec3 a{1, -4, 8};
    constexpr Vec3 b{-2, 3, 5};

    REQUIRE(yarep::math::max(a, b) == Vec3{1, 3, 8});
    REQUIRE(yarep::math::max(b, a) == Vec3{1, 3, 8});

    constexpr Vec3 equal{2, 2, 2};
    REQUIRE(yarep::math::max(equal, equal) == equal);
}

TEST_CASE("Vec3 clamp")
{
    constexpr Vec3 minimum{0, -2, 1};
    constexpr Vec3 maximum{4, 2, 3};

    REQUIRE(
        yarep::math::clamp(Vec3{2, 0, 2}, minimum, maximum)
        == Vec3{2,
        0,
        2}
    );

    REQUIRE(
        yarep::math::clamp(Vec3{5, -3, 0.5f}, minimum, maximum)
        == Vec3{4,
        -2,
        1}
    );

    REQUIRE(
        yarep::math::clamp(Vec3{-5, 8, 10}, minimum, maximum)
        == Vec3{0,
        2,
        3}
    );

    REQUIRE(
        yarep::math::clamp(minimum, minimum, maximum)
        == minimum
    );

    REQUIRE(
        yarep::math::clamp(maximum, minimum, maximum)
        == maximum
    );
}

TEST_CASE("Vec3 nearly equal")
{
    constexpr float epsilon = 0.0001f;

    constexpr Vec3 a{1.0f, -2.0f, 3.0f};
    constexpr Vec3 equal{1.0f, -2.0f, 3.0f};
    constexpr Vec3 close{1.00005f, -1.99995f, 3.00005f};

    constexpr Vec3 far_x{1.0002f, -2.0f, 3.0f};
    constexpr Vec3 far_y{1.0f, -1.9998f, 3.0f};
    constexpr Vec3 far_z{1.0f, -2.0f, 3.0002f};

    REQUIRE(nearly_equal(a, equal, epsilon));
    REQUIRE(nearly_equal(a, close, epsilon));

    REQUIRE_FALSE(nearly_equal(a, far_x, epsilon));
    REQUIRE_FALSE(nearly_equal(a, far_y, epsilon));
    REQUIRE_FALSE(nearly_equal(a, far_z, epsilon));

    REQUIRE(
        nearly_equal(
            Vec3{0, 0, 0},
            Vec3{0, 0, 0},
            epsilon)
    );
}


// ============================================================================
// Vec4
// ============================================================================

TEST_CASE("Vec4 length squared")
{
    constexpr Vec4 positive{1, 2, 2, 4};
    constexpr Vec4 negative{-1, -2, -2, -4};
    constexpr Vec4 mixed{-1, 2, -2, 4};
    constexpr Vec4 zero{0, 0, 0, 0};

    REQUIRE(length_squared(positive) == 25);
    REQUIRE(length_squared(negative) == 25);
    REQUIRE(length_squared(mixed) == 25);
    REQUIRE(length_squared(zero) == 0);
}

TEST_CASE("Vec4 length")
{
    constexpr Vec4 positive{1, 2, 2, 4};
    constexpr Vec4 negative{-1, -2, -2, -4};
    constexpr Vec4 irrational{1, 1, 1, 0};
    constexpr Vec4 zero{0, 0, 0, 0};

    REQUIRE(test_near(length(positive), 5.0f));
    REQUIRE(test_near(length(negative), 5.0f));
    REQUIRE(test_near(length(irrational), std::sqrt(3.0f)));
    REQUIRE(test_near(length(zero), 0.0f));

    REQUIRE(test_near(length(positive), length(-positive)));
}

TEST_CASE("Vec4 normalize")
{
    constexpr Vec4 positive{1, 2, 2, 4};
    constexpr Vec4 mixed{-1, 2, -2, 4};
    constexpr Vec4 unit{0, 0, 0, 1};
    constexpr Vec4 zero{0, 0, 0, 0};

    const Vec4 normalized_positive = normalize(positive);
    REQUIRE(test_near(normalized_positive.x, 0.2f));
    REQUIRE(test_near(normalized_positive.y, 0.4f));
    REQUIRE(test_near(normalized_positive.z, 0.4f));
    REQUIRE(test_near(normalized_positive.w, 0.8f));
    REQUIRE(test_near(length(normalized_positive), 1.0f));

    const Vec4 normalized_mixed = normalize(mixed);
    REQUIRE(test_near(normalized_mixed.x, -0.2f));
    REQUIRE(test_near(normalized_mixed.y, 0.4f));
    REQUIRE(test_near(normalized_mixed.z, -0.4f));
    REQUIRE(test_near(normalized_mixed.w, 0.8f));
    REQUIRE(test_near(length(normalized_mixed), 1.0f));

    const Vec4 normalized_unit = normalize(unit);
    REQUIRE(test_near(normalized_unit.x, 0.0f));
    REQUIRE(test_near(normalized_unit.y, 0.0f));
    REQUIRE(test_near(normalized_unit.z, 0.0f));
    REQUIRE(test_near(normalized_unit.w, 1.0f));

    REQUIRE(normalize(zero) == Vec4{0, 0, 0, 0});
}

TEST_CASE("Vec4 dot")
{
    constexpr Vec4 a{1, 2, 3, 4};
    constexpr Vec4 b{4, -5, 6, -7};

    REQUIRE(dot(a, b) == -16);
    REQUIRE(dot(b, a) == -16);

    constexpr Vec4 x_axis{1, 0, 0, 0};
    constexpr Vec4 y_axis{0, 1, 0, 0};

    REQUIRE(dot(x_axis, y_axis) == 0);

    constexpr Vec4 c{1, -2, 3, -4};
    constexpr Vec4 opposite{-1, 2, -3, 4};

    REQUIRE(dot(c, opposite) == -30);
    REQUIRE(dot(c, c) == length_squared(c));

    REQUIRE(dot(c, Vec4{0, 0, 0, 0}) == 0);
}

TEST_CASE("Vec4 distance squared")
{
    constexpr Vec4 a{1, 2, 3, 4};
    constexpr Vec4 b{2, 4, 5, 8};

    REQUIRE(distance_squared(a, b) == 25);
    REQUIRE(distance_squared(b, a) == 25);
    REQUIRE(distance_squared(a, a) == 0);

    constexpr Vec4 negative{-1, -2, -2, -4};
    constexpr Vec4 zero{0, 0, 0, 0};

    REQUIRE(distance_squared(negative, zero) == 25);
}

TEST_CASE("Vec4 distance")
{
    constexpr Vec4 a{1, 2, 3, 4};
    constexpr Vec4 b{2, 4, 5, 8};

    REQUIRE(test_near(distance(a, b), 5.0f));
    REQUIRE(test_near(distance(b, a), 5.0f));
    REQUIRE(test_near(distance(a, a), 0.0f));

    constexpr Vec4 c{0, 0, 0, 0};
    constexpr Vec4 d{1, 1, 1, 0};

    REQUIRE(test_near(distance(c, d), std::sqrt(3.0f)));
}

TEST_CASE("Vec4 lerp")
{
    constexpr Vec4 from{-2, 4, 6, 8};
    constexpr Vec4 to{6, -4, -2, 0};

    REQUIRE(lerp(from, to, 0.0f) == from);
    REQUIRE(lerp(from, to, 1.0f) == to);

    REQUIRE(lerp(from, to, 0.5f) == Vec4{2, 0, 2, 4});
    REQUIRE(lerp(from, to, 0.25f) == Vec4{0, 2, 4, 6});

    REQUIRE(lerp(from, to, -0.5f) == Vec4{-6, 8, 10, 12});
    REQUIRE(lerp(from, to, 1.5f) == Vec4{10, -8, -6, -4});

    REQUIRE(lerp(from, from, 0.5f) == from);
}

TEST_CASE("Vec4 min")
{
    constexpr Vec4 a{1, -4, 8, -10};
    constexpr Vec4 b{-2, 3, 5, 7};

    REQUIRE(
        yarep::math::min(a, b)
        == Vec4{-2,
        -4,
        5,
        -10}
    );

    REQUIRE(
        yarep::math::min(b, a)
        == Vec4{-2,
        -4,
        5,
        -10}
    );

    constexpr Vec4 equal{2, 2, 2, 2};
    REQUIRE(yarep::math::min(equal, equal) == equal);
}

TEST_CASE("Vec4 max")
{
    constexpr Vec4 a{1, -4, 8, -10};
    constexpr Vec4 b{-2, 3, 5, 7};

    REQUIRE(
        yarep::math::max(a, b)
        == Vec4{1,
        3,
        8,
        7}
    );

    REQUIRE(
        yarep::math::max(b, a)
        == Vec4{1,
        3,
        8,
        7}
    );

    constexpr Vec4 equal{2, 2, 2, 2};
    REQUIRE(yarep::math::max(equal, equal) == equal);
}

TEST_CASE("Vec4 clamp")
{
    constexpr Vec4 minimum{0, -2, 1, -4};
    constexpr Vec4 maximum{4, 2, 3, 4};

    REQUIRE(
        yarep::math::clamp(
            Vec4{2, 0, 2, 0},
            minimum,
            maximum)
        == Vec4{2,
        0,
        2,
        0}
    );

    REQUIRE(
        yarep::math::clamp(
            Vec4{5, -3, 0.5f, 8},
            minimum,
            maximum)
        == Vec4{4,
        -2,
        1,
        4}
    );

    REQUIRE(
        yarep::math::clamp(
            Vec4{-5, 8, 10, -10},
            minimum,
            maximum)
        == Vec4{0,
        2,
        3,
        -4}
    );

    REQUIRE(
        yarep::math::clamp(minimum, minimum, maximum)
        == minimum
    );

    REQUIRE(
        yarep::math::clamp(maximum, minimum, maximum)
        == maximum
    );
}

TEST_CASE("Vec4 nearly equal")
{
    constexpr float epsilon = 0.0001f;

    constexpr Vec4 a{1.0f, -2.0f, 3.0f, -4.0f};

    constexpr Vec4 equal{
        1.0f,
        -2.0f,
        3.0f,
        -4.0f
    };

    constexpr Vec4 close{
        1.00005f,
        -1.99995f,
        3.00005f,
        -3.99995f
    };

    REQUIRE(nearly_equal(a, equal, epsilon));
    REQUIRE(nearly_equal(a, close, epsilon));

    REQUIRE_FALSE(
        nearly_equal(
            a,
            Vec4{1.0002f, -2, 3, -4},
            epsilon)
    );

    REQUIRE_FALSE(
        nearly_equal(
            a,
            Vec4{1, -1.9998f, 3, -4},
            epsilon)
    );

    REQUIRE_FALSE(
        nearly_equal(
            a,
            Vec4{1, -2, 3.0002f, -4},
            epsilon)
    );

    REQUIRE_FALSE(
        nearly_equal(
            a,
            Vec4{1, -2, 3, -3.9998f},
            epsilon)
    );

    REQUIRE(
        nearly_equal(
            Vec4{0, 0, 0, 0},
            Vec4{0, 0, 0, 0},
            epsilon)
    );
}
