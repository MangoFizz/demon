#ifndef DEMON__IMPL_TAG_H
#define DEMON__IMPL_TAG_H

#include <stdint.h>

struct Scenario;

typedef uint32_t FourCC;

#include "table/table.h"

typedef struct TagReference {
    FourCC group_fourcc;
    char unused1[4];
    char unused2[4];
    TableID tag_id;
} TagReference;
_Static_assert(sizeof(TagReference) == 0x10);

/**
 * Get the data for the tag.
 *
 * @param tag_id tag id
 *
 * @return pointer to tag data
 */
void *get_tag_data(TableID tag_id);

/**
 * Get the path for the tag
 *
 * @param tag_id tag id
 *
 * @return pointer to tag path
 */
const char *get_tag_path(TableID tag_id);

/**
 * Lookup the tag.
 *
 * @param path  path of the tag (not including file extension)
 * @param group group of the tag
 *
 * @return tag ID if found, NULL_ID if not
 */
TableID lookup_tag(const char *path, FourCC group);

/**
 * Get the data of the currently loaded scenario tag.
 */
struct Scenario *get_scenario_tag_data(void);

/**
 * Convert a tag group FourCC to a name.
 *
 * @param group_fourcc fourcc to look up
 *
 * @return tag group or "???" if unknown
 */
const char *group_fourcc_to_name(FourCC group_fourcc);

/**
 * Get the base name of the tag path.
 *
 * @param path tag path to look
 *
 * @return base name
 */
const char *get_tag_base_name(const char *path);

