#include <string.h>
#include "tag.h"

uint8_t *map_loaded = (uint8_t *)(0x69F4F8);

typedef struct TagEntry {
    FourCC primary_group;
    FourCC secondary_group;
    FourCC tertiary_group;
    TableID id;
    char *path;
    void *data;
    char padding[0x8];
} TagEntry;
_Static_assert(sizeof(TagEntry) == 0x20);

typedef struct TagDataHeader {
    TagEntry *tags;
    TableID scenario_tag_id;
    uint32_t tag_checksum;
    uint32_t tag_count;
    uint32_t model_part_count;
    uint32_t model_data_file_offset;
    uint32_t model_part_count2;
    uint32_t vertex_size;
    uint32_t model_data_size;
    FourCC tags_fourcc;
} TagDataHeader;
_Static_assert(sizeof(TagDataHeader) == 0x28);

TagDataHeader **tag_data_header_loaded = (TagDataHeader **)(0x69FCFC);

void *get_tag_data(TableID tag_id) {
    return (*tag_data_header_loaded)->tags[ID_INDEX_PART(tag_id)].data;
}

TableID lookup_tag(const char *path, FourCC group) {
    if(!*map_loaded) {
        return NULL_ID;
    }

    TagDataHeader *header = *tag_data_header_loaded;
    uint32_t tag_count = header->tag_count;

    TagEntry *tag = header->tags;
    for(uint32_t i = 0; i < tag_count; i++, tag++) {
        if(tag->primary_group == group && strcmp(path, tag->path) == 0) {
            return tag->id;
        }
    }
    return NULL_ID;
}
