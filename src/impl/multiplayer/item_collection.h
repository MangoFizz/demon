#ifndef DEMON__IMPL_MULTIPLAYER_ITEM_COLLECTION_H
#define DEMON__IMPL_MULTIPLAYER_ITEM_COLLECTION_H

#include "../tag.h"

// TODO: Use tag definitions from Invader!

typedef struct ItemCollectionPermutation {
    uint8_t padding[0x20];
    float weight;
    TagReference item;
    uint8_t padding2[0x20];
} ItemCollectionPermutation;
_Static_assert(sizeof(ItemCollectionPermutation) == 0x54);

typedef struct ItemCollectionPermutationReflexive {
    uint32_t count;
    ItemCollectionPermutation *first_element;
    uint8_t padding[4];
} ItemCollectionPermutationReflexive;
_Static_assert(sizeof(ItemCollectionPermutationReflexive) == 0xC);

/**
 * Sums up the item collection weight into an integer value.
 *
 * @param ref  Reflexive of the item collection permutations.
 *
 * @return total sum
 */
uint32_t calculate_total_item_collection_weight(ItemCollectionPermutationReflexive *ref);

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
