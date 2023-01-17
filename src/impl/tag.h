#ifndef DEMON__IMPL_TAG_H
#define DEMON__IMPL_TAG_H

#include <stdint.h>

typedef uint32_t FourCC;

#include "id.h"

typedef struct TagReference {
    FourCC group_fourcc;
    char unused1[4];
    char unused2[4];
    TableID tag_id;
} TagReference;

/**
 * Get the data for the tag.
 */
void *get_tag_data(TableID tag_id);

/**
 * Lookup the tag.
 *
 * @return tag ID if found, NULL_ID if not
 */
TableID lookup_tag(const char *path, FourCC group);

#endif
