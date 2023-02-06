#include <string.h>

#include "table.h"
#include "memory.h"

#define ITER_FOURCC 0x69746572

void init_table_iterator(TableIterator *iterator, const void *table) {
    iterator->table = (GenericTable *)(table);
    iterator->id = NULL_ID;
    iterator->index = 0;
    iterator->salt = (uint32_t)(table) ^ ITER_FOURCC;
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

    // TODO: The game makes some call to 0x004cdf80, but this seems to corrupt the stack?? More investigation needs done here.
    // It isn't necessary to allocate this structure, but I don't like it.

    return table;
}
