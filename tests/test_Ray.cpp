#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Ray.h"

using Catch::Approx;

TEST_CASE("Ray Construction", "[Ray]") {

    SECTION("Default constructor initialises to 0") {
        Ray r;
        REQUIRE(r.origin()    == Vec3(0, 0, 0));
        REQUIRE(r.direction() == Vec3(0, 0, 0));
    }

    SECTION("Parameterised constructor stores origin and direction") {
        Ray r(Vec3(1.0, 2.0, 3.0), Vec3(4.0, 5.0, 6.0));
        REQUIRE(r.origin()    == Vec3(1.0, 2.0, 3.0));
        REQUIRE(r.direction() == Vec3(4.0, 5.0, 6.0));
    }

    SECTION("Getters are callable on a const Ray") {
        const Ray r(Vec3(1.0, 2.0, 3.0), Vec3(4.0, 5.0, 6.0));
        REQUIRE(r.origin()    == Vec3(1.0, 2.0, 3.0));
        REQUIRE(r.direction() == Vec3(4.0, 5.0, 6.0));
    }
}

TEST_CASE("Ray::at(t)", "[Ray]") {
    Ray r(Vec3(1.0, 2.0, 3.0), Vec3(1.0, 0.0, 0.0));

    SECTION("t=0 returns the origin") {
        REQUIRE(r.at(0.0) == Vec3(1.0, 2.0, 3.0));
    }

    SECTION("t=1 advances one step along direction") {
        REQUIRE(r.at(1.0) == Vec3(2.0, 2.0, 3.0));
    }

    SECTION("t=2 advances two steps along direction") {
        REQUIRE(r.at(2.0) == Vec3(3.0, 2.0, 3.0));
    }

    SECTION("negative t steps backward along direction") {
        REQUIRE(r.at(-1.0) == Vec3(0.0, 2.0, 3.0));
    }
}