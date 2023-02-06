#include <stdbool.h>

#include "player.h"
#include "../object/object.h"
#include "../math/math.h"

MAKE_TABLE_STRUCT(PlayerTable, Player);

PlayerTable **players = (PlayerTable **)(0x869460);

void update_player_interaction(uint16_t new_object_priority, uint16_t new_seat_index, TableID new_object_id, TableID player_id) {
    // Get the player
    Player *player = (*players)->first_element + ID_INDEX_PART(player_id);

    // Check priority if this is not 11
    if(new_object_priority != 11) {
        uint16_t current_interaction_object_priority = player->interaction_object_priority;
        uint16_t current_interaction_object_seat = player->interaction_object_seat;
        TableID current_interaction_object_id = player->interaction_object_id;

        // If it's the same priority, prioritize based on distance
        if(new_object_priority == current_interaction_object_priority) {
            // Optimization: We only need to check distance if we are not the same object
            if(new_object_id == player->interaction_object_id) {
                DynamicObjectBase *player_object = resolve_object_index_to_data_unchecked(ID_INDEX_PART(player->object_id));
                DynamicObjectBase *new_object = resolve_object_index_to_data_unchecked(ID_INDEX_PART(new_object_id));
                DynamicObjectBase *old_object = resolve_object_index_to_data_unchecked(ID_INDEX_PART(current_interaction_object_id));

                // Optimization: Don't use square roots for distance checks as it's unnecessary for checking if greater/less
                float new_object_distance = vectorxyz_distance_squared(&player_object->position, &new_object->position);
                float old_object_distance = vectorxyz_distance_squared(&player_object->position, &old_object->position);

                if(new_object_distance >= old_object_distance) {
                    return;
                }
            }
        }
        // Ignore it if it's lower
        else if(new_object_priority < current_interaction_object_priority) {
            return;
        }
    }

    // Set it!
    player->interaction_object_priority = new_object_priority;
    player->interaction_object_seat = new_seat_index;
    player->interaction_object_id = new_object_id;
}
