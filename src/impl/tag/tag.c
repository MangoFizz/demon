#include <string.h>
#include "tag.h"

#include "../types/types.h"
#include "../console/console.h"

uint8_t *map_loaded = (uint8_t *)(0x69F4F8);
struct Scenario **scenario_tag_data = (struct Scenario **)(0x7367AC);

typedef struct TagEntry {
    FourCC primary_group;
    FourCC secondary_group;
    FourCC tertiary_group;
    TableID id;
    char *path;
    void *data;
    char padding[0x8];
} TagEntry;
_Static_assert(sizeof(TagEntry) == 0x20);

typedef struct TagDataHeader {
    TagEntry *tags;
    TableID scenario_tag_id;
    uint32_t tag_checksum;
    uint32_t tag_count;
    uint32_t model_part_count;
    uint32_t model_data_file_offset;
    uint32_t model_part_count2;
    uint32_t vertex_size;
    uint32_t model_data_size;
    FourCC tags_fourcc;
} TagDataHeader;
_Static_assert(sizeof(TagDataHeader) == 0x28);

TagDataHeader **tag_data_header_loaded = (TagDataHeader **)(0x69FCFC);

void *get_tag_data(TableID tag_id) {
    return (*tag_data_header_loaded)->tags[ID_INDEX_PART(tag_id)].data;
}

const char *get_tag_path(TableID tag_id) {
    return (*tag_data_header_loaded)->tags[ID_INDEX_PART(tag_id)].path;
}

FourCC get_tag_group_fourcc(TableID tag_id) {
    return (*tag_data_header_loaded)->tags[ID_INDEX_PART(tag_id)].primary_group;
}

TableID lookup_tag(const char *path, FourCC group) {
    if(!*map_loaded) {
        return NULL_ID;
    }

    TagDataHeader *header = *tag_data_header_loaded;
    uint32_t tag_count = header->tag_count;

    TagEntry *tag = header->tags;
    for(uint32_t i = 0; i < tag_count; i++, tag++) {
        if(tag->primary_group == group && strcmp(path, tag->path) == 0) {
            return tag->id;
        }
    }

    console_printf_debug_err("lookup_tag failed!");
    console_printf_debug_err("    path: %s.%s", path, group_fourcc_to_name(group));
    return NULL_ID;
}

struct Scenario *get_scenario_tag_data(void) {
    return *scenario_tag_data;
}

