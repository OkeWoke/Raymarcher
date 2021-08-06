//
// Created by Liam Murphy on 6/08/21.
//

#include "Shading.h"


double softShadow(Vec lightRay, Vec pos, double minT, double maxT, double k, const Scene& scene)
{
    double res = 1;
    for(double t=minT; t<maxT;)
    {
        double dist = sceneSDF(scene, pos, nullptr);
        if (dist< 0.001) return 0;
        res = std::min(res, k*dist/t);
        t+=dist;
    }
    return res;
}

Vec Shade(Hit hit, const Scene& scene)
{
    return Vec(1,1,1)*hit.total_length;
    if(hit.total_length == MAX_LENGTH || hit.hit_obj == nullptr)
    {
        double t = 0.5*(hit.ray.y+1);
        return (1-t)*Vec(180, 180,180) + t*Vec(60, 60, 150);
    }


    Vec Light(3, 3, -8);
    Vec LightRay = (Light - hit.pos);
    double lightRayLength = LightRay.abs();
    LightRay = normalise(LightRay);
    Hit shadow_hit = RayMarch(hit.pos+0.001*LightRay, LightRay, scene, lightRayLength);
    //double shadow = softShadow(LightRay, hit.pos+EPSILON*LightRay, 0.01, lightRayLength, 2, scene);
    if (shadow_hit.hit_obj == nullptr ) return Vec(0,0,0);
    Vec normal = hit.hit_obj->getNormal(hit.pos-normalise(hit.ray)*2*EPSILON);

    return 3*clamp(normal.dot(LightRay), 0)*hit.hit_obj->color;//clamp(lightRayLength, 1);
}