//
// Created by Liam Murphy on 23/07/21.
//

#include "Vec.h"
#include<iostream>

Vec::Vec():
        x(0),
        y(0),
        z(0)
{
}

Vec::Vec(double x, double y, double z):
        x(x),
        y(y),
        z(z)
{
}

Vec::Vec(const Vec& RHS)
{
    this->x = RHS.x;
    this->y = RHS.y;
    this->z = RHS.z;
}

Vec& Vec::operator=(const Vec& RHS)
{
    this->x=RHS.x;
    this->y=RHS.y;
    this->z=RHS.z;

    return *this;
}

Vec& Vec::operator+=(const Vec& RHS)
{
    this->x+=RHS.x;
    this->y+=RHS.y;
    this->z+=RHS.z;

    return *this;
}

Vec& Vec::operator-=(const Vec& RHS)
{
    this->x-=RHS.x;
    this->y-=RHS.y;
    this->z-=RHS.z;

    return *this;
}

Vec& Vec::operator*=(double RHS)
{
    this->x*=RHS;
    this->y*=RHS;
    this->z*=RHS;

    return *this;
}

Vec& Vec::operator/=(double RHS)
{
    this->x/=RHS;
    this->y/=RHS;
    this->z/=RHS;

    return *this;
}

double Vec::dot(Vec v) const
{
    return this->x*v.x + this->y*v.y + this->z*v.z;
}

Vec operator+(const Vec& LHS, const Vec& RHS)
{
    Vec result = LHS;
    result+=RHS;

    return result;
}

Vec operator-(const Vec& LHS, const Vec& RHS)
{
    Vec result = LHS;
    result-=RHS;

    return result;
}

Vec operator*(const Vec& LHS, double RHS)
{
    Vec result = LHS;
    result*=RHS;

    return result;
}

Vec operator/(const Vec& LHS, double RHS)
{
    Vec result = LHS;
    result/=RHS;

    return result;
}

Vec operator*(double LHS, const Vec& RHS)
{
    Vec result = RHS;
    result*=LHS;

    return result;
}

Vec operator%(const Vec& LHS, const Vec& RHS)
{
    return Vec(LHS.y*RHS.z - LHS.z*RHS.y, LHS.z*RHS.x - LHS.x*RHS.z, LHS.x*RHS.y - LHS.y*RHS.x);
}

bool operator==(const Vec& LHS, const Vec& RHS)
{
    return (LHS.x==RHS.x && LHS.y==RHS.y && LHS.z==RHS.z);
}

double Vec::abs() const
{
    return sqrt(x*x + y*y + z*z);
}

Vec normalise(const Vec& vec)
{
    return (vec/vec.abs());
}

Vec Vec::cross(const Vec& vec)
{
    return Vec(this->y*vec.z - this->z*vec.y, this->z*vec.x - this->x*vec.z, this->x*vec.y - this->y*vec.x);
}

std::string Vec::to_string() const
{
    std::ostringstream oss;
    oss << "(" << this->x << ", " << this->y <<", " << this->z << ")";
    return oss.str();
}

