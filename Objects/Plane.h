//
// Created by Liam Murphy on 23/07/21.
//
#include "IObject.h"

#ifndef RAYMARCHER_PLANE_H
#define RAYMARCHER_PLANE_H


class Plane : public IObject
{
    public:
        Vec normal;
        Plane();
        Plane(Vec p, Vec n, Vec c);
        double SDF(Vec p);
        Vec getNormal(Vec p);
};


#endif //RAYMARCHER_PLANE_H
