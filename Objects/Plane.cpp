//
// Created by Liam Murphy on 23/07/21.
//

#include "Plane.h"

Plane::Plane() : IObject(Vec(), Vec(0,255,0)), normal(Vec(0,1,0))
{

}

Plane::Plane(Vec p, Vec n, Vec c) : IObject(p, c)
{
    this->normal = normalise(n);
}

double Plane::SDF(Vec p)
{
    return -1*(this->position-p).dot(this->normal);
}

Vec Plane::getNormal(Vec p)
{
    return this->normal;
}