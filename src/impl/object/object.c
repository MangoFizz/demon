#include <windows.h>
#include "object.h"

#include <stdio.h>

typedef struct ObjectTableEntry {
    uint16_t salt;
    uint8_t flags_low;
    uint8_t flags_high;
    uint32_t unknown;
    DynamicObjectBase *object;
} ObjectTableEntry;

_Static_assert(sizeof(ObjectTableEntry) == 0xC);

MAKE_TABLE_STRUCT(ObjectTable, ObjectTableEntry);

static ObjectTable **object_table = (ObjectTable **)(0x84F850);

DynamicObjectBase *resolve_object_id(uint32_t flag_check, TableID object_id) {
    ObjectTable *halo_object_table = *object_table;

    // Split it
    uint16_t input_index = ID_INDEX_PART(object_id);
    uint16_t input_salt = ID_SALT_PART(object_id);

    // Check if out of bounds.
    //
    // NOTE: The original code does a signed check which isn't necessary here.
    if(ID_IS_NULL(object_id) || input_index >= halo_object_table->max_elements) {
        return NULL;
    }

    // Get the offset
    size_t offset = (uint32_t)(input_index) * halo_object_table->element_size;
    ObjectTableEntry *table_entry = (ObjectTableEntry *)((uintptr_t)(halo_object_table->first_element) + offset);

    // Is the salt set in the table? If not, then this entry is unused right now.
    if(table_entry->salt == 0) {
        return NULL;
    }

    // Does the salt match?
    if(input_salt != 0 && table_entry->salt != input_salt) {
        return NULL;
    }

    // Does this flags thing not have parity?
    //
    // TODO: The purpose of this needs more investigation
    uint32_t v = 1 << table_entry->flags_high;
    if((v & flag_check) == 0) {
        return NULL;
    }

    // Done
    return table_entry->object;
}
