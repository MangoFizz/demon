#ifndef DEMON__IMPL_PLAYER_PLAYER_H
#define DEMON__IMPL_PLAYER_PLAYER_H

#include <uchar.h>

#include "../3d.h"
#include "../table/table.h"

typedef struct Player {
    uint16_t player_id;
    uint16_t local_id;
    char16_t name[12];
    uint8_t field_0x1c[4];
    uint8_t team;
    uint8_t field_0x21[3];
    TableID interaction_object_id;
    uint16_t interaction_object_priority;
    uint16_t interaction_object_seat;
    uint32_t respawn_time;
    uint32_t respawn_time_growth;
    TableID object_id;
    TableID last_object_id;
    uint8_t field_0x3c[8];
    uint32_t last_fire_time;
    char16_t name2[12];
    uint16_t color;
    uint8_t field_0x62[2];
    uint8_t machine_index;
    uint8_t controller_index;
    uint8_t team2;
    uint8_t index;
    uint16_t invisibility_time;
    uint8_t field_0x6a[2];
    float speed_multiplier;
    TableID teleporter_flag_id;
    uint32_t objective_mode;
    TableID objective_player_id;
    TableID target_player;
    uint32_t target_time;
    uint32_t last_death_time;
    TableID slayer_target;
    uint32_t odd_man_out;
    uint8_t field_0x90[6];
    uint16_t kill_streak;
    uint16_t multikill;
    uint16_t last_kill_time;
    uint16_t kills;
    uint8_t field_0x9e[6];
    uint16_t assists;
    uint8_t field_0xa6[6];
    uint16_t betrays;
    uint16_t deaths;
    uint16_t suicides;
    uint8_t field_0xb2[14];
    uint16_t team_kills;
    uint8_t field_0xc2[2];
    uint16_t misc_stats[3];
    uint8_t field_0xca[2];
    uint32_t telefrag_timer;
    uint32_t quit_time;
    uint8_t field_0xd4[8];
    uint32_t ping;
    uint32_t team_kill_count;
    uint32_t team_kill_timer;
    uint8_t field_0xe8[16];
    VectorXYZ position;
    uint8_t field_0x104[24];
    uint16_t controls;
    uint8_t field_0x11e[2];
    uint8_t field_0x120[24];
    float baseline_update_xy_aim;
    float baseline_update_z_aim;
    float baseline_update_forward;
    float baseline_update_left;
    float baseline_update_rate_of_fire;
    uint16_t baseline_update_weapon_slot;
    uint16_t baseline_update_grenade_slot;
    uint8_t field_0x150[4];
    VectorXYZ update_aiming;
    uint8_t field_0x160[16];
    VectorXYZ update_position;
    uint8_t field_0x17c[132];
} Player;
_Static_assert(sizeof(Player) == 0x200);

/**
 * Update the object the player is currently interacting with.
 *
 * This function will do nothing if the current object has a higher priority or, if it's the same priority, is closer UNLESS new_object_priority is 11.
 *
 * @param new_object_priority new priority
 * @param new_seat_index      new seat index
 * @param new_object_id       new object ID
 * @param player_id           player ID
 */
void update_player_interaction(uint16_t new_object_priority, uint16_t new_seat_index, TableID new_object_id, TableID player_id);

#endif
