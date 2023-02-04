#include <stdint.h>
#include <math.h>

#include "ringhopper/scenario.h"
#include "ringhopper/scenario_structure_bsp.h"
#include "ringhopper/sky.h"

#include "rendering.h"
#include "collision.h"
#include "loading.h"

#include "../console/console.h"
#include "../table/table.h"
#include "../tag.h"

uint32_t *current_leaf_index = (uint32_t *)(0x7B27E4);
uint32_t *current_cluster_index = (uint32_t *)(0x7B27E8);
uint16_t *current_sky_index = (uint16_t *)(0x7B27EE);
uint8_t *should_draw_skybox = (uint8_t *)(0x7B27ED);

void set_skybox_info(const VectorXYZ *point) {
    // Get the loaded BSP.
    ScenarioStructureBSP *loaded_bsp_data = get_loaded_bsp_tag_data();

    // Find the leaf we're in. If our leaf is not null, we can use it.
    //
    // Otherwise, if it is null, we can use it only if our last leaf was invalid for our *CURRENT* BSP. This is
    // SUUUUUUUUUUUUPER hacky, but the whole BSP and skybox disappear if the camera goes out of bounds if you don't do
    // this in this exact way, and the camera goes out of bounds on several cutscenes. -.-'
    uint32_t new_leaf_index = collision_bsp_leaf_for_point(loaded_bsp_data->collision_bsp.elements, point, 0);

    if(new_leaf_index != 0xFFFFFFFF || *current_leaf_index >= loaded_bsp_data->leaves.count) {
        *current_leaf_index = new_leaf_index;
    }

    // These parameters are safe.
    *current_sky_index = 0xFFFF;
    *should_draw_skybox = 0;

    // Resolve the current cluster index too.
    *current_cluster_index = bsp_cluster_for_leaf(*current_leaf_index);

    // No cluster? Do nothing then.
    if(*current_cluster_index == 0xFFFFFFFF) {
        return;
    }

    // Get the current sky index.
    *current_sky_index = loaded_bsp_data->clusters.elements[*current_cluster_index].sky;

    // Special case for an index of 65535: it uses 'indoor' fog.
    if(*current_sky_index == 0xFFFF) {
        return;
    }

    // Get the scenario data
    Scenario *scenario_data = get_scenario_tag_data();

    // Otherwise, if this is an invalid skybox, it will just use no skybox (black) and no fog, which shouldn't even happen on valid tag data.
    if(*current_sky_index >= scenario_data->skies.count) {
        console_printf_debug_err("Cluster #%u references sky #%u but there are only %zu sky(s)", *current_cluster_index, *current_sky_index, scenario_data->skies.count);
        return;
    }

    TableID sky_tag_id = scenario_data->skies.elements[*current_sky_index].sky.tag_id;

    // Check if null
    // (the game also checks if the sky index is signed, but that's insane, plus it'd short circuit here)
    if(ID_IS_NULL(sky_tag_id)) {
        return;
    }

    // Check if skybox model is null
    if(ID_IS_NULL(((Sky *)(get_tag_data(sky_tag_id)))->model.tag_id)) {
        return;
    }

    *should_draw_skybox = 1;
}

uint32_t bsp_cluster_for_leaf(uint32_t leaf) {
    // Null leaf = null cluster
    if(leaf == 0xFFFFFFFF) {
        return 0xFFFFFFFF;
    }

    // It's worth noting that this check is NOT necessary if the tag data is valid.
    ScenarioStructureBSP *loaded_bsp_data = get_loaded_bsp_tag_data();
    if(leaf >= loaded_bsp_data->leaves.count) {
        console_printf_debug_err("Can't get a BSP cluster for leaf #%u when there are only %u leaves", leaf, loaded_bsp_data->leaves.count);
        return 0xFFFFFFFF;
    }

    return loaded_bsp_data->leaves.elements[leaf].cluster;
}

void unknown_function_5092f0(const void *input, float *output) {
    double a = -*(float *)(input + 0x164);
    double b = -*(float *)(input + 0x168);
    double c = -*(float *)(input + 0x144);
    double d = -*(float *)(input + 0x158);
    output[0] = (a - 1.0) / c;
    output[1] = (a + 1.0) / c;
    output[2] = (b - 1.0) / d;
    output[3] = (b + 1.0) / d;
}

void transition_skybox_fog_value(float *current, float target, float delta) {
    double difference = target - *current;

    if(delta >= fabs(difference)) {
        *current = target;
    }
    else if(difference < 0) {
        *current -= delta;
    }
    else {
        *current += delta;
    }
}

void transition_skybox_fog_color(ColorRGB *current, const ColorRGB *target, float delta) {
    transition_skybox_fog_value(&current->r, target->r, delta);
    transition_skybox_fog_value(&current->g, target->g, delta);
    transition_skybox_fog_value(&current->b, target->b, delta);
}
