//
// Created by Liam Murphy on 23/07/21.
//
#include "IObject.h"

#ifndef RAYMARCHER_SPHERE_H
#define RAYMARCHER_SPHERE_H


class Sphere : public IObject
{
    public:
        Sphere();
        Sphere(Vec p, double radius, Vec color);
        double SDF(Vec p);
        Vec getNormal(Vec p);

    private:
        double radius;
};


#endif //RAYMARCHER_SPHERE_H
