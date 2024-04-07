#pragma once

#include <helper_3dmath.h>

struct Vector3f : VectorFloat  {
    float* array() { return &x; }

    Vector3f& add(const Vector3f& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3f& sub(const Vector3f& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3f& scale(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3f& set(const Vector3f& other){
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    void clear() {
        x = 0;
        y = 0;
        z = 0;
    }
};

struct Vector3i16 : VectorInt16 {

};