#include <Catch2\catch_all.hpp>
#include <catch2\catch_approx.hpp>
#include "Vec.h"


TEST_CASE("Vector Operation Tests", "[vector-operations]")
{
	Vec zero(0, 0, 0);
	REQUIRE(zero.x == 0);
	REQUIRE(zero.y == 0);
	REQUIRE(zero.z == 0);

	Vec one(1, 1, 1);
	Vec vecA(25, 23, -43);
	Vec vecB(3, 5, 9);
	double tol = 0.0001;

	SECTION("Addition")
	{
		Vec result(26, 24, -42);
		REQUIRE(one + vecA == result);
	}
	
	SECTION("Subtraction")
	{
		Vec result(-24, -22, 44);
		REQUIRE(one - vecA == result);
	}

	SECTION("Division")
	{
		Vec result(58.004640371, 53.36426914153, -99.7679814385);
		double x = 0.431;
		Vec tmp = vecA / x;
	
		REQUIRE(Catch::Approx(tmp.x).margin(tol) == result.x);
		REQUIRE(Catch::Approx(tmp.y).margin(tol) == result.y);
		REQUIRE(Catch::Approx(tmp.z).margin(tol) == result.z);
	}


	SECTION("Multiplication")
	{
		Vec result(10.775, 9.913, -18.533);
		double x = 0.431;
		Vec tmp = vecA * x;

		REQUIRE(Catch::Approx(tmp.x).margin(tol) == result.x);
		REQUIRE(Catch::Approx(tmp.y).margin(tol) == result.y);
		REQUIRE(Catch::Approx(tmp.z).margin(tol) == result.z);
	}

}