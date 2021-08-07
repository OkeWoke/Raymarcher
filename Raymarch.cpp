//
// Created by Liam Murphy on 6/08/21.
//

#include "Raymarch.h"


Hit RayMarch(const Vec& origin, const Vec& ray, const Scene& scene, const double max_length)
{
    Vec pos = origin;
    double total_length=0;
    Hit hit;
    unsigned int i;

    for(i=0; i < MAX_STEPS; i++)
    {
        double min_dist= 9e19;

        for(std::shared_ptr<IObject> obj: scene)
        {
            double dist = obj->SDF(pos);
            if (dist < min_dist)
            {
                min_dist = dist;
                if(min_dist < EPSILON)   hit.hit_obj = obj; //We've hit something.
            }
        }

        if (hit.hit_obj != nullptr) break;        //We've hit something.

        total_length+=min_dist;
        pos = pos + ray*min_dist;
        if (total_length > max_length)
        {
            total_length = max_length;
            break;
        }
    }

    hit.ray = ray;
    hit.pos = pos;
    hit.total_length = total_length;
    hit.steps = i;

    return hit;
}


