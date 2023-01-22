#include <stdint.h>

#include "collision.h"
#include "ringhopper/model_collision_geometry.h"
#include "ringhopper/scenario_structure_bsp.h"

uint32_t collision_bsp_leaf_for_point(ModelCollisionGeometryBSP *bsp, VectorXYZ *position, uint32_t node_index) {
    while((node_index & 0x80000000) == 0) {
        ModelCollisionGeometryBSP3DNode *node = &bsp->bsp3d_nodes.elements[node_index];
        Plane3D *plane = &bsp->planes.elements[node->plane].plane;

        float dot = plane->i * position->x + plane->j * position->y + plane->k * position->z - plane->w;
        if(dot > 0.0) {
            node_index = node->front_child;
        }
        else {
            node_index = node->back_child;
        }
    }

    // Out of the BSP
    if(node_index == 0xFFFFFFFF) {
        return node_index;
    }

    // Done
    return node_index & 0x7FFFFFFF;
}
