#include <stdint.h>
#include <string.h>

#include <ringhopper/scenario.h>
#include "../tag/tag.h"
#include "global.h"

typedef struct EngineGlobal {
    const char *name;
    uint32_t type;
    void *data;
    uint8_t unknown[4];
} EngineGlobal;
_Static_assert(sizeof(EngineGlobal) == 16);

static size_t global_count = 0x1E7;
static EngineGlobal **engine_globals = (EngineGlobal **)(0x683280);

uint16_t get_global_index(const char *global) {
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
