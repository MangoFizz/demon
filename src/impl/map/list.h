#ifndef DEMON__IMPL_MAP_LIST_H
#define DEMON__IMPL_MAP_LIST_H

#include <stdint.h>

/**
 * Name indices which correspond to the original 19 maps.
 */
typedef enum MapUIIndex {
    MapUIIndex_BattleCreek = 0,
    MapUIIndex_Sidewinder = 1,
    MapUIIndex_Damnation = 2,
    MapUIIndex_RatRace = 3,
    MapUIIndex_Prisoner = 4,
    MapUIIndex_HangEmHigh = 5,
    MapUIIndex_ChillOut = 6,
    MapUIIndex_Derelict = 7,
    MapUIIndex_BoardingAction = 8,
    MapUIIndex_BloodGulch = 9,
    MapUIIndex_Wizard = 10,
    MapUIIndex_ChironTL34 = 11,
    MapUIIndex_Longest = 12,
    MapUIIndex_IceFields = 13,
    MapUIIndex_DeathIsland = 14,
    MapUIIndex_DangerCanyon = 15,
    MapUIIndex_Infinity = 16,
    MapUIIndex_Timberland = 17,
    MapUIIndex_Gephyrophobia = 18,
    MapUIIndex_UnknownMap = 19
} MapUIIndex;

typedef struct MapEntry {
    char *scenario;
    uint32_t name_index; // corresponds to MapIndex
} MapEntry;

/**
 * Get the map index by its scenario name. This is case insensitive.
 *
 * @param scenario scenario to check
 */
uint32_t get_mp_map_index(const char *scenario);

/**
 * Add a map to the MP map list with an indirect offset to the UI index. Note the name will be lowercased when added.
 *
 * @param scenario          scenario name or tag path
 * @param name_index_offset index of the index
 */
void add_mp_map(const char *scenario, uint32_t ui_index_offset);

/**
 * Add a custom MP map to the map list.
 *
 * @param scenario scenario name or tag path
 */
void add_custom_mp_map(const char *scenario);

#endif
