#ifndef DEMON__IMPL_3D_H
#define DEMON__IMPL_3D_H

#include <stdint.h>

typedef struct VectorXYZ {
    float x;
    float y;
    float z;
} VectorXYZ;
_Static_assert(sizeof(VectorXYZ) == 0xC);

typedef struct VectorXY {
    float x;
    float y;
} VectorXY;
_Static_assert(sizeof(VectorXY) == 0x8);

typedef struct VectorIJK {
    float i;
    float j;
    float k;
} VectorIJK;
_Static_assert(sizeof(VectorIJK) == 0xC);

typedef struct Quaternion {
    float i;
    float j;
    float k;
    float l;
} Quaternion;
_Static_assert(sizeof(Quaternion) == 0x10);

typedef struct VectorPYR {
    float pitch;
    float yaw;
    float rotation;
} VectorPYR;
_Static_assert(sizeof(VectorPYR) == 0xC);

typedef struct VectorPY {
    float pitch;
    float yaw;
} VectorPY;
_Static_assert(sizeof(VectorPY) == 0x8);

typedef struct Rectangle2D {
    uint16_t top;
    uint16_t left;
    uint16_t bottom;
    uint16_t right;
} Rectangle2D;
_Static_assert(sizeof(Rectangle2D) == 0x8);

typedef struct Plane2D {
    float i;
    float j;
    float w;
} Plane2D;
_Static_assert(sizeof(Plane2D) == 0xC);

typedef struct Plane3D {
    float i;
    float j;
    float k;
    float w;
} Plane3D;
_Static_assert(sizeof(Plane3D) == 0x10);

/**
 * Calculate the distance squared between two points.
 *
 * Doing distance squared is preferred for speed for doing distance comparisons.
 *
 * @param a first vector
 * @param b second vector
 *
 * @return distance squared
 */
double vectorxyz_distance_squared(VectorXYZ *a, VectorXYZ *b);

#endif
