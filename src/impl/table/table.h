#ifndef DEMON__IMPL_TABLE_TABLE_H
#define DEMON__IMPL_TABLE_TABLE_H

#include <stdint.h>

typedef uint32_t TableID;
#define NULL_ID ((TableID)(0xFFFFFFFF))
#define ID_IS_NULL(id) (id == NULL_ID)
#define ID_INDEX_PART(id) ((size_t)(id & 0xFFFF))
#define ID_SALT_PART(id) ((uint16_t)(id >> 16))
#define MAKE_ID(salt, index) ((uint16_t)(index) | ((uint32_t)(salt) << 16))

#define MAKE_TABLE_STRUCT(table_struct_name, object_type) typedef struct table_struct_name { \
    char name[32]; \
    uint16_t max_elements; \
    uint16_t element_size; \
    uint8_t unknown1[4]; \
    uint32_t data_fourcc; \
    uint8_t unknown2[2]; \
    uint16_t current_size; \
    uint16_t count; \
    uint16_t next_id; \
    object_type *first_element; \
} table_struct_name; \
_Static_assert(sizeof(table_struct_name) == 0x38);

MAKE_TABLE_STRUCT(GenericTable, void);

/**
 * Iterator for iterating through tables.
 *
 * Should be initialized with init_table_iterator()
 */
typedef struct TableIterator {
    GenericTable *table;
    uint16_t index;
    uint8_t padding[2];
    TableID id;
    uint32_t salt;
} TableIterator;

/**
 * Prepare a TableIterator.
 */
void init_iterator(TableIterator *iterator, const void *table);

/**
 * Iterate through a table to get the next valid instance.
 *
 * @return next valid instance, or NULL if none
 */
void *iterate_table(TableIterator *iterator);

#endif
