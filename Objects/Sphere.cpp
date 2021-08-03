//
// Created by Liam Murphy on 23/07/21.
//

#include "Sphere.h"

Sphere::Sphere() : IObject(Vec(), Vec(255, 0, 0)), radius(5)
{
}

Sphere::Sphere(Vec p, double radius, Vec c) : IObject(p, c), radius(radius)
{
}

double Sphere::SDF(Vec p)
{
    return (p-this->position).abs() - this->radius;
}

Vec Sphere::getNormal(Vec p)
{
    return normalise(p-this->position);
}