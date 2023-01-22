#include <stdint.h>

#include "sky.h"
#include "collision.h"
#include "../id.h"
#include "../tag.h"

void **loaded_bsp_data = (void **)(0x7367BC);

uint32_t *current_leaf_index = (uint32_t *)(0x7B27E4);
uint32_t *current_cluster_index = (uint32_t *)(0x7B27E8);
uint16_t *current_sky_index = (uint16_t *)(0x7B27EE);
uint8_t *should_draw_skybox = (uint8_t *)(0x7B27ED);

void set_skybox_info(VectorXYZ *point) {
    // TODO: USE STRUUUUUUUUUUUCTS HERE
    CollisionBSP *collision_bsp_addr = *(CollisionBSP **)(*loaded_bsp_data + 0xB0 + 0x4);

    // These parameters are safe.
    *current_leaf_index = collision_bsp_leaf_for_point(collision_bsp_addr, point, 0);
    *current_cluster_index = 0xFFFFFFFF;
    *current_sky_index = 0xFFFF;
    *should_draw_skybox = 0;

    // No leaf? Do nothing then.
    if(*current_leaf_index == 0xFFFFFFFF) {
        return;
    }

    // Resolve the current cluster index. Again should use structs later with Invader's definitions
    *current_cluster_index = bsp_cluster_for_leaf(*current_leaf_index);

    // Resolve the sky index.
    void *bsp = *loaded_bsp_data;
    void *clusters = *(void **)(bsp + 0x134 + 0x4);

    *current_sky_index = *(uint16_t *)(clusters + *current_cluster_index * 0x68);

    // Get the scenario data
    void *scenario_data = get_scenario_tag_data();
    uint32_t sky_count = *(uint32_t *)(scenario_data + 0x30);

    // Try to resolve the sky index
    if(*current_sky_index >= sky_count) {
        return;
    }

    void *skies = *(void **)(scenario_data + 0x30 + 4);
    void *sky = skies + *current_sky_index * 0x10;
    TableID sky_tag_id = *(TableID *)(sky + 0xC);

    // Check if null
    // (the game also checks if the sky index is signed, but that's insane, plus it'd short circuit here)
    if(ID_IS_NULL(sky_tag_id)) {
        return;
    }

    // Check if skybox model is null
    TableID sky_model = *(TableID *)(get_tag_data(sky_tag_id) + 0xC);
    if(ID_IS_NULL(sky_model)) {
        return;
    }

    *should_draw_skybox = 1;
}

uint32_t bsp_cluster_for_leaf(uint32_t leaf) {
    if(leaf == 0xFFFFFFFF) {
        return 0xFFFFFFFF;
    }

    // TODO: USE STRUCTS
    uint32_t leaf_count = *(uint32_t *)(*loaded_bsp_data + 0xE0);
    if(leaf >= leaf_count) {
        return 0xFFFFFFFF;
    }
    void *leaves = *(void **)(*loaded_bsp_data + 0xE0 + 0x4);
    void *leaf_data = leaves + leaf * 0x10;
    return *(uint16_t *)(leaf_data + 0x8);
}
