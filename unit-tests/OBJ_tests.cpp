#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include "Objects/Sphere.h"
#include "Objects/Plane.h"
#include "Objects/MandelBulb.h"
#include "Vec.h"

Vec origin_0(0,0,0);
Vec origin_1(5,0,0);
Vec origin_2(0.5,0,0);
Vec origin_3(4, 2, 5); // 16 + 4 + 25 = 45, sqrt(45) = 6.708...
double tol = 0.0001;

TEST_CASE("Plane Tests", "[Plane-tests]")
{
    SECTION("Plane SDF Tests")
    {
        Plane plane_0(Vec(0,0,0),Vec(0,1,0),Vec(255,0,0));
        REQUIRE(plane_0.SDF(origin_0) == 0);
        REQUIRE(plane_0.SDF(origin_1) == 0);
        REQUIRE(plane_0.SDF(origin_2) == 0);
        REQUIRE(Catch::Approx(plane_0.SDF(origin_3)).margin(tol) == 2);
    }

    SECTION("Plane Normal Tests")
    {

    }

}

TEST_CASE("Sphere Tests", "[sphere-tests]")
{
    SECTION("Sphere SDF Tests")
    {
        Sphere sphere_0(Vec(0,0,0),1, Vec()); // unit sphere at 0 origin
        REQUIRE(sphere_0.SDF(origin_0) == -1);
        REQUIRE(sphere_0.SDF(origin_1) == 4);
        REQUIRE(sphere_0.SDF(origin_2) == -0.5);
        REQUIRE(Catch::Approx(sphere_0.SDF(origin_3)).margin(tol) == 5.7082);

        Sphere sphere_1(Vec(0,0,0), 3, Vec()); // radius 3 sphere at 0 origin
        REQUIRE(sphere_1.SDF(origin_0) == -3);
        REQUIRE(sphere_1.SDF(origin_1) == 2);
        REQUIRE(sphere_1.SDF(origin_2) == -2.5);
        REQUIRE(Catch::Approx(sphere_1.SDF(origin_3)).margin(tol) == 3.7082);

        Sphere sphere_2(origin_3, 5, Vec()); // radius 5 sphere at origin_3
        REQUIRE(Catch::Approx(sphere_2.SDF(origin_0)).margin(tol) == origin_3.abs() - 5);
        REQUIRE(Catch::Approx(sphere_2.SDF(origin_1)).margin(tol) == 0.4772255);
        REQUIRE(Catch::Approx(sphere_2.SDF(origin_2)).margin(tol) == 1.422616);
        REQUIRE(sphere_2.SDF(origin_3) == -5);
    }

    SECTION("Sphere Normal Tests")
    {
        
    }
}