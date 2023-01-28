#include <windows.h>
#include <ctype.h>
#include <string.h>

#include "list.h"
#include "../tag.h"

#define MAX_SCENARIO_NAME_LENGTH 259

static size_t mp_map_allocated_size = 0;
static uint32_t *mp_map_count = (uint32_t *)(0x702870); // used for the menu list
static uint32_t *mp_map_count2 = (uint32_t *)(0x702874); // unknown what this is for
static MapEntry **mp_maps = (MapEntry **)(0x70286C);

static size_t search_map_list_for_scenario(const char *scenario, MapEntry *maps, size_t maps_count) {
    const char *scenario_base_name = get_tag_base_name(scenario);

    // Copy the scenario until we hit a null byte
    char scenario_lower[MAX_SCENARIO_NAME_LENGTH];
    size_t scenario_length;
    for(scenario_length = 0; scenario_length < sizeof(scenario_lower); scenario_length++) {
        if((scenario_lower[scenario_length] = tolower(scenario_base_name[scenario_length])) == 0) {
            break;
        }
    }

    // Overflows
    if(scenario_length == sizeof(scenario_lower)) {
        return 0xFFFFFFFF;
    }

    // Search it
    //
    // Note that the original game checks for a substring rather than an exact match, but this can lead to the wrong
    // map being opened if there are other maps involved.
    //
    // This is fine to change to an exact match here since the demo only has one MP and one SP map, thus no effective
    // difference since it will only ever ask for those maps.
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

static uint32_t *mp_map_name_indices = (uint32_t *)(0x686458);

static void add_mp_map_with_index(const char *scenario, uint32_t name_index);

static void add_all_stock_mp_maps(void);

void add_mp_map(const char *scenario, uint32_t ui_index_offset) {
    #ifndef DEMON_ENABLE_ENHANCEMENTS

    add_mp_map_with_index(scenario, mp_map_name_indices[ui_index_offset]);

    #else

    // Hijack this function to add all stock maps since it's only called once on the demo because there's only one MP map.
    //
    // TODO: When the caller of this is reverse engineered, we won't need to hijack this poor function anymore.
    add_all_stock_mp_maps();

    #endif
}

void add_custom_mp_map(const char *scenario) {
    add_mp_map_with_index(scenario, MapUIIndex_UnknownMap);
}

static MapEntry *reallocate_mp_map_list(size_t new_count) {
    mp_map_allocated_size = new_count;
    return (*mp_maps = realloc(*mp_maps, new_count * sizeof(**mp_maps)));
}

static void add_mp_map_with_index(const char *scenario_path, uint32_t name_index) {
    // Get the base name to store.
    //
    // Note that the original game stores tag paths as-is! But later versions JUST store the base name, likely as an optimization
    // and to prevent loading the wrong map since the original game also checks for substrings rather than complete matches in
    // search_map_list_for_scenario()
    const char *scenario = get_tag_base_name(scenario_path);

    MapEntry *entries = *mp_maps;
    size_t scenario_strlen = strlen(scenario);
    size_t next_index = *mp_map_count;
    size_t next_size = next_index + 1;

    // If we don't have any maps allocated, start with allocating enough for all stock maps.
    if(entries == NULL) {
        entries = reallocate_mp_map_list(MapUIIndex_UnknownMap);
    }

    // If we do not have enough space, reallocate and double the space
    else if(next_index == mp_map_allocated_size) {
        entries = reallocate_mp_map_list(mp_map_allocated_size * 2);
    }

    // Allocate the path to the next scenario
    size_t string_length = strlen(scenario);
    char *scenario_out = malloc(string_length + 1);

    // Set the path/name index up
    entries[next_index].scenario = scenario_out;
    entries[next_index].name_index = name_index;

    // Copy, lowercasing everything
    for(size_t i = 0; i < string_length; i++) {
        scenario_out[i] = tolower(scenario[i]);
    }

    // Null pointer
    scenario_out[string_length] = 0;

    // Done
    *mp_map_count = next_size;
    *mp_map_count2 = next_size;
}

void free_mp_map_list(void) {
    MapEntry *all_mp_maps = *mp_maps;

    // We have to loop through all the maps to free the scenarios.
    if(all_mp_maps != NULL) {
        for(size_t i = 0; i < *mp_map_count; i++) {
            free(all_mp_maps[i].scenario);
        }
        free(all_mp_maps);
    }

    // Set these to 0. All clean now!
    *mp_maps = NULL;
    *mp_map_count = 0;
    *mp_map_count2 = 0;
    mp_map_allocated_size = 0;
}

static void add_all_stock_mp_maps(void) {
    // Add the stock maps.
    add_mp_map_with_index("levels\\test\\beavercreek\\beavercreek", MapUIIndex_BattleCreek);
    add_mp_map_with_index("levels\\test\\sidewinder\\sidewinder", MapUIIndex_Sidewinder);
    add_mp_map_with_index("levels\\test\\damnation\\damnation", MapUIIndex_Damnation);
    add_mp_map_with_index("levels\\test\\ratrace\\ratrace", MapUIIndex_RatRace);
    add_mp_map_with_index("levels\\test\\prisoner\\prisoner", MapUIIndex_Prisoner);
    add_mp_map_with_index("levels\\test\\hangemhigh\\hangemhigh", MapUIIndex_HangEmHigh);
    add_mp_map_with_index("levels\\test\\chillout\\chillout", MapUIIndex_ChillOut);
    add_mp_map_with_index("levels\\test\\carousel\\carousel", MapUIIndex_Derelict);
    add_mp_map_with_index("levels\\test\\boardingaction\\boardingaction", MapUIIndex_BoardingAction);
    add_mp_map_with_index("levels\\test\\bloodgulch\\bloodgulch", MapUIIndex_BloodGulch);
    add_mp_map_with_index("levels\\test\\wizard\\wizard", MapUIIndex_Wizard);
    add_mp_map_with_index("levels\\test\\putput\\putput", MapUIIndex_ChironTL34);
    add_mp_map_with_index("levels\\test\\longest\\longest", MapUIIndex_Longest);
    add_mp_map_with_index("levels\\test\\icefields\\icefields", MapUIIndex_IceFields);
    add_mp_map_with_index("levels\\test\\deathisland\\deathisland", MapUIIndex_DeathIsland);
    add_mp_map_with_index("levels\\test\\dangercanyon\\dangercanyon", MapUIIndex_DangerCanyon);
    add_mp_map_with_index("levels\\test\\infinity\\infinity", MapUIIndex_Infinity);
    add_mp_map_with_index("levels\\test\\timberland\\timberland", MapUIIndex_Timberland);
    add_mp_map_with_index("levels\\test\\gephyrophobia\\gephyrophobia", MapUIIndex_Gephyrophobia);

    // BINARY HACK TO MAKE IT SO ALL THE MAPS SHOW THEIR NAMES FROM THE TAG DATA RATHER THAN FILENAMES
    //
    // TODO: Decomp the function that gets this instead!
    DWORD old;
    uint8_t *count = (uint8_t *)(0x4935E7);
    VirtualProtect(count, 1, PAGE_READWRITE, &old);
    *count = 19;
    VirtualProtect(count, 1, old, &old);
}
