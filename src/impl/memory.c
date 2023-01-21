#include <windows.h>
#include <stdint.h>

#include "memory.h"

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
