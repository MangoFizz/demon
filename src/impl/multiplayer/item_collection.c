#include <stdint.h>

#include "item_collection.h"

#include "../rng/rng.h"
#include "../console/console.h"

uint32_t calculate_total_item_collection_weight(const ItemCollection *item_collection) {
    uint32_t total = 0;
    ItemCollectionPermutation *permutation = item_collection->permutations.elements;

    for(size_t i = 0; i < item_collection->permutations.count; i++, permutation++) {
        total += (uint32_t)(permutation->weight);
    }
    return total;
}

TableID pick_item_collection_object(TableID item_collection_tag_id) {
    // Get item collection tag data
    ItemCollection *item_collection = (ItemCollection *)(get_tag_data(item_collection_tag_id));

    // Get total weight
    uint32_t total_weight = calculate_total_item_collection_weight(item_collection);

    // Calculate the random item we want to find (note: this CAN overflow if total weight exceeds 2^15)
    //
    // NOTE: Doing this BEFORE returning NULL_ID is intentional due to parity reasons!
    int32_t random_weight_original = random_i16((int16_t)(total_weight));
    int32_t random_weight = random_weight_original;

    // Fail if we have no weight
    if(total_weight == 0) {
        console_printf_debug_err("%s.item_collection failed to spawn an item!", get_tag_path(item_collection_tag_id));
        console_printf_debug_err("REASON: Can't spawn items with 0 weight");
        return NULL_ID;
    }

    // Now we just need to pull items until we exceed random_weight
    ItemCollectionPermutation *permutation = item_collection->permutations.elements;
    for(size_t i = 0; i < item_collection->permutations.count; i++, permutation++) {
        random_weight -= (int32_t)(permutation->weight);
        if(random_weight < 0) {
            return permutation->item.tag_id;
        }
    }

    // Failure
    console_printf_debug_err("%s.item_collection failed to spawn an item!", get_tag_path(item_collection_tag_id));
    console_printf_debug_err("REASON: Likely due to weight being too high! random_weight = %i, total_weight = %u", random_weight_original, total_weight);
    return NULL_ID;
}
