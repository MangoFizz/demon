#include <ctype.h>
#include <string.h>

#include "list.h"

#define MAX_SCENARIO_NAME_LENGTH 259

static uint32_t *mp_map_count = (uint32_t *)(0x702870);
static MapEntry **mp_maps = (MapEntry **)(0x70286C);

static size_t search_map_list_for_scenario(const char *scenario, MapEntry *maps, size_t maps_count) {
    // Copy the scenario until we hit a null byte
    char scenario_lower[MAX_SCENARIO_NAME_LENGTH];
    size_t scenario_length;
    for(scenario_length = 0; scenario_length < sizeof(scenario_lower); scenario_length++) {
        if((scenario_lower[scenario_length] = tolower(scenario[scenario_length])) == 0) {
            break;
        }
    }

    // Overflows
    if(scenario_length == sizeof(scenario_lower)) {
        return 0xFFFFFFFF;
    }

    // Search it
    for(size_t i = 0; i < maps_count; i++) {
        if(strcmp(maps[i].scenario, scenario_lower) == 0) {
            return i;
        }
    }

    // Fail
    return 0xFFFFFFFF;
}

uint32_t get_mp_map_index(const char *scenario) {
    return search_map_list_for_scenario(scenario, *mp_maps, *mp_map_count);
}
