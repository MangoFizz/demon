#include <stdint.h>
#include <stdbool.h>

typedef struct BSP3DNode {
    uint32_t plane;

    // if signed bit is set, refers to a leaf instead
    uint32_t back_child;
    uint32_t front_child;
} BSP3DNode;

typedef struct Plane {
    float i;
    float j;
    float k;
    float w;
} Plane;

// TODO: use tag structs
typedef struct CollisionBSP {
    struct {
        uint32_t count;
        BSP3DNode *elements;
        char padding[4];
    } bsp3d_nodes;
    struct {
        uint32_t count;
        Plane *elements;
        char padding[4];
    } planes;
} CollisionBSP;

typedef struct VectorXYZ {
    float x;
    float y;
    float z;
} VectorXYZ;

/**
 * Traverse the collision BSP to determine the leaf for a given point.
 *
 * @return leaf index if found, or 0xFFFFFFFF if the point falls outside of the BSP
 */
uint32_t collision_bsp_leaf_for_point(CollisionBSP *bsp, VectorXYZ *position, uint32_t node_index) {
    while((node_index & 0x80000000) == 0) {
        BSP3DNode *node = &bsp->bsp3d_nodes.elements[node_index];
        Plane *plane = &bsp->planes.elements[node->plane];

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
