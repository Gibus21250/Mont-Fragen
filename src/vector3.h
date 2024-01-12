#pragma once

#include <cmath>

#include "vector2.h"

struct Vector3 {
    double x, y, z;

    inline Vector3() {}
    inline Vector3(double _x, double _y, double _z):
        x(_x), y(_y), z(_z) {}
    
    inline Vector3(Vector2& v, double _z) {
        x = v.x; y = v.y; z=_z;
    }

    inline Vector3(Vector2& v) {
        x = v.x; y = v.y; z=0;
    }

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
};

inline Vector3 average(Vector3& v1, Vector3& v2, Vector3& v3, Vector3& v4) {
    double x = (v1.x + v2.x + v3.x + v4.x) / 4;
    double y = (v1.y + v2.y + v3.y + v4.y) / 4;
    double z = (v1.z + v2.z + v3.z + v4.z) / 4;
    Vector3 v(x, y, z);
    return v;
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}