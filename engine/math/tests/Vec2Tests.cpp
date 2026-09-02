#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../include/Vec2.hpp"

using namespace yarep::math;

TEST_CASE("Vec2 empty constructor") {
    constexpr auto vec = Vec2();
    REQUIRE(vec.x == 0);
    REQUIRE(vec.y == 0);
}

TEST_CASE("Vec2 constructor") {
    constexpr auto vec = Vec2(1, 2);
    REQUIRE(vec.x == 1);
    REQUIRE(vec.y == 2);

    constexpr auto vec2 = Vec2(-13, -1.467f);
    REQUIRE(vec2.x == -13);
    REQUIRE(vec2.y == -1.467f);
}

TEST_CASE("Vec2 addition") {
    const Vec2& first{1.0f, 2.0f};
    const Vec2& second{3.0f, 4.0f};

    Vec2 result = first + second;
    REQUIRE(result == Vec2{4, 6});

    const Vec2& third{-4, -3};
    result = third + second;
    REQUIRE(result == Vec2{-1, 1});
}

TEST_CASE("Vec2 addition assignment") {
    Vec2 a{1.0f, 2.0f};
    constexpr Vec2 add{3.0f, 4.0f};

    a += add;
    REQUIRE(a == Vec2{4, 6});
    REQUIRE(add == Vec2{3, 4});

    Vec2 b{-4, -3};
    b += add;
    REQUIRE(b == Vec2{-1, 1});

    Vec2 c{3, 5};
    c += c;
    REQUIRE(c == Vec2{6, 10});
}

TEST_CASE("Vec2 subtraction") {
    const Vec2& first{1.0f, 2.0f};
    const Vec2& second{3.0f, 4.0f};
    Vec2 result = first - second;
    REQUIRE(result == Vec2{-2, -2});

    const Vec2& third{-4, -3};
    const Vec2& fourth{-4, -4};
    result = third - fourth;
    REQUIRE(result == Vec2{0, 1});

    result = second - fourth;
    REQUIRE(result == Vec2{7, 8});
}

TEST_CASE("Vec2 subtraction assignment") {
    Vec2 a{1.0f, 2.0f};
    Vec2 b{1.0f, 2.0f};
    constexpr Vec2 sub_pos{3.0f, 4.0f};
    a -= sub_pos;
    REQUIRE(a == Vec2{-2, -2});
    REQUIRE(sub_pos == Vec2{3, 4});

    constexpr Vec2 sub_neg{-4, -3};
    b -= sub_neg;
    REQUIRE(b == Vec2{5, 5});

    Vec2 c{3, 5};
    c -= c;
    REQUIRE(c == Vec2{0, 0});
}

TEST_CASE("Vec2 multiplication") {
    const Vec2& vector{1.0f, 2.0f};
    const Vec2& zero{0.0f, 0.0f};
    constexpr float scalar = 3;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    Vec2 result = vector * scalar;
    REQUIRE(result == Vec2{3, 6});
    result = scalar * vector;
    REQUIRE(result == Vec2{3, 6});

    result = vector * neg_scalar;
    REQUIRE(result == Vec2{-2, -4});

    result = vector * nil_scalar;
    REQUIRE(result == Vec2{0, 0});

    result = zero * scalar;
    REQUIRE(result == Vec2{0, 0});

    result = zero * neg_scalar;
    REQUIRE(result == Vec2{0, 0});
}

TEST_CASE("Vec2 multiplication assignment") {
    Vec2 first{1.0f, 2.0f};
    Vec2 second{1.0f, 2.0f};
    Vec2 third{1.0f, 2.0f};
    Vec2 zero{0.0f, 0.0f};

    constexpr float scalar = 3;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    first *= scalar;
    REQUIRE(first == Vec2{3, 6});

    second *= neg_scalar;
    REQUIRE(second == Vec2{-2, -4});

    third *= nil_scalar;
    REQUIRE(third == Vec2{0, 0});

    zero *= scalar;
    REQUIRE(zero == Vec2{0, 0});
}

TEST_CASE("Vec2 division") {
    const Vec2& vector{10.0f, 4.0f};
    const Vec2& zero{0.0f, 0.0f};
    constexpr float scalar = 2;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    Vec2 result = vector / scalar;
    REQUIRE(result == Vec2{5, 2});

    result = vector / neg_scalar;
    REQUIRE(result == Vec2{-5, -2});

    REQUIRE_THROWS(vector / nil_scalar);

    result = zero / scalar;
    REQUIRE(result == Vec2{0, 0});
}

TEST_CASE("Vec2 division assignment") {
    Vec2 first{10.0f, 4.0f};
    Vec2 second{10.0f, 4.0f};
    Vec2 third{10.0f, 4.0f};
    Vec2 fourth{0, 0};

    constexpr float scalar = 2;
    constexpr float neg_scalar = -2;
    constexpr float nil_scalar = 0;

    first /= scalar;
    REQUIRE(first == Vec2{5, 2});

    second /= neg_scalar;
    REQUIRE(second == Vec2{-5, -2});

    REQUIRE_THROWS(third /= nil_scalar);
    REQUIRE(third == Vec2{10, 4});

    fourth /= scalar;
    REQUIRE(fourth == Vec2{0, 0});
}

TEST_CASE("Vec2 unary") {
    const Vec2& positive{1.0f, 2.0f};
    const Vec2& negative{-1.0f, -2.0f};
    const Vec2& zero{0.0f, 0.0f};
    const Vec2& mixed{-1.0f, 1.0f};

    auto result_positive = -positive;
    REQUIRE(result_positive == Vec2{-1, -2});

    auto result_negative = -negative;
    REQUIRE(result_negative == Vec2{1, 2});

    auto result_zero = -zero;
    REQUIRE(result_zero == Vec2{0, 0});

    auto result_mixed = -mixed;
    REQUIRE(result_mixed == Vec2{1, -1});
}

TEST_CASE("Vec2 equals") {
    const Vec2& a{1, 2};
    const Vec2& b{1, 2};
    const Vec2& c{1.0000001f, 2};

    bool a_eq_b = a == b;
    REQUIRE(a_eq_b);

    bool a_eq_c = a == c;
    REQUIRE_FALSE(a_eq_c);
}

static_assert(Vec2{1.0f, 2.0f} + Vec2{3.0f, 4.0f} == Vec2{4.0f, 6.0f});
static_assert(Vec2{2.0f, 3.0f} * 2.0f == Vec2{4.0f, 6.0f});
