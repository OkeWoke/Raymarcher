//
// Created by Liam Murphy on 6/08/21.
//
#include "CommonDefinitions.h"
#include "Raymarch.h"
#ifndef RAYMARCHER_SHADING_H
#define RAYMARCHER_SHADING_H

double softShadow(Vec lightRay, Vec pos, double minT, double maxT, double k, const Scene& scene);
Vec Shade(Hit hit, const Scene& scene);

#endif //RAYMARCHER_SHADING_H
