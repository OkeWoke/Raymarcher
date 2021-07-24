//
// Created by Liam Murphy on 24/07/21.
//
#include "IObject.h"

#ifndef RAYMARCHER_MANDELBULB_H
#define RAYMARCHER_MANDELBULB_H


class MandelBulb : public IObject
{
    public:
        MandelBulb();
        double SDF(Vec pos);
        Vec getNormal(Vec pos);
};


#endif //RAYMARCHER_MANDELBULB_H
