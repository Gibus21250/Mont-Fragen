#pragma once

#include <cmath>
#include <iostream>

#include "vector2.h"

struct Vector3 {
    double x, y, z;

    inline Vector3() {}
    inline Vector3(double _x, double _y, double _z):
        x(_x), y(_y), z(_z) {}
    
    inline Vector3(double _x, double _y): x(_x), y(_y) {}

    inline ~Vector3() {}
    
    inline double magnitude() {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    inline double magnitudeSquared() {
        return pow(magnitude(), 2);
    }

    inline double distance(Vector3& v) {
        return sqrt(pow(v.x - this->x, 2) + pow(v.y - this->y, 2) + pow(v.z - this->z, 2));
    }

    inline void display() {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vector3 operator+(const Vector3& v, const double scalar) {
    return Vector3(v.x + scalar, v.y + scalar, v.z + scalar);
}

inline Vector3 operator-(const Vector3& v, const double scalar) {
    return Vector3(v.x - scalar, v.y - scalar, v.z - scalar);
}