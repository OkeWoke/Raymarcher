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
    double epsilon = 0.01;
    Vec normal(0,0,0);

    normal.x = this->SDF(Vec(pos.x+epsilon, pos.y, pos.z)) - this->SDF(Vec(pos.x-epsilon, pos.y, pos.z));
    normal.y = this->SDF(Vec(pos.x, pos.y+epsilon, pos.z)) - this->SDF(Vec(pos.x, pos.y-epsilon, pos.z));
    normal.z = this->SDF(Vec(pos.x, pos.y, pos.z+epsilon)) - this->SDF(Vec(pos.x, pos.y, pos.z-epsilon));

    return normal;
}

double MandelBulb::SDF(Vec pos)
{
    Vec z = pos;
    double dr = 1.0;
    double r = 0.0;
    int iterations = 15;
    int power = 8;

    for (int i = 0; i < iterations; i++)
    {
        r = z.abs();

        if (r>2) break;

        // convert to polar coordinates
        double theta = acos(z.z/r);
        double phi = atan2(z.y,z.x);
        dr =  pow( r, power-1.0)*power*dr + 1.0;

        // scale and rotate the point
        double zr = pow( r,power);
        theta = theta*power;
        phi = phi*power;

        // convert back to cartesian coordinates
        z = zr*Vec(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
        z+=pos;
    }
    double dst = 0.5*log(r)*r/dr;
    return dst;// iterations;
    /** old
    Vec w = pos;
    double dr = 0;
    double wr = 0.0;
    int Iterations = 10;
    int Power = 8;
    double Bailout = 2;

    for (int i = 0; i < Iterations ; i++)
    {
        // convert to polar coordinates
        wr = w.abs();
        if (wr>Bailout) break;
        double theta = acos(w.y/wr);
        double phi = atan(w.x/w.z);

        //update derivative
        dr =  pow( wr, Power-1.0)*Power*dr + 1.0;

        // scale and rotate the point
        wr = pow(wr, Power);
        theta = theta * Power;
        phi = phi * Power;

        // convert back to cartesian coordinates
        w.x = wr * sin(theta) * sin(phi);
        w.y = wr * cos(theta);
        w.z = wr * sin(theta) * cos(phi);

        w+=pos;
    }
    if (dr == 0) dr =1;
    return log(wr)*wr/(dr);**/
}