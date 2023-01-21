#ifndef DEMON__IMPL_3D_H
#define DEMON__IMPL_3D_H

typedef struct VectorXYZ {
    float x;
    float y;
    float z;
} VectorXYZ;
_Static_assert(sizeof(VectorXYZ) == 0xC);

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

typedef struct VectorIJK {
    float i;
    float j;
    float k;
} VectorIJK;
_Static_assert(sizeof(VectorIJK) == 0xC);

typedef struct VectorPYR {
    float pitch;
    float yaw;
    float rotation;
} VectorPYR;
_Static_assert(sizeof(VectorPYR) == 0xC);

typedef struct ColorRGB {
    float r;
    float g;
    float b;
} ColorRGB;
_Static_assert(sizeof(ColorRGB) == 0xC);

typedef struct Plane {
    float i;
    float j;
    float k;
    float w;
} Plane;
_Static_assert(sizeof(Plane) == 0x10);

#endif
