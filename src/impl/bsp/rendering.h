#ifndef DEMON__IMPL_BSP_RENDERING_H
#define DEMON__IMPL_BSP_RENDERING_H

#include "../3d.h"

/**
 * Resolve a cluster index from a leaf.
 *
 * @param leaf leaf index to check
 *
 * @return cluster index or 0xFFFFFFFF if none
 */
uint32_t bsp_cluster_for_leaf(uint32_t leaf);

/**
 * Set the current skybox information.
 *
 * @param point point to test
 */
void set_skybox_info(const VectorXYZ *point);

/**
 * Currently unknown.
 *
 * @param input  some buffer
 * @param output array of four floats
 */
void unknown_function_5092f0(const void *input, float *output);

#endif
