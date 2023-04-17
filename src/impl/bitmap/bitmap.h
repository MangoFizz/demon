#ifndef DEMON__IMPL_BITMAP_BITMAP_H
#define DEMON__IMPL_BITMAP_BITMAP_H

#include <stdint.h>

struct BitmapData;
#include "../memory/table.h"

/**
 * Get the bitmap data.
 *
 * @param bitmap_tag_id tag ID
 * @param bitmap_index  bitmap
 *
 * @return NULL if tag ID is null
 */
BitmapData *get_bitmap_data(TableID bitmap_tag_id, uint16_t bitmap_index);

/**
 * Get the bitmap data from a bitmap in a sequence.
 *
 * @param bitmap_tag_id   tag ID
 * @param sequence_index  sequence
 * @param sequence_bitmap sequence bitmap
 *
 * @return NULL if tag ID is null
 */
BitmapData *get_bitmap_data_for_sequence(TableID bitmap_tag_id, uint16_t sequence_index, uint16_t sequence_bitmap);

#endif
