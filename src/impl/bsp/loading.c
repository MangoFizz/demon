#include <windows.h>
#include <stdio.h>

#include "../tag.h"
#include "ringhopper/model_collision_geometry.h"
#include "ringhopper/scenario.h"
#include "ringhopper/scenario_structure_bsp.h"
#include "loading.h"
#include "../console/console.h"

static ScenarioStructureBSP **loaded_bsp_data = (ScenarioStructureBSP **)(0x7367BC);
static uint16_t *current_bsp_index = (uint16_t *)(0x696090);
static uint16_t **current_bsp_index_ptr = (uint16_t **)(0x7367B4);
static ModelCollisionGeometryBSP **current_collision_bsp_address = (ModelCollisionGeometryBSP **)(0x7367B0);
static ModelCollisionGeometryBSP **current_collision_bsp_address2 = (ModelCollisionGeometryBSP **)(0x7367B8);
static uint8_t *value_007090aa = (uint8_t *)(0x007090AA);

extern void (*switch_bsp_00539350)(void);
extern void (*switch_bsp_00442520)(ScenarioBSP *bsp_entry);
extern bool (*switch_bsp_004424B0)(ScenarioBSP *bsp_entry);
extern void (*switch_bsp_00539370)(void);

ScenarioStructureBSP *get_loaded_bsp_tag_data(void) {
    return *loaded_bsp_data;
}

bool switch_bsp(uint16_t bsp_index) {
    // Do nothing if the BSP is the same.
    if(*current_bsp_index == bsp_index) {
        return false;
    }

    // Don't switch to an out-of-bounds BSP.
    Scenario *scenario = get_scenario_tag_data();
    if(bsp_index >= scenario->structure_bsps.count) {
        return false;
    }

    // Are we loading a BSP from something else?
    bool switching_bsp = *current_bsp_index != 0xFFFF;
    if(switching_bsp) {
        ScenarioBSP *old_entry = &scenario->structure_bsps.elements[*current_bsp_index];
        switch_bsp_00539350();
        switch_bsp_00442520(old_entry);
        **current_bsp_index_ptr = 0xFFFF;
        *current_bsp_index = 0xFFFF;
    }

    ScenarioBSP *entry = &scenario->structure_bsps.elements[bsp_index];

    bool loaded = switch_bsp_004424B0(entry);
    if(loaded) {
        *loaded_bsp_data = get_tag_data(entry->structure_bsp.tag_id);
        *current_collision_bsp_address = (*current_collision_bsp_address2 = (*loaded_bsp_data)->collision_bsp.elements);
        *current_bsp_index = (**current_bsp_index_ptr = bsp_index);
        if(switching_bsp) {
            switch_bsp_00539370();
        }
    }

    *value_007090aa = 1;
    return loaded;
}
