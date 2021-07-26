//
// Created by Liam Murphy on 24/07/21.
//

#include "MandelBulb.h"
#include <cmath>

MandelBulb::MandelBulb(): IObject(Vec(), Vec(255, 0, 0))
{

}

Vec MandelBulb::getNormal(Vec pos)
{
    return Vec();
}

double MandelBulb::SDF(Vec pos)
{
    Vec z = pos;
    float dr = 1.0;
    float r = 0.0;
    int Iterations = 10;
    int Power = 8;
    double Bailout = 2;
    for (int i = 0; i < Iterations ; i++) {
        r = z.abs();
        if (r>Bailout) break;

        // convert to polar coordinates
        float theta = acos(z.z/r);
        float phi = atan(z.y/z.x);
        dr =  pow( r, Power-1.0)*Power*dr + 1.0;

        // scale and rotate the point
        float zr = pow( r,Power);
        theta = theta*Power;
        phi = phi*Power;

        // convert back to cartesian coordinates
        z = zr*Vec(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
        z+=pos;
    }
    return 0.5*log(r)*r/dr;
}