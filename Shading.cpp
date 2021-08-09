//
// Created by Liam Murphy on 6/08/21.
//

#include "Shading.h"

double softShadow(const Vec& lightRay, const Vec& pos, const double minT, const double maxT, const double k, const Scene& scene)
{
    double res = 1.0;
    float ph = 1e20;
    for(double t=minT; t<maxT;)
    {
        double min_dist = 9e19;
        for(std::shared_ptr<IObject> obj: scene)
        {
            double dist = obj->SDF(pos + lightRay*t);
            if (dist < min_dist)
            {
                min_dist = dist;
            }
        }

        if (min_dist< EPSILON) return 0;
        double y = min_dist*min_dist/(2.0*ph);
        double d = sqrt(min_dist*min_dist-y*y);
        res = std::min(res, k*d/std::max(0.0,t-y));
        ph = min_dist;
        t+=min_dist;
    }
    return res;
}

Vec Shade(Hit& hit, const Scene& scene)
{
    //return Vec(1,1,1)*hit.total_length;
    if(hit.total_length == MAX_LENGTH || hit.hit_obj == nullptr)
    {
        double t = 0.5*(hit.ray.y+1);
        return Vec(0,0,0);// (1-t)*Vec(180, 180,180) + t*Vec(60, 60, 150);
    }


    Vec Light(-3, 3, -15);
    Vec surface_pos = hit.pos - (EPSILON*hit.ray);
    Vec LightRay = (Light - surface_pos);
    double lightRayLength = LightRay.abs();
    LightRay = normalise(LightRay);
    //Hit shadow_hit = RayMarch(surface_pos, LightRay, scene, lightRayLength);
    double shadow = softShadow(LightRay, surface_pos, 0, lightRayLength, 1, scene);
    //if (shadow_hit.hit_obj != nullptr ) return Vec(0,0,0); //object is in shadow
    Vec normal = hit.hit_obj->getNormal(hit.pos);

    return shadow*5*normal.dot(LightRay)*hit.hit_obj->color;//+(Vec(0,0,0.01)*hit.total_length);//clamp(lightRayLength, 1);
}