//
// Created by Liam Murphy on 24/07/21.
//

#include "MandelBulb.h"
#include <cmath>

MandelBulb::MandelBulb(): IObject(Vec(), Vec(255, 0, 0))
{

}

Vec MandelBulb::getNormal(const Vec& pos)
{
    double epsilon = 0.01;
    Vec normal(0,0,0);

    normal.x = this->SDF(Vec(pos.x+epsilon, pos.y, pos.z)) - this->SDF(Vec(pos.x-epsilon, pos.y, pos.z));
    normal.y = this->SDF(Vec(pos.x, pos.y+epsilon, pos.z)) - this->SDF(Vec(pos.x, pos.y-epsilon, pos.z));
    normal.z = this->SDF(Vec(pos.x, pos.y, pos.z+epsilon)) - this->SDF(Vec(pos.x, pos.y, pos.z-epsilon));

    return normal;
}

double MandelBulb::SDF(const Vec& pos)
{
    //Implementation taken from: http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-v-the-mandelbulb-different-de-approximations/

    Vec z = pos;
    double dr = 1.0;
    double r = 0.0;
    int iterations = 200;
    int power = 3;

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

    return dst;
}