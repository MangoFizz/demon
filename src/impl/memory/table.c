#include <string.h>

#include "table.h"
#include "memory.h"
#include "../exception/exception.h"

#define ITERATOR_FOURCC 0x69746572 // 'iter'

void init_table_iterator(TableIterator *iterator, const void *table) {
    iterator->table = (GenericTable *)(table);
    iterator->id = NULL_ID;
    iterator->index = 0;
    iterator->salt = (uint32_t)(table) ^ ITERATOR_FOURCC;
}

void *iterate_table(TableIterator *iterator) {
    const GenericTable *table = iterator->table;
    const size_t current_size = table->current_size;
    const size_t element_size = table->element_size;
    void *element = (void *)(table->first_element + (size_t)(iterator->index) * element_size);

    while(iterator->index < current_size) {
        iterator->index++;

        // First two bytes of every element is a salt value, and if it's non-zero, then it's considered occupied.
        uint16_t salt = *(uint16_t *)(element);
        if(salt != 0) {
            iterator->id = MAKE_ID(salt, iterator->index - 1);
            return element;
        }

        element += element_size;
    }

    return NULL;
}

void *create_table(const char *name, uint16_t maximum_count, uint16_t element_size) {
    GenericTable *table;

    uint32_t allocation_amount = sizeof(*table) + (size_t)(maximum_count) * (size_t)(element_size);
    table = (GenericTable *)(allocate_heap(allocation_amount));
    memset(table, 0, allocation_amount);

    strncpy(table->name, name, sizeof(table->name) - 1);
    table->max_elements = maximum_count;
    table->element_size = element_size;
    table->data_fourcc = 0x64407440;
    table->first_element = (void *)(table) + sizeof(*table);
    table->next_id = *(uint16_t *)(table->name) | 0x8000;

    // TODO: The game makes some call to 0x004CDF80, but this seems to corrupt the stack?? More investigation needs done here.
    // It isn't necessary to allocate this structure, but I don't like it.

    return table;
}

void iterate_table_simple(void *table, table_iterator_callback callback, void *user_data) {
    TableIterator iterator;
    init_table_iterator(&iterator, table);

    for(void *value = iterate_table(&iterator); value != NULL; value = iterate_table(&iterator)) {
        if(!callback(&iterator, value, user_data)) {
            return;
        }
    }
}

void clear_table(GenericTable *table) {
    size_t count = table->max_elements;

    for(size_t i = 0; i < count; i++) {
        *(uint16_t *)(table->first_element + i * table->element_size) = 0;
    }

    table->unknown_2c = 0;
    table->current_size = 0;
    table->count = 0;
    table->next_id = *(uint16_t *)(table->name) | 0x8000;
}

void *get_table_element(GenericTable *table, TableID id) {
    // ID is null?
    if(ID_IS_NULL(id)) {
        return NULL;
    }

    size_t index = ID_INDEX_PART(id);
    size_t max_elements = table->max_elements;

    // Out of bounds? (If so then something really needs fixed!)
    if(index >= max_elements) {
        CRASHF_DEBUG("Out-of-bounds access of element %zu / %zu for table 0x%08X (%s) with ID 0x%08X", index, max_elements, (uintptr_t)(table), table->name, id);
        return NULL;
    }

    // Match the salt
    uint16_t salt = ID_SALT_PART(id);
    void *element = table->first_element + index * table->element_size;
    uint16_t element_salt = *(uint16_t *)(element);
    if(element_salt == 0 || (salt != 0 && element_salt != salt)) {
        return NULL;
    }

    // Done!
    return element;
}

void init_table_element(GenericTable *table, void *new_element_location) {
    memset(new_element_location, 0, table->element_size);
    *(uint16_t *)(new_element_location) = (table->next_id)++;
    if(table->next_id == 0) {
        table->next_id = 0x8000;
    }
}
