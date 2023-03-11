#include <stdint.h>
#include <string.h>

#include <ringhopper/scenario.h>
#include "../tag/tag.h"
#include "global.h"
#include "../console/console.h"
#include "../exception/exception.h"

typedef struct EngineGlobal {
    const char *name;
    uint32_t type;
    void *data;
    uint8_t unknown[4];
} EngineGlobal;
_Static_assert(sizeof(EngineGlobal) == 16);

static size_t global_count = 0x1E7;
static EngineGlobal **engine_globals = (EngineGlobal **)(0x683280);

GlobalID get_global_id(const char *global) {
    // First check internal globals.
    for(size_t i = 0; i < global_count; i++) {
        if(strcmp(global, engine_globals[i]->name) == 0) {
            return i | 0x8000;
        }
    }

    // Next check engine globals.
    const Scenario *scenario = get_scenario_tag_data();
    if(scenario != NULL) {
        size_t global_count = scenario->globals.count & 0x7FFF; // & with 0x7FFF because we can't access globals past that
        for(size_t i = 0; i < global_count; i++) {
            if(strcmp(global, scenario->globals.elements[i].name.string) == 0) {
                return i;
            }
        }
    }

    return 0xFFFF;
}

uint16_t get_global_type(GlobalID global_id) {
    if(IS_INTERNAL_GLOBAL(global_id)) {
        return engine_globals[global_id & 0x7FFF]->type;
    }
    else {
        return get_scenario_tag_data()->globals.elements[global_id].type;
    }
}

const char *get_global_name(GlobalID global_id) {
    if(IS_INTERNAL_GLOBAL(global_id)) {
        return engine_globals[global_id & 0x7FFF]->name;
    }
    else {
        return get_scenario_tag_data()->globals.elements[global_id].name.string;
    }
}

extern bool (*value_types_can_be_converted)(enum ScenarioScriptValueType actual, enum ScenarioScriptValueType expected);

bool compile_global(TableID node_id) {
    ScriptNodeTable *nodes = *(ScriptNodeTable **)(0x869454);
    const char *string_data = *(const char **)(0x6A8868);

    uint16_t node_index = ID_INDEX_PART(node_id);
    ScenarioScriptNode *node = nodes->first_element + node_index;

    // Does the global exist?
    const char *node_name = string_data + node->string_offset;
    GlobalID id = get_global_id(node_name);
    node->data.l = (int16_t)id;
    if(id == 0xFFFF) {
        if(*(uint8_t *)(0x6A8988) != 0) {
            *(const char **)(0x6A887C) = "Invalid variable name!";
            *(const char **)(0x6A8880) = node_name;
            CRASHF_DEBUG("Broken or incompatible scripts! global '%s' not found!", node_name);
        }
        return false;
    }

    // Make sure the type is valid?
    enum ScenarioScriptValueType type = get_global_type(id);
    enum ScenarioScriptValueType expected = node->type;
    if(expected != ScenarioScriptValueType_passthrough) {
        if(!value_types_can_be_converted(type, expected)) {
            *(const char **)(0x6A887C) = "Invalid variable type!";
            *(const char **)(0x6A8880) = node_name;
            CRASHF_DEBUG("Broken or incompatible scripts! global '%s' isn't the right type!", node_name);
            return false;
        }
    }

    node->type = type;
    node->flags |= 4;
    return true;
}
