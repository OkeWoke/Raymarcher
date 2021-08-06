//
// Created by Liam Murphy on 6/08/21.
//

#include "Raymarch.h"

double sceneSDF(const Scene& scene, Vec pos, std::shared_ptr<Hit> hit)
{
    double min_dist= 9e19;

    for(std::shared_ptr<IObject> obj: scene)
    {
        double dist = obj->SDF(pos);
        if (dist < min_dist) min_dist = dist;
        if (dist < EPSILON)
        {
            if(hit != nullptr) hit->hit_obj = obj;
            break;
        }
    }

    return min_dist;
}

Hit RayMarch(const Vec& origin, const Vec& ray, const Scene& scene, double max_length)
{
    Vec pos = origin;
    double total_length=0;
    Hit hit;
    unsigned int i;

    for(i=0; i < MAX_STEPS; i++)
    {
        if (total_length > max_length)
        {
            total_length = max_length;
            break;
        }

        double min_dist = sceneSDF(scene, pos, std::make_shared<Hit>(hit));
        total_length+=min_dist;
        pos = pos + ray*min_dist;
    }

    hit.ray = ray;
    hit.pos = pos;
    hit.total_length = total_length;
    hit.steps = i;
    return hit;
}


