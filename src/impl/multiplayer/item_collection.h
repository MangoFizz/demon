#ifndef DEMON__IMPL_MULTIPLAYER_ITEM_COLLECTION_H
#define DEMON__IMPL_MULTIPLAYER_ITEM_COLLECTION_H

#include "../tag.h"

#include "ringhopper/item_collection.h"

/**
 * Sums up the item collection weight into an integer value.
 *
 * @param pointer to the item collection
 *
 * @return total sum
 */
uint32_t calculate_total_item_collection_weight(ItemCollection *item_collection);

/**
 * Pick a random object in the item collection.
 *
 * Note: Items with fractional weights will be rounded down. Also, collections with a total weight above 32768 may randomly fail!
 *
 * @param item_collection_tag_id  Tag ID of the item collection
 *
 * @return item to spawn or NULL_ID if none spawned
 */
TableID pick_item_collection_object(TableID item_collection_tag_id);

#endif
