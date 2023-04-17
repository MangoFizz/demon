#include <ringhopper/bitmap.h>

#include "../memory/table.h"
#include "../tag/tag.h"
#include "../exception/exception.h"
#include "../console/console.h"
#include "bitmap.h"

BitmapData *get_bitmap_data(TableID bitmap_tag_id, uint16_t bitmap_index) {
    if(ID_IS_NULL(bitmap_tag_id)) {
        return NULL;
    }

    Bitmap *bitmap = (Bitmap *)(get_tag_data(bitmap_tag_id));
    size_t bitmap_count = bitmap->bitmap_data.count;
    if(bitmap_index >= bitmap_count) {
        CRASHF_DEBUG("tried to access out-of-bounds bitmap data %zu/%zu for %s.%s",
                     (size_t)(bitmap_index),
                     bitmap_count,
                     get_tag_path(bitmap_tag_id),
                     group_fourcc_to_name(get_tag_group_fourcc(bitmap_tag_id)));
    }

    return &bitmap->bitmap_data.elements[bitmap_index];
}

BitmapData *get_bitmap_data_for_sequence(TableID bitmap_tag_id, uint16_t sequence_index, uint16_t sequence_bitmap) {
    if(ID_IS_NULL(bitmap_tag_id)) {
        return NULL;
    }

    Bitmap *bitmap = (Bitmap *)(get_tag_data(bitmap_tag_id));

    // Is it invalid?
    size_t sequence_count = bitmap->bitmap_group_sequence.count;
    if(sequence_index >= sequence_count) {
        CRASHF_DEBUG("tried to access out-of-bounds bitmap sequence %zu/%zu for %s.%s",
                     (size_t)(sequence_index),
                     sequence_count,
                     get_tag_path(bitmap_tag_id),
                     group_fourcc_to_name(get_tag_group_fourcc(bitmap_tag_id)));
    }

    // Get the bitmap from the sequence
    BitmapGroupSequence *sequence = &bitmap->bitmap_group_sequence.elements[sequence_index];

    if(bitmap->type == BitmapType_sprites) {
        size_t sequence_sprite_count = sequence->sprites.count;
        if(sequence_sprite_count == 0) {
            CRASHF_DEBUG("tried to access out-of-bounds sequence sprite %zu for empty sequence %zu in %s.%s",
                         (size_t)(sequence_bitmap),
                         (size_t)(sequence_index),
                         get_tag_path(bitmap_tag_id),
                         group_fourcc_to_name(get_tag_group_fourcc(bitmap_tag_id)));
        }
        return get_bitmap_data(bitmap_tag_id, sequence->sprites.elements[sequence_bitmap % sequence_sprite_count].bitmap_index);
    }
    else {
        size_t sequence_bitmap_count = sequence->bitmap_count;
        if(sequence_bitmap_count == 0) {
            CRASHF_DEBUG("tried to access out-of-bounds sequence bitmap %zu for empty sequence %zu in %s.%s",
                         (size_t)(sequence_bitmap),
                         (size_t)(sequence_index),
                         get_tag_path(bitmap_tag_id),
                         group_fourcc_to_name(get_tag_group_fourcc(bitmap_tag_id)));
        }
        return get_bitmap_data(bitmap_tag_id, sequence->first_bitmap_index + sequence_bitmap % sequence_bitmap_count);
    }
}
