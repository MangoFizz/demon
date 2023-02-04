#ifndef DEMON__IMPL_MEMORY_H
#define DEMON__IMPL_MEMORY_H

/**
 * Allocate a table of objects. See table/table.h for the table structure.
 *
 * @param name          name of table
 * @param maximum_count maximum element count
 * @param element_size  size of each element
 *
 * @return table
 */
void *create_table(const char *name, uint16_t maximum_count, uint16_t element_size);

/**
 * Allocate buffers for holding tag data and sounds.
 */
void allocate_heaps(void);

#endif
