//
// Created by Liam Murphy on 6/08/21.
//

#include "Shading.h"

double softShadow(const Vec& lightRay, const Vec& pos, const double minT, const double maxT, const double k, const Scene& scene)
{
    double res = 1;
    for(double t=minT; t<maxT;)
    {
        double dist = 6;//sceneSDF(scene, pos);
        if (dist< 0.001) return 0;
        res = std::min(res, k*dist/t);
        t+=dist;
    }
    return res;
}

Vec Shade(Hit& hit, const Scene& scene)
{
    //return Vec(1,1,1)*hit.total_length;
    if(hit.total_length == MAX_LENGTH || hit.hit_obj == nullptr)
    {
        double t = 0.5*(hit.ray.y+1);
        return (1-t)*Vec(180, 180,180) + t*Vec(60, 60, 150);
    }


    Vec Light(-3, 3, -10);
    Vec surface_pos = hit.pos - (EPSILON*hit.ray);
    Vec LightRay = (Light - surface_pos);
    double lightRayLength = LightRay.abs();
    LightRay = normalise(LightRay);
    Hit shadow_hit = RayMarch(surface_pos, LightRay, scene, lightRayLength);
    //double shadow = softShadow(LightRay, hit.pos+EPSILON*LightRay, 0.01, lightRayLength, 2, scene);
    if (shadow_hit.hit_obj != nullptr ) return Vec(0,0,0); //object is in shadow
    Vec normal = hit.hit_obj->getNormal(hit.pos);

    return normal.dot(LightRay)*hit.hit_obj->color;//clamp(lightRayLength, 1);
}