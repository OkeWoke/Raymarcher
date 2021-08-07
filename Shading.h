//
// Created by Liam Murphy on 6/08/21.
//
#include "CommonDefinitions.h"
#include "Raymarch.h"
#ifndef RAYMARCHER_SHADING_H
#define RAYMARCHER_SHADING_H

double softShadow(const Vec& lightRay, const Vec& pos, const double minT, const double maxT, const double k, const Scene& scene);
Vec Shade(Hit& hit, const Scene& scene);

#endif //RAYMARCHER_SHADING_H
