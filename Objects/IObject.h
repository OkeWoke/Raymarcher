//
// Created by Liam Murphy on 23/07/21.
//
#include "Vec.h"
#ifndef RAYMARCHER_IOBJECT_H
#define RAYMARCHER_IOBJECT_H


class IObject {
    public:
        Vec color;
        Vec position;
        IObject();
        IObject(Vec p, Vec c) : position(p), color(c){};

        virtual double SDF(Vec p) = 0;
        virtual Vec getNormal(Vec p) = 0;

};

#endif //RAYMARCHER_IOBJECT_H
