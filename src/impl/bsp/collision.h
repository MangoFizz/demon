#ifndef DEMON__IMPL_BSP_COLLISION_H
#define DEMON__IMPL_BSP_COLLISION_H

#include "../3d.h"

// TODO: Use tag defs for this!!!
typedef struct CollisionBSP CollisionBSP;

/**
 * Traverse the collision BSP to determine the leaf for a given point.
 *
 * @return leaf index if found, or 0xFFFFFFFF if the point falls outside of the BSP
 */
uint32_t collision_bsp_leaf_for_point(CollisionBSP *bsp, VectorXYZ *position, uint32_t node_index);

#endif
