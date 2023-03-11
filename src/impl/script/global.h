#ifndef DEMON__IMPL_SCRIPT_GLOBAL_H
#define DEMON__IMPL_SCRIPT_GLOBAL_H

#include <stdint.h>

#include "ringhopper/scenario.h"
#include "../memory/table.h"

MAKE_TABLE_STRUCT(ScriptNodeTable, ScenarioScriptNode);

typedef uint16_t GlobalID;

/**
 * Get the index of the scripting global if it exists.
 *
 * @param global name of the global
 *
 * @return 0xFFFF if not found, index if found
 */
GlobalID get_global_id(const char *global);

/**
 * Get the value type of the global.
 *
 * @param global_id id of the global
 *
 * @return value type (see ScenarioScriptValueType enum in scenario.json)
 */
uint16_t get_global_type(GlobalID global_id);

/**
 * Get the name of the global.
 *
 * @param global_id id of the global
 *
 * @return global name
 */
const char *get_global_name(GlobalID global_id);

/**
 * Check if the global index is internal to the engine.
 *
 * @param global_id ID of the global returned by get_global_id
 *
 * @return true if the global is internal to the engine, false if it belongs to the scenario
 */
#define IS_INTERNAL_GLOBAL(global_id) ((global_id & 0x8000) != 0)

#endif
