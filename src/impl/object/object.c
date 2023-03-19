#include <windows.h>
#include <float.h>
#include <math.h>
#include "object.h"

#include <stdio.h>

#include "../console/console.h"
#include "ringhopper/object.h"

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

DynamicObjectBase *resolve_object_id_to_data(uint32_t flag_check, TableID object_id) {
    ObjectTable *halo_object_table = *object_table;

    // Split it
    uint16_t input_index = ID_INDEX_PART(object_id);
    uint16_t input_salt = ID_SALT_PART(object_id);

    // Is it null?
    if(ID_IS_NULL(object_id)) {
        return NULL;
    }

    // Check if out of bounds.
    //
    // NOTE: The original code does a signed check which isn't necessary here.
    if(input_index >= halo_object_table->max_elements) {
        console_printf_debug_err("Tried to access dynamic object #%u when there are #%u max element(s)", input_index, halo_object_table->max_elements);
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

TableID resolve_parent_object_id(TableID object_id) {
    ObjectTableEntry *entries = (*object_table)->first_element;
    if(!ID_IS_NULL(object_id)) {
        while(1) {
            TableID parent_id = entries[ID_INDEX_PART(object_id)].object->parent;
            if(ID_IS_NULL(parent_id)) {
                break;
            }
            object_id = parent_id;
        }
    }
    return object_id;
}

#define MAX_OBJECT_NAMES 0x200

TableID **object_name_ids = (TableID **)(0x6AFF58);

TableID resolve_object_name_to_id(uint16_t name_index) {
    if(name_index == 0xFFFF || name_index >= MAX_OBJECT_NAMES) {
        return NULL_ID;
    }
    return (*object_name_ids)[name_index];
}

DynamicObjectBase *resolve_object_index_to_data_unchecked(uint16_t object_index) {
    return (*object_table)->first_element[object_index].object;
}

void get_center_and_render_bounding_radius_of_object(TableID object_id, VectorXYZ *center, float *render_bounding_radius) {
    DynamicObjectBase *object = resolve_object_index_to_data_unchecked(ID_INDEX_PART(object_id));
    *center = object->center;
    *render_bounding_radius = ((Object *)(get_tag_data(object->tag)))->render_bounding_radius;
}

static uint16_t *model_detail_setting = (uint16_t *)(0x681378);
typedef enum ModelDetailSetting {
    ModelDetailSetting_low = 0,
    ModelDetailSetting_medium = 1,
    ModelDetailSetting_high = 2
} ModelDetailSetting;

float calculate_lod_pixels(TableID object_id) {
    DynamicObjectBase *object = resolve_object_index_to_data_unchecked(ID_INDEX_PART(object_id));

    if(object->flags_1 & DynamicObjectFlags1_render_at_full_lod) {
        return FLT_MAX;
    }

    float height = object->height;
    switch ((ModelDetailSetting)(*model_detail_setting)) {
        case ModelDetailSetting_low: height *= 0.25; break;
        case ModelDetailSetting_medium: height *= 0.50; break;
        default: break;
    }

    // appears to be part of a larger structure at 0x7B2608
    float *camx = (float *)(0x7B2608 + 0x1C);
    float *camy = (float *)(0x7B2608 + 0x28);
    float *camz = (float *)(0x7B2608 + 0x34);
    float *camq = (float *)(0x7B2608 + 0x40);
    float *camr = (float *)(0x7B2608 + 0x188);

    float d = *camx * object->center.x
            + *camy * object->center.y
            + *camz * object->center.z
            + *camq;
    float denominator = fmax(fabs(d), 0.1);
    return *camr / denominator * height;
}
