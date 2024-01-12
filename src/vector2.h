#pragma once

#include <cmath>
#include <iostream>
struct Vector2
{
    double x, y;

    inline Vector2() {}
    inline Vector2(double _x, double _y) : x(_x), y(_y) {}

    inline ~Vector2() {}

    inline double magnitude()
    {
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    inline double magnitudeSquared()
    {
        return pow(magnitude(), 2);
    }

    inline double distance(Vector2& v)
    {
        return sqrt(pow(v.x - this->x, 2) + pow(v.y - this->y, 2));
    }

    inline void display() {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};

inline Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}

inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}

inline Vector2 operator+(const Vector2& v, const double scalar) {
    return Vector2(v.x + scalar, v.y + scalar);
}

inline Vector2 operator-(const Vector2& v, const double scalar) {
    return Vector2(v.x - scalar, v.y - scalar);
}