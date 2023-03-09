#ifndef DEMON__IMPL_SCRIPT_GLOBAL_H
#define DEMON__IMPL_SCRIPT_GLOBAL_H

#include <stdint.h>

/**
 * Get the index of the scripting global if it exists.
 *
 * @return 0xFFFF if not found, index if found
 */
uint16_t get_global_index(const char *global);

/**
 * Check if the global index is internal to the engine.
 *
 * @return true if the global is internal to the engine, false if it belongs to the scenario
 */
#define IS_INTERNAL_GLOBAL(global_index) ((global & 0x8000) != 0)

#endif
