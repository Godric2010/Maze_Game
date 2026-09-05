#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include <cmath>

#include "../include/AngleMath.hpp"

using namespace yarep::math;

namespace {
    constexpr float TEST_EPSILON = 0.0001f;

    bool test_near(
            const float lhs,
            const float rhs,
            const float epsilon = TEST_EPSILON) {
        return std::abs(lhs - rhs) <= epsilon;
    }
}


// ============================================================================
// normalize_angle
// ============================================================================

TEST_CASE("Angle normalize keeps normalized positive angles") {
    const Angle zero = normalize_angle(
            Angle::from_degrees(0)
            );

    const Angle thirty = normalize_angle(
            Angle::from_degrees(30)
            );

    const Angle three_fifty_nine = normalize_angle(
            Angle::from_degrees(359)
            );

    REQUIRE(test_near(zero.degrees(), 0.0f));
    REQUIRE(test_near(thirty.degrees(), 30.0f));
    REQUIRE(test_near(three_fifty_nine.degrees(), 359.0f));
}

TEST_CASE("Angle normalize wraps full rotations") {
    const Angle full_rotation = normalize_angle(
            Angle::from_degrees(360)
            );

    const Angle two_rotations = normalize_angle(
            Angle::from_degrees(720)
            );

    const Angle three_rotations = normalize_angle(
            Angle::from_degrees(1080)
            );

    REQUIRE(test_near(full_rotation.degrees(), 0.0f));
    REQUIRE(test_near(two_rotations.degrees(), 0.0f));
    REQUIRE(test_near(three_rotations.degrees(), 0.0f));
}

TEST_CASE("Angle normalize wraps angles above full rotation") {
    const Angle a = normalize_angle(
            Angle::from_degrees(370)
            );

    const Angle b = normalize_angle(
            Angle::from_degrees(750)
            );

    const Angle c = normalize_angle(
            Angle::from_degrees(1085)
            );

    REQUIRE(test_near(a.degrees(), 10.0f));
    REQUIRE(test_near(b.degrees(), 30.0f));
    REQUIRE(test_near(c.degrees(), 5.0f));
}

TEST_CASE("Angle normalize wraps negative angles") {
    const Angle a = normalize_angle(
            Angle::from_degrees(-10)
            );

    const Angle b = normalize_angle(
            Angle::from_degrees(-90)
            );

    const Angle c = normalize_angle(
            Angle::from_degrees(-359)
            );

    REQUIRE(test_near(a.degrees(), 350.0f));
    REQUIRE(test_near(b.degrees(), 270.0f));
    REQUIRE(test_near(c.degrees(), 1.0f));
}

TEST_CASE("Angle normalize wraps multiple negative rotations") {
    const Angle a = normalize_angle(
            Angle::from_degrees(-360)
            );

    const Angle b = normalize_angle(
            Angle::from_degrees(-370)
            );

    const Angle c = normalize_angle(
            Angle::from_degrees(-730)
            );

    REQUIRE(test_near(a.degrees(), 0.0f));
    REQUIRE(test_near(b.degrees(), 350.0f));
    REQUIRE(test_near(c.degrees(), 350.0f));
}


// ============================================================================
// delta_angle
// ============================================================================

TEST_CASE("Angle delta calculates simple positive difference") {
    const Angle result = delta_angle(
            Angle::from_degrees(30),
            Angle::from_degrees(80)
            );

    REQUIRE(test_near(result.degrees(), 50.0f));
}

TEST_CASE("Angle delta calculates simple negative difference") {
    const Angle result = delta_angle(
            Angle::from_degrees(80),
            Angle::from_degrees(30)
            );

    auto result_deg = result.degrees();
    REQUIRE(test_near(result.degrees(), -50.0f));
}

TEST_CASE("Angle delta returns zero for equal angles") {
    const Angle result = delta_angle(
            Angle::from_degrees(45),
            Angle::from_degrees(45)
            );

    REQUIRE(test_near(result.degrees(), 0.0f));
}

TEST_CASE("Angle delta recognizes equivalent rotations") {
    const Angle a = delta_angle(
            Angle::from_degrees(0),
            Angle::from_degrees(360)
            );

    const Angle b = delta_angle(
            Angle::from_degrees(30),
            Angle::from_degrees(390)
            );

    const Angle c = delta_angle(
            Angle::from_degrees(-10),
            Angle::from_degrees(350)
            );

    REQUIRE(test_near(a.degrees(), 0.0f));
    REQUIRE(test_near(b.degrees(), 0.0f));
    REQUIRE(test_near(c.degrees(), 0.0f));
}

TEST_CASE("Angle delta takes shortest path across zero positive") {
    const Angle result = delta_angle(
            Angle::from_degrees(350),
            Angle::from_degrees(10)
            );

    REQUIRE(test_near(result.degrees(), 20.0f));
}

TEST_CASE("Angle delta takes shortest path across zero negative") {
    const Angle result = delta_angle(
            Angle::from_degrees(10),
            Angle::from_degrees(350)
            );

    REQUIRE(test_near(result.degrees(), -20.0f));
}

