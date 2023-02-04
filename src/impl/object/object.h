#ifndef DEMON__IMPL_OBJECT_OBJECT_H
#define DEMON__IMPL_OBJECT_OBJECT_H

#include "../table/table.h"
#include "../3d.h"
#include "../misc_types.h"

typedef struct DynamicObjectBase {
    TableID tag;
    uint8_t field_0x4[0x4];
    uint32_t flags_0;
    uint32_t existence_time;
    uint32_t flags_1;
    uint8_t field_0x14[0x48];
    VectorXYZ position;
    VectorXYZ velocity;
    VectorIJK rotation[2];
    VectorPYR rotation_velocity;
    uint8_t field_0x98[0x8];
    VectorXYZ center;
    uint8_t field_0xac[0x4];
    float scale;
    uint16_t object_type; // ObjectType
    uint8_t field_0xb6[0x16];
    TableID animation_tag_id;
    uint16_t animation;
    uint16_t animation_frame;
    uint8_t field_0xd4[0x4];
    float base_health;
    float base_shield;
    float health;
    float shield;
    float current_shield_damage;
    float current_health_damage;
    uint8_t field_0xf0[0x4];
    float recent_shield_damage;
    float recent_health_damage;
    uint32_t recent_shield_damage_time;
    uint32_t recent_health_damage_time;
    uint16_t shield_stun_time;
    uint16_t flags_2;
    uint8_t field_0x108[0x10];
    TableID weapon;
    TableID parent;
    uint16_t parent_seat_index;
    uint8_t field_0x122[0x66];
    ColorRGB color_change[4][2];
    uint8_t field_0x1e8[0xC];
} DynamicObjectBase;
_Static_assert(sizeof(DynamicObjectBase) == 0x1F4);

/**
 * Resolve an object ID into a dynamic object.
 *
 * @param object_id  object ID to check (salt is checked unless salt is set to 0)
 * @param flag_check does a parity check in the entry (needs more investigation)
 *
 * @return pointer to object or NULL if object_id does not match anything
 */
DynamicObjectBase *resolve_object_id_to_data(uint32_t flag_check, TableID object_id);

/**
 * Resolve an object index into a dynamic object
 *
 * @param object_index object index to use
 *
 * @return pointer to object or garbage if it doesn't point to anything valid
 */
DynamicObjectBase *resolve_object_index_to_data_unchecked(uint16_t object_index);

/**
 * Resolve the parent ID of the object recursively (i.e. until it gets an object that has no parent).
 *
 * @param object_id object ID to start with
 *
 * @return parent object ID or a null object ID if object_id is null
 */
TableID resolve_parent_object_id(TableID object_id);

/**
 * Resolve the object name index to an object ID
 *
 * @param name_index index of the object name in the scenario
 *
 * @return object ID that corresponds to the object name
 */
TableID resolve_object_name_to_data(uint16_t name_index);

/**
 * Find the object's center and render bounding radius.
 *
 * @param object_id              object to check
 * @param center                 (out) center coordinates
 * @param render_bounding_radius (out) render bounding radius
 */
void get_center_and_render_bounding_radius_of_object(TableID object_id, VectorXYZ *center, float *render_bounding_radius);

#endif
