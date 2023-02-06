#ifndef DEMON__IMPL_MATH_MATH_H
#define DEMON__IMPL_MATH_MATH_H

struct VectorXYZ;

/**
 * Calculate base to the power of an exponent.
 *
 * @param base base
 * @param exp  exponent
 *
 * @return base to the power of the exponent
 */
double pow_float(double base, double exp);

/**
 * Calculate the distance squared between two points.
 *
 * Doing distance squared is preferred for speed for doing distance comparisons as it saves calculating a square root.
 *
 * @param a first vector
 * @param b second vector
 *
 * @return distance squared
 */
double vectorxyz_distance_squared(struct VectorXYZ *a, struct VectorXYZ *b);

#endif
