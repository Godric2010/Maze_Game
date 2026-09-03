#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include <catch2/catch_approx.hpp>
#include "../include/Angle.hpp"
using namespace yarep::math;

constexpr float allowed_margin = 0.0001f;

TEST_CASE("Angle Create empty") {
    constexpr auto angle = Angle();
    REQUIRE(angle.radians() == 0.0f);
    REQUIRE(angle.degrees() == 0.0f);
}

TEST_CASE("Angle from degrees") {
    auto angle_from_deg = Angle::from_degrees(45);
    float target_angle_in_rad = 0.78549816f;
    REQUIRE(angle_from_deg.degrees() == 45.0f);
    REQUIRE(angle_from_deg.radians() == Catch::Approx(target_angle_in_rad).margin(allowed_margin));

    angle_from_deg = Angle::from_degrees(-45);
    target_angle_in_rad = -0.78549816f;
    REQUIRE(angle_from_deg.degrees() == -45.0f);
    REQUIRE(angle_from_deg.radians() == Catch::Approx(target_angle_in_rad).margin(allowed_margin));

    angle_from_deg = Angle::from_degrees(405);
    target_angle_in_rad = 7.06858347f;
    REQUIRE(angle_from_deg.degrees() == 405.0f);
    REQUIRE(angle_from_deg.radians() == Catch::Approx(target_angle_in_rad).margin(allowed_margin));
}

TEST_CASE("Angle from radians") {
    constexpr auto angle_from_radians = Angle::from_radians(std::numbers::pi_v<float>);
    constexpr float target_angle_in_deg = 180.0f;
    REQUIRE(angle_from_radians.degrees() == target_angle_in_deg);
}

TEST_CASE("Angle additions") {
    constexpr auto a = Angle::from_degrees(30);
    constexpr auto b = Angle::from_degrees(-30);
    constexpr auto c = Angle::from_degrees(350);
    constexpr auto d = Angle::from_degrees(-45);

    constexpr auto a_plus_b = a + b;
    REQUIRE(a_plus_b.degrees() == 0.0f);

    constexpr auto a_plus_a = a + a;
    REQUIRE(a_plus_a.degrees() == 60.0f);

    constexpr auto a_plus_d = a + d;
    REQUIRE(a_plus_d.degrees() == -15.0f);

    constexpr auto a_plus_c = a + c;
    REQUIRE(a_plus_c.degrees() == Catch::Approx(380.0f).margin(allowed_margin));
}

TEST_CASE("Angle subtractions") {
    constexpr auto a = Angle::from_degrees(30);
    constexpr auto b = Angle::from_degrees(-30);
    constexpr auto c = Angle::from_degrees(380);
    constexpr auto d = Angle::from_degrees(-45);

    constexpr auto a_minus_b = a - b;
    REQUIRE(a_minus_b.degrees() == 60.0f);

    constexpr auto a_minus_a = a - a;
    REQUIRE(a_minus_a.degrees() == 0.0f);

    constexpr auto d_minus_a = d - a;
    REQUIRE(d_minus_a.degrees() == -75.0f);

    constexpr auto a_minus_c = a - c;
    REQUIRE(a_minus_c.degrees() == -350.0f);
}

TEST_CASE("Angle multiplications") {
    constexpr auto angle = Angle::from_degrees(30);
    constexpr auto angle_neg = Angle::from_degrees(-30);
    constexpr auto angle_nil = Angle();

    constexpr float scalar = 2;
    constexpr float scalar_neg = -2;
    constexpr float scalar_nil = 0;

    auto result = angle * scalar;
    REQUIRE(result.degrees() == 60);
    result = scalar * angle;
    REQUIRE(result.degrees() == 60);

    result = angle * scalar_neg;
    REQUIRE(result.degrees() == -60);

    result = angle * scalar_nil;
    REQUIRE(result.degrees() == 0);

    result = angle_neg * scalar;
    REQUIRE(result.degrees() == -60);

    result = angle_neg * scalar_neg;
    REQUIRE(result.degrees() == 60);

    result = angle_neg * scalar_nil;
    REQUIRE(result.degrees() == 0);

    result = angle_nil * scalar;
    REQUIRE(result.degrees() == 0);

    result = angle_nil * scalar_nil;
    REQUIRE(result.degrees() == 0);

    result = angle_nil * scalar_neg;
    REQUIRE(result.degrees() == 0);
}

TEST_CASE("Angle divisions") {
    constexpr auto angle = Angle::from_degrees(30);
    constexpr auto angle_neg = Angle::from_degrees(-30);
    constexpr auto angle_nil = Angle();

    constexpr float scalar = 2;
    constexpr float scalar_neg = -2;
    constexpr float scalar_nil = 0;

    auto result = angle / scalar;
    REQUIRE(result.degrees() == 15);

    result = angle / scalar_neg;
    REQUIRE(result.degrees() == -15);

    REQUIRE_THROWS(angle/ scalar_nil);

    result = angle_neg / scalar;
    REQUIRE(result.degrees() == -15);

    result = angle_neg / scalar_neg;
    REQUIRE(result.degrees() == 15);

    REQUIRE_THROWS(angle_neg / scalar_nil);

    result = angle_nil / scalar;
    REQUIRE(result.degrees() == 0);

    result = angle_nil / scalar_neg;
    REQUIRE(result.degrees() == 0);

    REQUIRE_THROWS(angle_nil / scalar_nil);
}

TEST_CASE("Angle equality") {
    constexpr auto a = Angle::from_degrees(30);
    constexpr auto b = Angle::from_degrees(30);
    constexpr auto c = Angle::from_radians(0.5235f);

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}
