#pragma once

#include <helper_3dmath.h>

struct Vector3f : VectorFloat  {
    float* array() { return &x; }
};

struct Vector3i16 : VectorInt16 {

};