//
// Created by Liam Murphy on 6/08/21.
//
#include "CommonDefinitions.h"

#ifndef RAYMARCHER_RAYMARCH_H
#define RAYMARCHER_RAYMARCH_H

Hit RayMarch(const Vec& origin, const Vec& ray, const Scene& scene, double max_length);
double sceneSDF(const Scene& scene, Vec pos, std::shared_ptr<Hit> hit);
#endif //RAYMARCHER_RAYMARCH_H
