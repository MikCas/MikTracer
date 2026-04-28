#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Interval.h"

using Catch::Approx;

TEST_CASE("Interval Construction", "[Interval]") {

    SECTION("Default constructor produces an empty interval") {
        Interval i;
        REQUIRE(i.min > i.max); // min=+inf, max=-inf — nothing fits inside
    }

    SECTION("Parameterised constructor stores min and max") {
        Interval i(1.0, 5.0);
        REQUIRE(i.min == 1.0);
        REQUIRE(i.max == 5.0);
    }

    SECTION("size() returns max - min") {
        Interval i(1.0, 5.0);
        REQUIRE(i.size() == Approx(4.0));
    }
}

TEST_CASE("Interval::contains and surrounds", "[Interval]") {
    Interval i(1.0, 5.0);

    SECTION("contains includes boundary values") {
        REQUIRE(i.contains(1.0));       // min boundary — included
        REQUIRE(i.contains(5.0));       // max boundary — included
        REQUIRE(i.contains(3.0));       // strictly inside
        REQUIRE_FALSE(i.contains(0.9));
        REQUIRE_FALSE(i.contains(5.1));
    }

    SECTION("surrounds excludes boundary values") {
        REQUIRE_FALSE(i.surrounds(1.0)); // min boundary — excluded
        REQUIRE_FALSE(i.surrounds(5.0)); // max boundary — excluded
        REQUIRE(i.surrounds(3.0));       // strictly inside
        REQUIRE_FALSE(i.surrounds(0.9));
        REQUIRE_FALSE(i.surrounds(5.1));
    }
}

TEST_CASE("Interval::clamp", "[Interval]") {
    Interval i(1.0, 5.0);

    SECTION("value below range clamps to min") {
        REQUIRE(i.clamp(0.0) == Approx(1.0));
    }

    SECTION("value above range clamps to max") {
        REQUIRE(i.clamp(9.0) == Approx(5.0));
    }

    SECTION("value inside range is unchanged") {
        REQUIRE(i.clamp(3.0) == Approx(3.0));
    }
}

TEST_CASE("Interval sentinels", "[Interval]") {

    SECTION("empty contains nothing") {
        REQUIRE_FALSE(Interval::empty.contains(0.0));
        REQUIRE_FALSE(Interval::empty.contains(1e10));
    }

    SECTION("universe contains everything") {
        REQUIRE(Interval::universe.contains(0.0));
        REQUIRE(Interval::universe.contains(1e10));
        REQUIRE(Interval::universe.contains(-1e10));
    }
}