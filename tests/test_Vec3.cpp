#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Vec3.h"

using Catch::Approx;

TEST_CASE("Vec3 Basic Operations", "[Vec3]") {
    SECTION("Construction and Accessors") {
        Vec3 v1;
        REQUIRE(v1.x() == 0.0);
        REQUIRE(v1.y() == 0.0);
        REQUIRE(v1.z() == 0.0);

        Vec3 v2(1.0, 2.0, 3.0);
        REQUIRE(v2.x() == 1.0);
        REQUIRE(v2.y() == 2.0);
        REQUIRE(v2.z() == 3.0);
    }

    SECTION("Equality") {
        Vec3 a(1.0, 2.0, 3.0);
        Vec3 b(1.0, 2.0, 3.0);
        Vec3 c(1.0, 2.0, 4.0);

        REQUIRE(a == b);
        REQUIRE_FALSE(a == c);
        REQUIRE(a != c);
        REQUIRE_FALSE(a != b);
    }

    SECTION("Arithmetic Operators") {
        Vec3 a(1, 2, 3);
        Vec3 b(4, 5, 6);

        Vec3 sum = a + b;
        REQUIRE(sum.x() == 5);
        REQUIRE(sum.y() == 7);
        REQUIRE(sum.z() == 9);

        Vec3 diff = b - a;
        REQUIRE(diff.x() == 3);
        REQUIRE(diff.y() == 3);
        REQUIRE(diff.z() == 3);

        Vec3 scaled = a * 2.0;
        REQUIRE(scaled.x() == 2.0);
    }
}

TEST_CASE("Vec3 Vector Math", "[Vec3]") {

    SECTION("Dot Product") {
        Vec3 v1(1, 0, 0);
        Vec3 v2(0, 1, 0);
        Vec3 v3(1, 2, 3);

        REQUIRE(dot(v1, v2) == 0.0); // Perpendicular
        REQUIRE(dot(v3, v3) == 14.0); // 1*1 + 2*2 + 3*3
    }

    SECTION("Cross Product") {
        Vec3 x(1, 0, 0);
        Vec3 y(0, 1, 0);
        
        Vec3 z = cross(x, y);
        REQUIRE(z.x() == 0);
        REQUIRE(z.y() == 0);
        REQUIRE(z.z() == 1);

        // a x b = -(b x a)
        Vec3 anti_z = cross(y, x);
        REQUIRE(anti_z.z() == -1);
    }

    SECTION("Length and Normalisation") {
        Vec3 v(3, 4, 0);
        REQUIRE(v.length() == Approx(5.0));

        Vec3 n = normalise(v);
        REQUIRE(n.length() == Approx(1.0));
        REQUIRE(n.x() == Approx(0.6)); // 3/5
    }
}

TEST_CASE("Vec3 Raytracing", "[Vec3]") {

    SECTION("Reflection") {
        Vec3 v(1, -1, 0);
        Vec3 n(0, 1, 0);
        Vec3 reflected = reflect(v, n);
        
        // Incoming (1,-1) reflects across Y-axis normal to (1,1)
        REQUIRE(reflected.x() == 1.0);
        REQUIRE(reflected.y() == 1.0);
    }

    SECTION("nearZero") {
        Vec3 tiny(1e-9, 1e-9, 1e-9);
        Vec3 normal(0.1, 0.5, 0.9);

        REQUIRE(tiny.nearZero() == true);
        REQUIRE(normal.nearZero() == false);
    }
}