#ifndef DEMON__IMPL_TABLE_TABLE_H
#define DEMON__IMPL_TABLE_TABLE_H

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t TableID;

/** Null ID that refers to no object */
#define NULL_ID ((TableID)(0xFFFFFFFF))

/**
 * Check if the ID is null
 *
 * @return true if the ID is NULL, false if not
 */
#define ID_IS_NULL(id) (id == NULL_ID)

/**
 * Get the index part of the ID
 *
 * @param id table ID to get
 *
 * @return index
 */
#define ID_INDEX_PART(id) ((size_t)(id & 0xFFFF))

/**
 * Get the salt part of the ID
 *
 * @param id table ID to get
 *
 * @return salt
 */
#define ID_SALT_PART(id) ((uint16_t)(id >> 16))

/**
 * Make an ID from a salt and index
 *
 * @param salt  salt to use
 * @param index index to use
 *
 * @return id
 */
#define MAKE_ID(salt, index) ((uint16_t)(index) | ((uint32_t)(salt) << 16))

/**
 * Generate a table struct
 *
 * @param table_struct_name name of the struct to generate
 * @param object_type       element type to use
 *
 * @return table struct definition
 */
#define MAKE_TABLE_STRUCT(table_struct_name, object_type) typedef struct table_struct_name { \
    /** Name - unused */ \
    char name[32]; \
    \
    /** Capacity of the table */ \
    uint16_t max_elements; \
    \
    /** Size of each element in the table */ \
    uint16_t element_size; \
    \
    /** Unknown */ \
    uint8_t unknown_24; \
    \
    /** Unknown? */ \
    uint8_t unknown1[3]; \
    \
    /** d@t@ fourcc - unused */ \
    uint32_t data_fourcc; \
    \
    /** Unknown what this does */ \
    uint16_t unknown_2c; \
    \
    /** Current size of the table, even including elements not active */ \
    uint16_t current_size; \
    \
    /** Number of elements that are active */ \
    uint16_t count; \
    \
    /** Salt of the next element to be occupied (probably) */ \
    uint16_t next_id; \
    \
    /** Pointer to first element in the table */ \
    object_type *first_element; \
} table_struct_name; \
_Static_assert(sizeof(table_struct_name) == 0x38);

/**
 * Generic table struct that can be used to refer to most tables when knowing the element type isn't necessary
 */
MAKE_TABLE_STRUCT(GenericTable, void);

/**
 * Iterator for iterating through tables.
 *
 * Should be initialized with init_table_iterator()
 */
typedef struct TableIterator {
    /** Reference to the table being iterated */
    GenericTable *table;

    /** Current index the iterator is on */
    uint16_t index;

    uint8_t padding[2];

    /** Full ID of the object found - not valid if init_iterator returned NULL */
    TableID id;

    /** Unused */
    uint32_t salt;
} TableIterator;

/**
 * Prepare a TableIterator.
 */
void init_iterator(TableIterator *iterator, const void *table);

/**
 * Iterate through a table to get the next valid instance.
 *
 * @param iterator to use (must be initialized with init_iterator)
 *
 * @return next valid instance, or NULL if none
 */
void *iterate_table(TableIterator *iterator);

/**
 * Iterator callback for the iterate_table_simple function.
 *
 * @param iterator  current iterator value
 * @param item      pointer to item in table
 * @param user_data user data passed in iterate function
 *
 * @return true if the iterator should continue, false if not
 */
typedef bool (*table_iterator_callback)(const TableIterator *iterator, void *item, void *user_data);

/**
 * Iterate a table from start to the end.
 *
 * @param table     table to iterate
 * @param callback  callback to call
 * @param user_data user data to pass into the callback
 */
void iterate_table_simple(void *table, table_iterator_callback callback, void *user_data);

/**
 * Allocate a table of objects. See memory/table.h for the table structure.
 *
 * @param name          name of table
 * @param maximum_count maximum element count
 * @param element_size  size of each element
 *
 * @return table
 */
void *create_table(const char *name, uint16_t maximum_count, uint16_t element_size);

/**
 * Clear the contents of a table.
 *
 * @param table table pointer
 */
void clear_table(GenericTable *table);

#endif
