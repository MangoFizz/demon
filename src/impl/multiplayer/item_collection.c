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
    // Get item collection tag data
    //
    // TODO: Use definitions for this
    ItemCollectionPermutationReflexive *permutations = (ItemCollectionPermutationReflexive *)(get_tag_data(item_collection_tag_id));

    // Get total weight
    uint32_t total_weight = calculate_total_item_collection_weight(permutations);

    // Calculate the random item we want to find (note: this CAN overflow if total weight exceeds 2^15)
    //
    // NOTE: Doing this BEFORE returning NULL_ID is intentional due to parity reasons!
    int32_t random_weight = random_i16((int16_t)(total_weight));

    // Fail if we have no weight
    if(total_weight == 0) {
        return NULL_ID;
    }

    // Now we just need to pull items until we exceed random_weight
    ItemCollectionPermutation *permutation = permutations->first_element;
    for(size_t i = 0; i < permutations->count; i++, permutation++) {
        random_weight -= (int32_t)(permutation->weight);
        if(random_weight < 0) {
            return permutation->item.tag_id;
        }
    }

    // Failure
    return NULL_ID;
}
