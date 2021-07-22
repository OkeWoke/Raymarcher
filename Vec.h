//
// Created by Liam Murphy on 23/07/21.
//

#include<string>
#include <sstream>
#include <math.h>

#ifndef RAYMARCHER_VEC_H
#define RAYMARCHER_VEC_H

class Vec
{
    public:
        Vec();
        Vec(double x, double y, double z);
        Vec(const Vec& RHS);

        Vec& operator=(const Vec& RHS);
        Vec& operator+=(const Vec& RHS);
        Vec& operator-=(const Vec& RHS);
        Vec& operator*=(double RHS);
        Vec& operator/=(double RHS);

        double abs() const;
        double dot (Vec V) const;

        std::string to_string() const;

        double x;
        double y;
        double z;

        Vec cross(const Vec& vec);
};

bool operator==(const Vec& LHS, const Vec& RHS);
Vec operator+(const Vec& LHS, const Vec& RHS);
Vec operator-(const Vec& LHS, const Vec& RHS);
Vec operator*(const Vec& LHS, double RHS);
Vec operator*(double LHS, const Vec& RHS);
Vec operator/ (const Vec& LHS, double RHS);
Vec operator%(const Vec& LHS, const Vec& RHS);
Vec normalise(const Vec& vec);

#endif //RAYMARCHER_VEC_H
