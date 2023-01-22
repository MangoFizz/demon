#ifndef DEMON__IMPL_BSP_SKY_H
#define DEMON__IMPL_BSP_SKY_H

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
 */
void set_skybox_info(VectorXYZ *point);

#endif
