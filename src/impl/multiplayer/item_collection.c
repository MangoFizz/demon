#include <stdint.h>

#include "item_collection.h"

#include "../rng.h"

uint32_t calculate_total_item_collection_weight(ItemCollectionPermutationReflexive *ref) {
    uint32_t total = 0;
    ItemCollectionPermutation *permutation = ref->first_element;

    for(size_t i = 0; i < ref->count; i++, permutation++) {
        total += (uint32_t)permutation->weight;
    }
    return total;
}

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
