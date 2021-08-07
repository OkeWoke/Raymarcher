//
// Created by Liam Murphy on 6/08/21.
//
#include "CommonDefinitions.h"

#ifndef RAYMARCHER_RAYMARCH_H
#define RAYMARCHER_RAYMARCH_H

Hit RayMarch(const Vec& origin, const Vec& ray, const Scene& scene, const double max_length);
#endif //RAYMARCHER_RAYMARCH_H
