#ifndef DEMON__IMPL_SCRIPT_GLOBAL_H
#define DEMON__IMPL_SCRIPT_GLOBAL_H

#include <stdint.h>

#include "ringhopper/scenario.h"
#include "../memory/table.h"

MAKE_TABLE_STRUCT(ScriptNodeTable, ScenarioScriptNode);

typedef struct HSGlobal {
    uint32_t unknown;
    ScenarioScriptNodeValue value;
} HSGlobal;
MAKE_TABLE_STRUCT(HSGlobalTable, HSGlobal);

typedef struct EngineGlobal {
    const char *name;
    uint32_t type;
    ScenarioScriptNodeValue *data;
    uint32_t permission;
} EngineGlobal;
_Static_assert(sizeof(EngineGlobal) == 16);

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
#define IS_INTERNAL_GLOBAL(global_id) (((global_id) & 0x8000) != 0)

/**
 * Convert the global ID to an index.
 *
 * @param global_id ID of the global returned by get_global_id
 *
 * @return index
 */
#define GLOBAL_ID_TO_INDEX(global_id) ((global_id) & 0x7FFF)

/**
 * Get the number of globals internal to the engine.
 *
 * @return global count
 */
size_t get_internal_global_count(void);

/**
 * Get all internal globals.
 *
 * @return internal globals
 */
EngineGlobal **get_internal_globals(void);


#endif