#define FOURCC_ACTOR ((FourCC)(0x61637472))
#define FOURCC_ACTOR_VARIANT ((FourCC)(0x61637476))
#define FOURCC_ANTENNA ((FourCC)(0x616E7421))
#define FOURCC_MODEL_ANIMATIONS ((FourCC)(0x616E7472))
#define FOURCC_BIPED ((FourCC)(0x62697064))
#define FOURCC_BITMAP ((FourCC)(0x6269746D))
#define FOURCC_SPHEROID ((FourCC)(0x626F6F6D))
#define FOURCC_CONTINUOUS_DAMAGE_EFFECT ((FourCC)(0x63646D67))
#define FOURCC_MODEL_COLLISION_GEOMETRY ((FourCC)(0x636F6C6C))
#define FOURCC_COLOR_TABLE ((FourCC)(0x636F6C6F))
#define FOURCC_CONTRAIL ((FourCC)(0x636F6E74))
#define FOURCC_DEVICE_CONTROL ((FourCC)(0x6374726C))
#define FOURCC_DECAL ((FourCC)(0x64656361))
#define FOURCC_UI_WIDGET_DEFINITION ((FourCC)(0x44654C61))
#define FOURCC_INPUT_DEVICE_DEFAULTS ((FourCC)(0x64657663))
#define FOURCC_DEVICE ((FourCC)(0x64657669))
#define FOURCC_DETAIL_OBJECT_COLLECTION ((FourCC)(0x646F6263))
#define FOURCC_EFFECT ((FourCC)(0x65666665))
#define FOURCC_EQUIPMENT ((FourCC)(0x65716970))
#define FOURCC_FLAG ((FourCC)(0x666C6167))
#define FOURCC_FOG ((FourCC)(0x666F6720))
#define FOURCC_FONT ((FourCC)(0x666F6E74))
#define FOURCC_MATERIAL_EFFECTS ((FourCC)(0x666F6F74))
#define FOURCC_GARBAGE ((FourCC)(0x67617262))
#define FOURCC_GLOW ((FourCC)(0x676C7721))
#define FOURCC_GRENADE_HUD_INTERFACE ((FourCC)(0x67726869))
#define FOURCC_HUD_MESSAGE_TEXT ((FourCC)(0x686D7420))
#define FOURCC_HUD_NUMBER ((FourCC)(0x68756423))
#define FOURCC_HUD_GLOBALS ((FourCC)(0x68756467))
#define FOURCC_ITEM ((FourCC)(0x6974656D))
#define FOURCC_ITEM_COLLECTION ((FourCC)(0x69746D63))
#define FOURCC_DAMAGE_EFFECT ((FourCC)(0x6A707421))
#define FOURCC_LENS_FLARE ((FourCC)(0x6C656E73))
#define FOURCC_LIGHTNING ((FourCC)(0x656C6563))
#define FOURCC_DEVICE_LIGHT_FIXTURE ((FourCC)(0x6C696669))
#define FOURCC_LIGHT ((FourCC)(0x6C696768))
#define FOURCC_SOUND_LOOPING ((FourCC)(0x6C736E64))
#define FOURCC_DEVICE_MACHINE ((FourCC)(0x6D616368))
#define FOURCC_GLOBALS ((FourCC)(0x6D617467))
#define FOURCC_METER ((FourCC)(0x6D657472))
#define FOURCC_LIGHT_VOLUME ((FourCC)(0x6D677332))
#define FOURCC_GBXMODEL ((FourCC)(0x6D6F6432))
#define FOURCC_MODEL ((FourCC)(0x6D6F6465))
#define FOURCC_MULTIPLAYER_SCENARIO_DESCRIPTION ((FourCC)(0x6D706C79))
#define FOURCC_PREFERENCES_NETWORK_GAME ((FourCC)(0x6E677072))
#define FOURCC_OBJECT ((FourCC)(0x6F626A65))
#define FOURCC_PARTICLE ((FourCC)(0x70617274))
#define FOURCC_PARTICLE_SYSTEM ((FourCC)(0x7063746C))
#define FOURCC_PHYSICS ((FourCC)(0x70687973))
#define FOURCC_PLACEHOLDER ((FourCC)(0x706C6163))
#define FOURCC_POINT_PHYSICS ((FourCC)(0x70706879))
#define FOURCC_PROJECTILE ((FourCC)(0x70726F6A))
#define FOURCC_WEATHER_PARTICLE_SYSTEM ((FourCC)(0x7261696E))
#define FOURCC_SCENARIO_STRUCTURE_BSP ((FourCC)(0x73627370))
#define FOURCC_SCENERY ((FourCC)(0x7363656E))
#define FOURCC_SHADER_TRANSPARENT_CHICAGO_EXTENDED ((FourCC)(0x73636578))
#define FOURCC_SHADER_TRANSPARENT_CHICAGO ((FourCC)(0x73636869))
#define FOURCC_SCENARIO ((FourCC)(0x73636E72))
#define FOURCC_SHADER_ENVIRONMENT ((FourCC)(0x73656E76))
#define FOURCC_SHADER_TRANSPARENT_GLASS ((FourCC)(0x73676C61))
#define FOURCC_SHADER ((FourCC)(0x73686472))
#define FOURCC_SKY ((FourCC)(0x736B7920))
#define FOURCC_SHADER_TRANSPARENT_METER ((FourCC)(0x736D6574))
#define FOURCC_SOUND ((FourCC)(0x736E6421))
#define FOURCC_SOUND_ENVIRONMENT ((FourCC)(0x736E6465))
#define FOURCC_SHADER_MODEL ((FourCC)(0x736F736F))
#define FOURCC_SHADER_TRANSPARENT_GENERIC ((FourCC)(0x736F7472))
#define FOURCC_UI_WIDGET_COLLECTION ((FourCC)(0x536F756C))
#define FOURCC_SHADER_TRANSPARENT_PLASMA ((FourCC)(0x73706C61))
#define FOURCC_SOUND_SCENERY ((FourCC)(0x73736365))
#define FOURCC_STRING_LIST ((FourCC)(0x73747223))
#define FOURCC_SHADER_TRANSPARENT_WATER ((FourCC)(0x73776174))
#define FOURCC_TAG_COLLECTION ((FourCC)(0x74616763))
#define FOURCC_CAMERA_TRACK ((FourCC)(0x7472616B))
#define FOURCC_DIALOGUE ((FourCC)(0x75646C67))
#define FOURCC_UNIT_HUD_INTERFACE ((FourCC)(0x756E6869))
#define FOURCC_UNIT ((FourCC)(0x756E6974))
#define FOURCC_UNICODE_STRING_LIST ((FourCC)(0x75737472))
#define FOURCC_VIRTUAL_KEYBOARD ((FourCC)(0x76636B79))
#define FOURCC_VEHICLE ((FourCC)(0x76656869))
#define FOURCC_WEAPON ((FourCC)(0x77656170))
#define FOURCC_WIND ((FourCC)(0x77696E64))
#define FOURCC_WEAPON_HUD_INTERFACE ((FourCC)(0x77706869))

#endif
