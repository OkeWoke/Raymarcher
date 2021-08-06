//
// Created by Liam Murphy on 6/08/21.
//
#include <valarray>
#include <memory>
#include <vector>
#include "IObject.h"
#include "Vec.h"

#ifndef RAYMARCHER_COMMONDEFINITIONS_H
#define RAYMARCHER_COMMONDEFINITIONS_H

typedef std::valarray<double> imBufDouble;
typedef std::valarray<uint8_t> imBufuInt;
typedef std::vector<std::shared_ptr<IObject>> Scene;

const unsigned int MAX_STEPS = 255;
const double MAX_LENGTH = 100;
const double EPSILON = 0.01;
const unsigned int THREADS_TO_USE = 24;

struct Camera
{
    Vec position;
    Vec u, v, n;
    unsigned int X_RES;
    unsigned int Y_RES;
    double WIDTH;
    double HEIGHT;
    double FL;
};

struct Hit
{
    int steps;
    double total_length;
    Vec ray;
    Vec pos;
    std::shared_ptr<IObject> hit_obj = nullptr;
};

double clamp(double val, double min);
#endif //RAYMARCHER_COMMONDEFINITIONS_H
