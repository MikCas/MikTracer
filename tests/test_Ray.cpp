#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Ray.h"

using Catch::Approx;

// Construction: origin and direction stored correctly.
// at(t) returns origin + t * direction for t = 0, t = 1, negative t.
// Const-correctness of getters.

TEST_CASE("Ray Basic Operations", "[Ray]") {
    SECTION("Construction") {
        Ray r;
        REQUIRE(r.origin() == Vec3(0, 0, 0));
        REQUIRE(r.direction() == Vec3(0, 0, 0));
    }

    SECTION("GETTERS") {

    }

    SECTION("OPERATIONS") {

    }
}