#ifndef DEMON__IMPL_MEMORY_MEMORY_H
#define DEMON__IMPL_MEMORY_MEMORY_H

#include <stdint.h>

/**
 * Allocate buffers for holding tag data and sounds.
 */
void allocate_heaps(void);

/**
 * Allocate memory in the heap.
 *
 * Take caution that there is not much space in this heap (only 4.25 MiB). If there is not enough space, NULL will be
 * returned, and an error will be logged in stderr.
 *
 * @param size number of bytes to allocate
 *
 * @return     pointer to new data or NULL if there is not enough space
 */
void *allocate_heap(size_t size);

#endif
