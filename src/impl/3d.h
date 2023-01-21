#ifndef DEMON__IMPL_3D_H
#define DEMON__IMPL_3D_H

#include "id.h"

typedef struct VectorXYZ {
    float x;
    float y;
    float z;
} VectorXYZ;

typedef struct VectorIJK {
    float i;
    float j;
    float k;
} VectorIJK;

typedef struct VectorPYR {
    float pitch;
    float yaw;
    float rotation;
} VectorPYR;

typedef struct ColorRGB {
    float r;
    float g;
    float b;
} ColorRGB;

#endif