TEST_CASE("Angle delta handles angles outside normalized range") {
    const Angle a = delta_angle(
            Angle::from_degrees(710),
            Angle::from_degrees(370)
            );

    const Angle b = delta_angle(
            Angle::from_degrees(-350),
            Angle::from_degrees(-10)
            );

    REQUIRE(test_near(a.degrees(), 20.0f));
    REQUIRE(test_near(b.degrees(), -20.0f));
}

TEST_CASE("Angle delta handles opposite directions") {
    const Angle positive = delta_angle(
            Angle::from_degrees(0),
            Angle::from_degrees(180)
            );

    const Angle negative = delta_angle(
            Angle::from_degrees(0),
            Angle::from_degrees(-180)
            );

    REQUIRE(test_near(
                std::abs(positive.degrees()),
                180.0f)
            );

    REQUIRE(test_near(
                std::abs(negative.degrees()),
                180.0f)
            );
}

TEST_CASE("Angle delta is inverse when direction is reversed") {
    const Angle forward = delta_angle(
            Angle::from_degrees(350),
            Angle::from_degrees(40)
            );

    const Angle backward = delta_angle(
            Angle::from_degrees(40),
            Angle::from_degrees(350)
            );

    REQUIRE(test_near(
                forward.degrees(),
                -backward.degrees())
            );
}


// ============================================================================
// lerp
// ============================================================================

TEST_CASE("Angle lerp returns start at zero") {
    const Angle result = lerp(
            Angle::from_degrees(30),
            Angle::from_degrees(90),
            0.0f
            );

    REQUIRE(test_near(result.degrees(), 30.0f));
}

TEST_CASE("Angle lerp returns target at one") {
    const Angle result = lerp(
            Angle::from_degrees(30),
            Angle::from_degrees(90),
            1.0f
            );

    REQUIRE(test_near(result.degrees(), 90.0f));
}

TEST_CASE("Angle lerp interpolates simple angles") {
    const Angle halfway = lerp(
            Angle::from_degrees(30),
            Angle::from_degrees(90),
            0.5f
            );

    const Angle quarter = lerp(
            Angle::from_degrees(30),
            Angle::from_degrees(90),
            0.25f
            );

    REQUIRE(test_near(halfway.degrees(), 60.0f));
    REQUIRE(test_near(quarter.degrees(), 45.0f));
}

TEST_CASE("Angle lerp takes shortest path across zero positive") {
    const Angle halfway = lerp(
            Angle::from_degrees(350),
            Angle::from_degrees(10),
            0.5f
            );

    const bool close_to_zero = test_near(halfway.degrees(), 0.0f);
    const bool close_to_360 = test_near(halfway.degrees(), 360.0f);
    bool requirement_fulfilled = close_to_zero || close_to_360;
    REQUIRE(requirement_fulfilled);
}

TEST_CASE("Angle lerp takes shortest path across zero negative") {
    const Angle halfway = lerp(
            Angle::from_degrees(10),
            Angle::from_degrees(350),
            0.5f
            );

    const bool close_to_zero = test_near(halfway.degrees(), 0.0f);
    const bool close_to_360 = test_near(halfway.degrees(), 360.0f);
    bool requirement_fulfilled = close_to_zero || close_to_360;
    REQUIRE(requirement_fulfilled);
}

TEST_CASE("Angle lerp does not move between equivalent rotations") {
    const Angle halfway = lerp(
            Angle::from_degrees(30),
            Angle::from_degrees(390),
            0.5f
            );

    REQUIRE(
            nearly_equal(
                halfway,
                Angle::from_degrees(30),
                TEST_EPSILON)
            );
}

TEST_CASE("Angle lerp supports extrapolation") {
    const Angle before = lerp(
            Angle::from_degrees(30),
            Angle::from_degrees(90),
            -0.5f
            );

    const Angle after = lerp(
            Angle::from_degrees(30),
            Angle::from_degrees(90),
            1.5f
            );

    REQUIRE(
            nearly_equal(
                before,
                Angle::from_degrees(0),
                TEST_EPSILON)
            );

    REQUIRE(
            nearly_equal(
                after,
                Angle::from_degrees(120),
                TEST_EPSILON)
            );
}


// ============================================================================
// sin
// ============================================================================

TEST_CASE("Angle sin calculates cardinal angles") {
    REQUIRE(test_near(
                yarep::math::sin(Angle::from_degrees(0)),
                0.0f)
            );

    REQUIRE(test_near(
                yarep::math::sin(Angle::from_degrees(90)),
                1.0f)
            );

    REQUIRE(test_near(
                yarep::math::sin(Angle::from_degrees(180)),
                0.0f)
            );

    REQUIRE(test_near(
                yarep::math::sin(Angle::from_degrees(270)),
                -1.0f)
            );

    REQUIRE(test_near(
                yarep::math::sin(Angle::from_degrees(360)),
                0.0f)
            );
}

