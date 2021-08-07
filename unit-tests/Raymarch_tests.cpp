//
// Created by Liam Murphy on 6/08/21.
//

#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include "Raymarch.h"
#include "CommonDefinitions.h"
#include "Objects/Sphere.h"
#include "Objects/Plane.h"

class Raymarch_Test_Fixture
{
protected:
    Scene populateScene()
    {
        Scene scene;
        std::shared_ptr<IObject> sphere_0 = std::make_shared<Sphere>(Vec(0,0,5), 2.5, Vec(200, 0, 30));
        std::shared_ptr<IObject> plane_0 = std::make_shared<Plane>(Vec(0,-1, 0), Vec(0, 1, 0), Vec(0, 100, 30));
        scene.push_back(std::move(sphere_0));
        scene.push_back(std::move(plane_0));

        return scene;
    }
};

TEST_CASE_METHOD(Raymarch_Test_Fixture, "Raymarch Tests", "[raymarch-tests]")
{
    Scene scene = populateScene();
    Vec origin(0,0,0);
    Vec ray(0,0, 1);
    Hit hit = RayMarch(origin, ray, scene, 10);
    REQUIRE(hit.total_length == 2.5);
    REQUIRE(hit.hit_obj == scene[0]);
    REQUIRE(hit.steps == 3);

    Vec ray2(0,0, -1);
    hit = RayMarch(origin, ray2, scene, 10 );
    REQUIRE(hit.total_length == 10);
    REQUIRE(hit.hit_obj == nullptr);
    REQUIRE(hit.steps == 10);

    Vec ray3(0,-1, 0);
    hit = RayMarch(origin, ray3, scene, 10);
    REQUIRE(hit.total_length == 1);
    REQUIRE(hit.hit_obj == scene[1]);
    REQUIRE(hit.steps == 1);

    Vec origin_2(0,0,2.4999);
    hit = RayMarch(origin_2, ray, scene, 10);
    REQUIRE(hit.hit_obj== scene[0]);
    REQUIRE(hit.total_length == 0);
    REQUIRE(hit.steps == 0);

    Vec origin_3(0,-0.999, 0);
    hit = RayMarch(origin_3, ray, scene, 10);
    REQUIRE(hit.hit_obj== scene[1]);
    REQUIRE(hit.total_length == 0);
    REQUIRE(hit.steps == 0);

    hit = RayMarch(origin, ray, scene, 1);
    REQUIRE(hit.total_length == 1);
    REQUIRE(hit.hit_obj == nullptr);
    REQUIRE(hit.steps == 1);
}