#pragma once

#include <cmath>

struct Vector2 {
    double x, y;

    inline Vector2() {}
    inline Vector2(double _x, double _y):
        x(_x), y(_y) {}

    inline ~Vector2() {}
    
    inline double magnitude() {
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    inline double magnitudeSquared() {
        return pow(magnitude(), 2);
    }

    inline double distance(Vector2& v) {
        return sqrt(pow(v.x - this->x, 2) + pow(v.y - this->y, 2));
    }

    inline Vector2& average(Vector2& v1, Vector2& v2, Vector2& v3, Vector2& v4) {
        double x = (v1.x + v2.x + v3.x + v4.x) / 4;
        double y = (v1.y + v2.y + v3.y + v4.y) / 4;
        Vector2 v(x, y);
        return v;
    }
};

inline Vector2 operator-(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}

inline Vector2 operator+(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}