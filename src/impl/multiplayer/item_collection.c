#include <stdint.h>

#include "../rng.h"
#include "../tag.h"

typedef struct ItemCollectionPermutation {
    char padding[0x20];
    float weight;
    TagReference item;
    char padding2[0x20];
} ItemCollectionPermutation;

typedef struct ItemCollectionPermutationReflexive {
    uint32_t count;
    ItemCollectionPermutation *first_element;
    char padding[4];
} ItemCollectionPermutationReflexive;

/**
 * Sums up the item collection weight into an integer value.
 *
 * @param ref  Reflexive of the item collection permutations.
 *
 * @return total sum
 */
uint32_t calculate_total_item_collection_weight(ItemCollectionPermutationReflexive *ref) {
    uint32_t total = 0;
    ItemCollectionPermutation *permutation = ref->first_element;

    for(size_t i = 0; i < ref->count; i++, permutation++) {
        total += (uint32_t)permutation->weight;
    }
    return total;
}

/**
 * Pick a random object in the item collection.
 *
 * Note: Items with fractional weights will be rounded down. Also, collections with a total weight above 32768 may randomly fail!
 *
 * @param item_collection_tag_id  Tag ID of the item collection
 *
 * @return item to spawn or NULL_ID if none spawned
 */
TableID pick_item_collection_object(TableID item_collection_tag_id) {
    // Get RNG
    uint32_t rng = advance_rng() >> 0x10;

    // TODO: Use definitions for this
    ItemCollectionPermutationReflexive *permutations = (ItemCollectionPermutationReflexive *)(get_tag_data(item_collection_tag_id));

    // Get total weight
    uint32_t total_weight = calculate_total_item_collection_weight(permutations);

    // Fail if we have no weight
    if(total_weight == 0) {
        return NULL_ID;
    }

    // Calculate the random item we want to find (note: this CAN overflow if total weight exceeds 2^15)
    int32_t random_weight = (rng * total_weight) >> 0x10;

    // Now we just need to pull items until we exceed this value.
    ItemCollectionPermutation *permutation = permutations->first_element;
    for(size_t i = 0; i < permutations->count; i++, permutation++) {
        random_weight -= (int32_t)permutation->weight;
        if(random_weight < 0) {
            return permutation->item.tag_id;
        }
    }

    // Failure
    return NULL_ID;
}
