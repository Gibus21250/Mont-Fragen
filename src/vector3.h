#pragma once

#include <cmath>

struct Vector3 {
    double x, y, z;

    inline Vector3() {}
    inline Vector3(double _x, double _y, double _z):
        x(_x), y(_y), z(_z) {}
    
    inline double magnitude() {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    inline double magnitudeSquared() {
        return pow(magnitude(), 2);
    }
    
    inline ~Vector3() {}
};

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}