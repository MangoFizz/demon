#ifndef DEMON__IMPL_ID_H
#define DEMON__IMPL_ID_H

#include <stdint.h>

typedef uint32_t TableID;
#define NULL_ID ((TableID)(0xFFFFFFFF))
#define ID_IS_NULL(id) (id == NULL_ID)
#define ID_INDEX_PART(id) ((size_t)(id & 0xFFFF))
#define ID_SALT_PART(id) ((uint16_t)(id >> 16))
#define MAKE_ID(salt, index) ((uint16_t)(index) | (uint32_t)(salt << 16))

#define MAKE_TABLE_STRUCT(table_struct_name, object_type) typedef struct table_struct_name { \
    char name[32]; \
    uint16_t max_elements; \
    uint16_t element_size; \
    uint8_t unknown1[8]; \
    uint8_t unknown2[2]; \
    uint16_t current_size; \
    uint16_t count; \
    uint16_t next_id; \
    object_type *first_element; \
} table_struct_name; \
_Static_assert(sizeof(table_struct_name) == 0x38);

#endif
