#ifndef DEMON__IMPL_MAP_LIST_H
#define DEMON__IMPL_MAP_LIST_H

#include <stdint.h>

typedef enum MapIndex {
    MapIndex_BattleCreek = 0,
    MapIndex_Sidewinder = 1,
    MapIndex_Damnation = 2,
    MapIndex_RatRace = 3,
    MapIndex_Prisoner = 4,
    MapIndex_HangEmHigh = 5,
    MapIndex_ChillOut = 6,
    MapIndex_Derelict = 7,
    MapIndex_BoardingAction = 8,
    MapIndex_BloodGulch = 9,
    MapIndex_Wizard = 10,
    MapIndex_ChironTL34 = 11,
    MapIndex_Longest = 12,
    MapIndex_IceFields = 13,
    MapIndex_DeathIsland = 14,
    MapIndex_DangerCanyon = 15,
    MapIndex_Infinity = 16,
    MapIndex_Timberland = 17,
    MapIndex_Gephyrophobia = 18,
    MapIndex_UnknownMap = 19
} MapIndex;

typedef struct MapEntry {
    const char *scenario;
    uint16_t map_index;
    uint8_t padding[2];
} MapEntry;

/**
 * Get the map index by its scenario name.
 */
uint32_t get_mp_map_index(const char *scenario);

#endif
