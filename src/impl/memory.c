#include <windows.h>
#include <stdint.h>

#include "memory.h"
#include "id.h"

#define BASE_HEAP_AMOUNT (size_t)(4352 * 1024) // 4.25 MiB
#define TAG_DATA_LENGTH (size_t)(23 * 1024 * 1024) // 23 MiB

#define MANAGED_HEAP_ADDRESS_START (void *)(0x4BAD0000)
#define TAG_DATA_LOAD_ADDRESS (void *)(MANAGED_HEAP_ADDRESS_START + BASE_HEAP_AMOUNT) // 0x4BF10000

#define UNK1_SIZE (size_t)(16 * 1024) // 16 KiB

static void **managed_heap = (void *)(0x6A38F0);
static void **tag_data = (void *)(0x6A38F4);
static void **unk1 = (void *)(0x6A38F8);
static void **sound_cache = (void *)(0x6A38FC);

static uint16_t *sound_cache_mib = (uint16_t *)(0x67E9B4);

/**
 * Allocate buffers for holding tag data and sounds.
 */
void allocate_heaps(void) {
    *tag_data = TAG_DATA_LOAD_ADDRESS;
    *managed_heap = VirtualAlloc(MANAGED_HEAP_ADDRESS_START, BASE_HEAP_AMOUNT + TAG_DATA_LENGTH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    *unk1 = VirtualAlloc(0, UNK1_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // unknown; does not appear to be used?
    *sound_cache = VirtualAlloc(0, *sound_cache_mib * 1024 * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}


static void **alloc_base_address = (void **)(0x6DA068);
static uint32_t *alloc_size = (uint32_t *)(0x6DA06C);

MAKE_TABLE_STRUCT(GenericTable, void);

void *create_table(const char *name, uint16_t maximum_count, uint16_t element_size) {
    GenericTable *table = *alloc_base_address + *alloc_size;
    uint32_t allocation_amount = sizeof(*table) + (size_t)(maximum_count) * (size_t)(element_size);

    memset(table, 0, allocation_amount);
    *alloc_size += allocation_amount;

    strncpy(table->name, name, sizeof(table->name) - 1);
    table->max_elements = maximum_count;
    table->element_size = element_size;
    table->data_fourcc = 0x64407440;
    table->first_element = (void *)(table) + sizeof(*table);

    // TODO: The game makes some call to 0x004cdf80, but this seems to corrupt the stack?? More investigation needs done here.
    // It isn't necessary to allocate this structure, but I don't like it.

    return table;
}
