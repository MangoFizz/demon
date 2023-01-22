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
_Static_assert(sizeof(TagReference) == 0x10);

/**
 * Get the data for the tag.
 *
 * @param tag_id
 *
 * @return pointer to tag data
 */
void *get_tag_data(TableID tag_id);

/**
 * Lookup the tag.
 *
 * @param path  path of the tag (not including file extension)
 * @param group group of the tag
 *
 * @return tag ID if found, NULL_ID if not
 */
TableID lookup_tag(const char *path, FourCC group);

/**
 * Get the data of the currently loaded scenario tag.
 */
void *get_scenario_tag_data(void);

#endif