const char *group_fourcc_to_name(FourCC group_fourcc) {
    switch(group_fourcc) {
        case FOURCC_ACTOR:
            return "actor";
        case FOURCC_ACTOR_VARIANT:
            return "actor_variant";
        case FOURCC_ANTENNA:
            return "antenna";
        case FOURCC_MODEL_ANIMATIONS:
            return "model_animations";
        case FOURCC_BIPED:
            return "biped";
        case FOURCC_BITMAP:
            return "bitmap";
        case FOURCC_SPHEROID:
            return "spheroid";
        case FOURCC_CONTINUOUS_DAMAGE_EFFECT:
            return "continuous_damage_effect";
        case FOURCC_MODEL_COLLISION_GEOMETRY:
            return "model_collision_geometry";
        case FOURCC_COLOR_TABLE:
            return "color_table";
        case FOURCC_CONTRAIL:
            return "contrail";
        case FOURCC_DEVICE_CONTROL:
            return "device_control";
        case FOURCC_DECAL:
            return "decal";
        case FOURCC_UI_WIDGET_DEFINITION:
            return "ui_widget_definition";
        case FOURCC_INPUT_DEVICE_DEFAULTS:
            return "input_device_defaults";
        case FOURCC_DEVICE:
            return "device";
        case FOURCC_DETAIL_OBJECT_COLLECTION:
            return "detail_object_collection";
        case FOURCC_EFFECT:
            return "effect";
        case FOURCC_EQUIPMENT:
            return "equipment";
        case FOURCC_FLAG:
            return "flag";
        case FOURCC_FOG:
            return "fog";
        case FOURCC_FONT:
            return "font";
        case FOURCC_MATERIAL_EFFECTS:
            return "material_effects";
        case FOURCC_GARBAGE:
            return "garbage";
        case FOURCC_GLOW:
            return "glow";
        case FOURCC_GRENADE_HUD_INTERFACE:
            return "grenade_hud_interface";
        case FOURCC_HUD_MESSAGE_TEXT:
            return "hud_message_text";
        case FOURCC_HUD_NUMBER:
            return "hud_number";
        case FOURCC_HUD_GLOBALS:
            return "hud_globals";
        case FOURCC_ITEM:
            return "item";
        case FOURCC_ITEM_COLLECTION:
            return "item_collection";
        case FOURCC_DAMAGE_EFFECT:
            return "damage_effect";
        case FOURCC_LENS_FLARE:
            return "lens_flare";
        case FOURCC_LIGHTNING:
            return "lightning";
        case FOURCC_DEVICE_LIGHT_FIXTURE:
            return "device_light_fixture";
        case FOURCC_LIGHT:
            return "light";
        case FOURCC_SOUND_LOOPING:
            return "sound_looping";
        case FOURCC_DEVICE_MACHINE:
            return "device_machine";
        case FOURCC_GLOBALS:
            return "globals";
        case FOURCC_METER:
            return "meter";
        case FOURCC_LIGHT_VOLUME:
            return "light_volume";
        case FOURCC_GBXMODEL:
            return "gbxmodel";
        case FOURCC_MODEL:
            return "model";
        case FOURCC_MULTIPLAYER_SCENARIO_DESCRIPTION:
            return "multiplayer_scenario_description";
        case FOURCC_PREFERENCES_NETWORK_GAME:
            return "preferences_network_game";
        case FOURCC_OBJECT:
            return "object";
        case FOURCC_PARTICLE:
            return "particle";
        case FOURCC_PARTICLE_SYSTEM:
            return "particle_system";
        case FOURCC_PHYSICS:
            return "physics";
        case FOURCC_PLACEHOLDER:
            return "placeholder";
        case FOURCC_POINT_PHYSICS:
            return "point_physics";
        case FOURCC_PROJECTILE:
            return "projectile";
        case FOURCC_WEATHER_PARTICLE_SYSTEM:
            return "weather_particle_system";
        case FOURCC_SCENARIO_STRUCTURE_BSP:
            return "scenario_structure_bsp";
        case FOURCC_SCENERY:
            return "scenery";
        case FOURCC_SHADER_TRANSPARENT_CHICAGO_EXTENDED:
            return "shader_transparent_chicago_extended";
        case FOURCC_SHADER_TRANSPARENT_CHICAGO:
            return "shader_transparent_chicago";
        case FOURCC_SCENARIO:
            return "scenario";
        case FOURCC_SHADER_ENVIRONMENT:
            return "shader_environment";
        case FOURCC_SHADER_TRANSPARENT_GLASS:
            return "shader_transparent_glass";
        case FOURCC_SHADER:
            return "shader";
        case FOURCC_SKY:
            return "sky";
        case FOURCC_SHADER_TRANSPARENT_METER:
            return "shader_transparent_meter";
        case FOURCC_SOUND:
            return "sound";
        case FOURCC_SOUND_ENVIRONMENT:
            return "sound_environment";
        case FOURCC_SHADER_MODEL:
            return "shader_model";
        case FOURCC_SHADER_TRANSPARENT_GENERIC:
            return "shader_transparent_generic";
        case FOURCC_UI_WIDGET_COLLECTION:
            return "ui_widget_collection";
        case FOURCC_SHADER_TRANSPARENT_PLASMA:
            return "shader_transparent_plasma";
        case FOURCC_SOUND_SCENERY:
            return "sound_scenery";
        case FOURCC_STRING_LIST:
            return "string_list";
        case FOURCC_SHADER_TRANSPARENT_WATER:
            return "shader_transparent_water";
        case FOURCC_TAG_COLLECTION:
            return "tag_collection";
        case FOURCC_CAMERA_TRACK:
            return "camera_track";
        case FOURCC_DIALOGUE:
            return "dialogue";
        case FOURCC_UNIT_HUD_INTERFACE:
            return "unit_hud_interface";
        case FOURCC_UNIT:
            return "unit";
        case FOURCC_UNICODE_STRING_LIST:
            return "unicode_string_list";
        case FOURCC_VIRTUAL_KEYBOARD:
            return "virtual_keyboard";
        case FOURCC_VEHICLE:
            return "vehicle";
        case FOURCC_WEAPON:
            return "weapon";
        case FOURCC_WIND:
            return "wind";
        case FOURCC_WEAPON_HUD_INTERFACE:
            return "weapon_hud_interface";
        default:
            return "???";
    }
}

const char *get_tag_base_name(const char *path) {
    const char *end = strrchr(path, '\\');
    if(end) {
        return end + 1;
    }
    else {
        return path;
    }
}