TEST_CASE("Angle sin handles negative angles") {
    REQUIRE(test_near(
                yarep::math::sin(Angle::from_degrees(-90)),
                -1.0f)
            );

    REQUIRE(test_near(
                yarep::math::sin(Angle::from_degrees(-270)),
                1.0f)
            );
}

TEST_CASE("Angle sin handles angles above full rotation") {
    REQUIRE(test_near(
                yarep::math::sin(Angle::from_degrees(450)),
                1.0f)
            );

    REQUIRE(test_near(
                yarep::math::sin(Angle::from_degrees(720)),
                0.0f)
            );
}


// ============================================================================
// cos
// ============================================================================

TEST_CASE("Angle cos calculates cardinal angles") {
    REQUIRE(test_near(
                yarep::math::cos(Angle::from_degrees(0)),
                1.0f)
            );

    REQUIRE(test_near(
                yarep::math::cos(Angle::from_degrees(90)),
                0.0f)
            );

    REQUIRE(test_near(
                yarep::math::cos(Angle::from_degrees(180)),
                -1.0f)
            );

    REQUIRE(test_near(
                yarep::math::cos(Angle::from_degrees(270)),
                0.0f)
            );

    REQUIRE(test_near(
                yarep::math::cos(Angle::from_degrees(360)),
                1.0f)
            );
}

TEST_CASE("Angle cos handles negative angles") {
    REQUIRE(test_near(
                yarep::math::cos(Angle::from_degrees(-90)),
                0.0f)
            );

    REQUIRE(test_near(
                yarep::math::cos(Angle::from_degrees(-180)),
                -1.0f)
            );
}

TEST_CASE("Angle cos handles angles above full rotation") {
    REQUIRE(test_near(
                yarep::math::cos(Angle::from_degrees(450)),
                0.0f)
            );

    REQUIRE(test_near(
                yarep::math::cos(Angle::from_degrees(720)),
                1.0f)
            );
}


// ============================================================================
// tan
// ============================================================================

TEST_CASE("Angle tan calculates common angles") {
    REQUIRE(test_near(
                yarep::math::tan(Angle::from_degrees(0)),
                0.0f)
            );

    REQUIRE(test_near(
                yarep::math::tan(Angle::from_degrees(45)),
                1.0f)
            );

    REQUIRE(test_near(
                yarep::math::tan(Angle::from_degrees(-45)),
                -1.0f)
            );

    REQUIRE(test_near(
                yarep::math::tan(Angle::from_degrees(180)),
                0.0f)
            );
}

TEST_CASE("Angle tan handles angles above full rotation") {
    REQUIRE(test_near(
                yarep::math::tan(Angle::from_degrees(405)),
                1.0f)
            );

    REQUIRE(test_near(
                yarep::math::tan(Angle::from_degrees(720)),
                0.0f)
            );
}


// ============================================================================
// nearly_equal
// ============================================================================

TEST_CASE("Angle nearly equal recognizes equal angles") {
    constexpr float tolerance = 1e-5f;

    REQUIRE(
            nearly_equal(
                Angle::from_degrees(30),
                Angle::from_degrees(30),
                tolerance)
            );
}

TEST_CASE("Angle nearly equal recognizes values within tolerance") {
    constexpr float tolerance = 1e-5f;

    REQUIRE(
            nearly_equal(
                Angle::from_degrees(30),
                Angle::from_degrees(30.0005f),
                tolerance)
            );

    REQUIRE(
            nearly_equal(
                Angle::from_degrees(30),
                Angle::from_degrees(29.9995f),
                tolerance)
            );
}

TEST_CASE("Angle nearly equal rejects values outside tolerance") {
    constexpr float tolerance = 1e-5f;

    REQUIRE_FALSE(
            nearly_equal(
                Angle::from_degrees(30),
                Angle::from_degrees(30.01f),
                tolerance)
            );

    REQUIRE_FALSE(
            nearly_equal(
                Angle::from_degrees(30),
                Angle::from_degrees(29.99f),
                tolerance)
            );
}

TEST_CASE("Angle nearly equal recognizes equivalent full rotations") {
    constexpr float tolerance = 1e-5f;

    REQUIRE(
            nearly_equal(
                Angle::from_degrees(0),
                Angle::from_degrees(360),
                tolerance)
            );

    REQUIRE(
            nearly_equal(
                Angle::from_degrees(30),
                Angle::from_degrees(390),
                tolerance)
            );

    REQUIRE(
            nearly_equal(
                Angle::from_degrees(-10),
                Angle::from_degrees(350),
                tolerance)
            );

    REQUIRE(
            nearly_equal(
                Angle::from_degrees(720),
                Angle::from_degrees(0),
                tolerance)
            );
}

TEST_CASE("Angle nearly equal handles tolerance across zero") {
    constexpr float tolerance = 1e-4f;

    REQUIRE(
            nearly_equal(
                Angle::from_degrees(359.995f),
                Angle::from_degrees(0),
                tolerance)
            );

    REQUIRE_FALSE(
            nearly_equal(
                Angle::from_degrees(359.9f),
                Angle::from_degrees(0),
                tolerance)
            );
}
