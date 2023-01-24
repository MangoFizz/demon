#ifndef DEMON__IMPL_BSP_LOADING_H
#define DEMON__IMPL_BSP_LOADING_H

#include <stdbool.h>

struct ScenarioStructureBSP;

/**
 * Get a pointer to the currently loaded BSP
 */
struct ScenarioStructureBSP *get_loaded_bsp_tag_data(void);

/**
 * Load the given BSP
 *
 * @param bsp_index bsp to load
 */
bool switch_bsp(uint16_t bsp_index);

#endif
